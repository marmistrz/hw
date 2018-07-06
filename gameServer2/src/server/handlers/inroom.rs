use mio;

use protocol::messages::{
    HWProtocolMessage,
    HWServerMessage::*
};
use server::{
    server::HWServer,
    client::ClientId,
    room::HWRoom,
    actions::{Action, Action::*}
};
use utils::is_name_illegal;
use std::mem::swap;
use base64::{encode, decode};
use super::common::rnd_reply;

#[derive(Clone)]
struct ByMsg<'a> {
    messages: &'a[u8]
}

impl <'a> Iterator for ByMsg<'a> {
    type Item = &'a[u8];

    fn next(&mut self) -> Option<<Self as Iterator>::Item> {
        if let Some(size) = self.messages.get(0) {
            let (msg, next) = self.messages.split_at(*size as usize + 1);
            self.messages = next;
            Some(msg)
        } else {
            None
        }
    }
}

fn by_msg(source: &Vec<u8>) -> ByMsg {
    ByMsg {messages: &source[..]}
}

const VALID_MESSAGES: &[u8] =
    b"M#+LlRrUuDdZzAaSjJ,NpPwtgfhbc12345\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A";
const NON_TIMED_MESSAGES: &[u8] = b"M#hb";

#[cfg(canhazslicepatterns)]
fn is_msg_valid(msg: &[u8], team_indices: &[u8]) -> bool {
    match msg {
        [size, typ, body..] => VALID_MESSAGES.contains(typ)
            && match body {
                [1...8, team, ..] if *typ == b'h' => team_indices.contains(team),
                _ => *typ != b'h'
            },
        _ => false
    }
}

fn is_msg_valid(msg: &[u8], team_indices: &[u8]) -> bool {
    if let Some(typ) = msg.get(1) {
        VALID_MESSAGES.contains(typ)
    } else {
        false
    }
}

fn is_msg_empty(msg: &[u8]) -> bool {
    msg.get(1).filter(|t| **t == b'+').is_some()
}

