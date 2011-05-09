/*
 * Hedgewars, a free turn based strategy game
 * Copyright (c) 2006-2011 Andrey Korotaev <unC0Rr@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef PAGE_NETGAME_H
#define PAGE_NETGAME_H

#include "pages.h"
class HWChatWidget;

class PageNetGame : public AbstractPage
{
    Q_OBJECT

public:
    PageNetGame(QWidget* parent, QSettings * config, SDLInteraction * sdli);

    QPushButton *BtnBack;
    QPushButton *BtnGo;
    QPushButton *BtnMaster;
    QPushButton *BtnStart;
    QPushButton *BtnUpdate;

    QLineEdit * leRoomName;

    QAction * restrictJoins;
    QAction * restrictTeamAdds;

    HWChatWidget* pChatWidget;

    TeamSelWidget* pNetTeamsWidget;
    GameCFGWidget* pGameCFG;

public slots:
    void setReadyStatus(bool isReady);
    void onUpdateClick();
    void setMasterMode(bool isMaster);

signals:
    void SetupClicked();
    void askForUpdateRoomName(const QString &);
};

#endif
