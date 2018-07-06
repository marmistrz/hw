use mio;

use server::{
    server::HWServer,
    client::ClientId,
    actions::{Action, Action::*}
};
use protocol::messages::{
    HWProtocolMessage,
    HWServerMessage::*
};
use utils::is_name_illegal;
use super::common::rnd_reply;

pub fn handle(server: &mut HWServer, client_id: ClientId, message: HWProtocolMessage) {
    use protocol::messages::HWProtocolMessage::*;
    match message {
        CreateRoom(name, password) => {
            let actions =
                if is_name_illegal(&name) {
                    vec![Warn("Illegal room name! A room name must be between 1-40 characters long, must not have a trailing or leading space and must not have any of these characters: $()*+?[]^{|}".to_string())]
                } else if server.has_room(&name) {
                    vec![Warn("A room with the same name already exists.".to_string())]
                } else {
                    let flags_msg = ClientFlags(
                        "+hr".to_string(),
                        vec![server.clients[client_id].nick.clone()]);
                    vec![AddRoom(name, password),
                         flags_msg.send_self().action()]
                };
            server.react(client_id, actions);
        },
        Chat(msg) => {
            let chat_msg = ChatMsg {nick: server.clients[client_id].nick.clone(), msg: msg};
            server.react(client_id, vec![chat_msg.send_all().but_self().action()]);
        },
        JoinRoom(name, password) => {
            let actions;
            {
                let room = server.rooms.iter().find(|(_, r)| r.name == name);
                let room_id = room.map(|(_, r)| r.id);
                let nicks = server.clients.iter()
                    .filter(|(_, c)| c.room_id == room_id)
                    .map(|(_, c)| c.nick.clone())
                    .collect();
                let c = &mut server.clients[client_id];

                actions = if let Some((_, r)) = room {
                    if c.protocol_number != r.protocol_number {
                        vec![Warn("Room version incompatible to your Hedgewars version!".to_string())]
                    } else {
                        vec![MoveToRoom(r.id),
                             RoomJoined(nicks).send_self().action()]
                    }
                } else {
                    vec![Warn("No such room.".to_string())]
                };
            }
            server.react(client_id, actions);
        },
        Rnd(v) => server.react(client_id, rnd_reply(v)),
        List => warn!("Deprecated LIST message received"),
        _ => warn!("Incorrect command in lobby state"),
    }
}