pub fn handle(server: &mut HWServer, client_id: ClientId, message: HWProtocolMessage) {
    use protocol::messages::HWProtocolMessage::*;
    match message {
        Part(None) => server.react(client_id, vec![
            MoveToLobby("part".to_string())]),
        Part(Some(msg)) => server.react(client_id, vec![
            MoveToLobby(format!("part: {}", msg))]),
        Chat(msg) => {
            let actions = {
                let c = &mut server.clients[client_id];
                let chat_msg = ChatMsg {nick: c.nick.clone(), msg: msg};
                if let Some(room_id) = c.room_id {
                    vec![chat_msg.send_all().in_room(room_id).but_self().action()]
                } else {
                    Vec::new()
                }
            };
            server.react(client_id, actions);
        },
        RoomName(new_name) => {
            let actions =
                if is_name_illegal(&new_name) {
                    vec![Warn("Illegal room name! A room name must be between 1-40 characters long, must not have a trailing or leading space and must not have any of these characters: $()*+?[]^{|}".to_string())]
                } else if server.has_room(&new_name) {
                    vec![Warn("A room with the same name already exists.".to_string())]
                } else {
                    let mut old_name = new_name.clone();
                    if let (_, Some(r)) = server.client_and_room(client_id) {
                        swap(&mut r.name, &mut old_name);
                        vec![SendRoomUpdate(Some(old_name))]
                    } else {
                        Vec::new()
                    }
                };
            server.react(client_id, actions);
        },
        ToggleReady => {
            let actions = if let (c, Some(r)) = server.client_and_room(client_id) {
                let flags = if c.is_ready {
                    r.ready_players_number -= 1;
                    "-r"
                } else {
                    r.ready_players_number += 1;
                    "+r"
                };
                c.is_ready = !c.is_ready;
                vec![ClientFlags(flags.to_string(), vec![c.nick.clone()])
                    .send_all().in_room(r.id).action()]
            } else {
                Vec::new()
            };
            server.react(client_id, actions);
        }
        AddTeam(info) => {
            let mut actions = Vec::new();
            if let (c, Some(r)) = server.client_and_room(client_id) {
                let room_id = r.id;
                if r.teams.len() >= r.team_limit as usize {
                    actions.push(Warn("Too many teams!".to_string()))
                } else if r.addable_hedgehogs() == 0 {
                    actions.push(Warn("Too many hedgehogs!".to_string()))
                } else if r.find_team(|t| t.name == info.name) != None {
                    actions.push(Warn("There's already a team with same name in the list.".to_string()))
                } else if r.game_info.is_some() {
                    actions.push(Warn("Joining not possible: Round is in progress.".to_string()))
                } else {
                    let team = r.add_team(c.id, info);
                    c.teams_in_game += 1;
                    c.clan = Some(team.color);
                    actions.push(TeamAccepted(team.name.clone())
                        .send_self().action());
                    actions.push(TeamAdd(HWRoom::team_info(&c, team))
                        .send_all().in_room(room_id).but_self().action());
                    actions.push(TeamColor(team.name.clone(), team.color)
                        .send_all().in_room(room_id).action());
                    actions.push(HedgehogsNumber(team.name.clone(), team.hedgehogs_number)
                        .send_all().in_room(room_id).action());
                    actions.push(SendRoomUpdate(None));
                }
            }
            server.react(client_id, actions);
        },
        RemoveTeam(name) => {
            let mut actions = Vec::new();
            if let (c, Some(r)) = server.client_and_room(client_id) {
                match r.find_team_owner(&name) {
                    None =>
                        actions.push(Warn("Error: The team you tried to remove does not exist.".to_string())),
                    Some((id, _)) if id != client_id =>
                        actions.push(Warn("You can't remove a team you don't own.".to_string())),
                    Some((_, name)) => {
                        c.teams_in_game -= 1;
                        c.clan = r.find_team_color(c.id);
                        actions.push(Action::RemoveTeam(name.to_string()));
                    }
                }
            };
            server.react(client_id, actions);
        },
        SetHedgehogsNumber(team_name, number) => {
            let actions = if let (c, Some(r)) = server.client_and_room(client_id) {
                let room_id = r.id;
                let addable_hedgehogs = r.addable_hedgehogs();
                if let Some((_, mut team)) = r.find_team_and_owner_mut(|t| t.name == team_name) {
                    if !c.is_master {
                        vec![ProtocolError("You're not the room master!".to_string())]
                    } else if number < 1 || number > 8
                           || number > addable_hedgehogs + team.hedgehogs_number {
                        vec![HedgehogsNumber(team.name.clone(), team.hedgehogs_number)
                            .send_self().action()]
                    } else {
                        team.hedgehogs_number = number;
                        vec![HedgehogsNumber(team.name.clone(), number)
                            .send_all().in_room(room_id).but_self().action()]
                    }
                } else {
                    vec![(Warn("No such team.".to_string()))]
                }
            } else {
                Vec::new()
            };
            server.react(client_id, actions);
        },
        SetTeamColor(team_name, color) => {
            let mut owner_id = None;
            let actions = if let (c, Some(r)) = server.client_and_room(client_id) {
                let room_id = r.id;
                if let Some((owner, mut team)) = r.find_team_and_owner_mut(|t| t.name == team_name) {
                    if !c.is_master {
                        vec![ProtocolError("You're not the room master!".to_string())]
                    } else if false  {
                        Vec::new()
                    } else {
                        owner_id = Some(owner);
                        team.color = color;
                        vec![TeamColor(team.name.clone(), color)
                            .send_all().in_room(room_id).but_self().action()]
                    }
                } else {
                    vec![(Warn("No such team.".to_string()))]
                }
            } else {
                Vec::new()
            };

            if let Some(id) = owner_id {
                server.clients[id].clan = Some(color);
            }

            server.react(client_id, actions);
        },
        Cfg(cfg) => {
            let actions = if let (c, Some(r)) = server.client_and_room(client_id) {
                if !c.is_master {
                    vec![ProtocolError("You're not the room master!".to_string())]
                } else {
                    r.set_config(cfg.clone());
                    vec![cfg.into_server_msg()
                        .send_all().in_room(r.id).but_self().action()]
                }
            } else {
                Vec::new()
            };
            server.react(client_id, actions);
        }
        StartGame => {
            let actions = if let (_, Some(r)) = server.client_and_room(client_id) {
                vec![StartRoomGame(r.id)]
            } else {
                Vec::new()
            };
            server.react(client_id, actions);
        }
        EngineMessage(em) => {
            let mut actions = Vec::new();
            if let (c, Some(r)) = server.client_and_room(client_id) {
                if c.teams_in_game > 0 {
                    let decoding = decode(&em[..]).unwrap();
                    let messages = by_msg(&decoding);
                    let valid = messages.clone().filter(|m| is_msg_valid(m, &c.team_indices));
                    let non_empty = messages.filter(|m| !is_msg_empty(m));
                    let last_msg = None;

                    let em_response = encode(&valid.flat_map(|msg| msg).cloned().collect::<Vec<_>>());
                    if !em_response.is_empty() {
                        actions.push(ForwardEngineMessage(vec![em_response])
                            .send_all().in_room(r.id).but_self().action());
                    }
                    let em_log = encode(&non_empty.flat_map(|msg| msg).cloned().collect::<Vec<_>>());
                    if let Some(ref mut info) = r.game_info {
                        if !em_log.is_empty() {
                            info.msg_log.push(em_log);
                        }
                        if last_msg.is_some() {
                            info.last_msg = last_msg;
                        }
                    }
                }
            }
            server.react(client_id, actions)
        }
        RoundFinished => {
            let mut actions = Vec::new();
            if let (c, Some(r)) = server.client_and_room(client_id) {
                if c.is_in_game {
                    c.is_in_game = false;
                    actions.push(ClientFlags("-g".to_string(), vec![c.nick.clone()]).
                        send_all().in_room(r.id).action());
                    if r.game_info.is_some() {
                        for team in r.client_teams(c.id) {
                            actions.push(SendTeamRemovalMessage(team.name.clone()));
                        }
                    }
                }
            }
            server.react(client_id, actions)
        },
        Rnd(v) => server.react(client_id, rnd_reply(v)),
        _ => warn!("Unimplemented!")
    }
}
