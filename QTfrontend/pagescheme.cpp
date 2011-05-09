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

#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QSpinBox>

#include "ammoSchemeModel.h"
#include "pagescheme.h"
#include "misc.h"

PageScheme::PageScheme(QWidget* parent) :
    AbstractPage(parent)
{
    QGridLayout * pageLayout = new QGridLayout(this);
    QGroupBox * gb = new QGroupBox(this);

    QGridLayout * gl = new QGridLayout();
    gb->setLayout(gl);
    QSizePolicy sp;
    sp.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    sp.setHorizontalPolicy(QSizePolicy::Expanding);

    pageLayout->addWidget(gb, 1,0,13,5);

    gbGameModes = new QGroupBox(QGroupBox::tr("Game Modifiers"), gb);
    gbBasicSettings = new QGroupBox(QGroupBox::tr("Basic Settings"), gb);

    gbGameModes->setStyleSheet(".QGroupBox {"
            "background-color: #130f2c; background-image:url();"
            "}");
    gbBasicSettings->setStyleSheet(".QGroupBox {"
            "background-color: #130f2c; background-image:url();"
            "}");

    gbGameModes->setSizePolicy(sp);
    gbBasicSettings->setSizePolicy(sp);
    gl->addWidget(gbGameModes,0,0,1,3,Qt::AlignTop);
    gl->addWidget(gbBasicSettings,0,3,1,3,Qt::AlignTop);

    QGridLayout * glGMLayout = new QGridLayout(gbGameModes);
    QGridLayout * glBSLayout = new QGridLayout(gbBasicSettings);
    gbGameModes->setLayout(glGMLayout);
    gbBasicSettings->setLayout(glBSLayout);
    // Left

    TBW_mode_Forts = new ToggleButtonWidget(gbGameModes, ":/res/btnForts.png");
    TBW_mode_Forts->setToolTip("<b>" + ToggleButtonWidget::tr("Fort Mode") + "</b>:<br />" + tr("Defend your fort and destroy the opponents, two team colours max!"));
    glGMLayout->addWidget(TBW_mode_Forts,0,0,1,1);

    TBW_teamsDivide = new ToggleButtonWidget(gbGameModes, ":/res/btnTeamsDivide.png");
    TBW_teamsDivide->setToolTip("<b>" + ToggleButtonWidget::tr("Divide Teams") + "</b>:<br />" + tr("Teams will start on opposite sides of the terrain, two team colours max!"));
    glGMLayout->addWidget(TBW_teamsDivide,0,1,1,1);

    TBW_solid = new ToggleButtonWidget(gbGameModes, ":/res/btnSolid.png");
    TBW_solid->setToolTip("<b>" + ToggleButtonWidget::tr("Solid Land") + "</b>:<br />" + tr("Land can not be destroyed!"));
    glGMLayout->addWidget(TBW_solid,0,2,1,1);

    TBW_border = new ToggleButtonWidget(gbGameModes, ":/res/btnBorder.png");
    TBW_border->setToolTip("<b>" + ToggleButtonWidget::tr("Add Border") + "</b>:<br />" + tr("Add an indestructable border around the terrain"));
    glGMLayout->addWidget(TBW_border,0,3,1,1);

    TBW_lowGravity = new ToggleButtonWidget(gbGameModes, ":/res/btnLowGravity.png");
    TBW_lowGravity->setToolTip("<b>" + ToggleButtonWidget::tr("Low Gravity") + "</b>:<br />" + tr("Lower gravity"));
    glGMLayout->addWidget(TBW_lowGravity,0,4,1,1);

    TBW_laserSight = new ToggleButtonWidget(gbGameModes, ":/res/btnLaserSight.png");
    TBW_laserSight->setToolTip("<b>" + ToggleButtonWidget::tr("Laser Sight") + "</b>:<br />" + tr("Assisted aiming with laser sight"));
    glGMLayout->addWidget(TBW_laserSight,1,0,1,1);

    TBW_invulnerable = new ToggleButtonWidget(gbGameModes, ":/res/btnInvulnerable.png");
    TBW_invulnerable->setToolTip("<b>" + ToggleButtonWidget::tr("Invulnerable") + "</b>:<br />" + tr("All hogs have a personal forcefield"));
    glGMLayout->addWidget(TBW_invulnerable,1,1,1,1);

    TBW_resethealth = new ToggleButtonWidget(gbGameModes, ":/res/btnResetHealth.png");
    TBW_resethealth->setToolTip("<b>" + ToggleButtonWidget::tr("Reset Health") + "</b>:<br />" + tr("All (living) hedgehogs are fully restored at the end of turn"));
    glGMLayout->addWidget(TBW_resethealth,1,2,1,1);

    TBW_vampiric = new ToggleButtonWidget(gbGameModes, ":/res/btnVampiric.png");
    TBW_vampiric->setToolTip("<b>" + ToggleButtonWidget::tr("Vampirism") + "</b>:<br />" + tr("Gain 80% of the damage you do back in health"));
    glGMLayout->addWidget(TBW_vampiric,1,3,1,1);

    TBW_karma = new ToggleButtonWidget(gbGameModes, ":/res/btnKarma.png");
    TBW_karma->setToolTip("<b>" + ToggleButtonWidget::tr("Karma") + "</b>:<br />" + tr("Share your opponents pain, share their damage"));
    glGMLayout->addWidget(TBW_karma,1,4,1,1);

    TBW_artillery = new ToggleButtonWidget(gbGameModes, ":/res/btnArtillery.png");
    TBW_artillery->setToolTip("<b>" + ToggleButtonWidget::tr("Artillery") + "</b>:<br />" + tr("Your hogs are unable to move, put your artillery skills to the test"));
    glGMLayout->addWidget(TBW_artillery,2,0,1,1);

    TBW_randomorder = new ToggleButtonWidget(gbGameModes, ":/res/btnRandomOrder.png");
    TBW_randomorder->setToolTip("<b>" + ToggleButtonWidget::tr("Random Order") + "</b>:<br />" + tr("Order of play is random instead of in room order."));
    glGMLayout->addWidget(TBW_randomorder,2,1,1,1);

    TBW_king = new ToggleButtonWidget(gbGameModes, ":/res/btnKing.png");
    TBW_king->setToolTip("<b>" + ToggleButtonWidget::tr("King") + "</b>:<br />" + tr("Play with a King. If he dies, your side dies."));
    glGMLayout->addWidget(TBW_king,2,2,1,1);

    TBW_placehog = new ToggleButtonWidget(gbGameModes, ":/res/btnPlaceHog.png");
    TBW_placehog->setToolTip("<b>" + ToggleButtonWidget::tr("Place Hedgehogs") + "</b>:<br />" + tr("Take turns placing your hedgehogs before the start of play."));
    glGMLayout->addWidget(TBW_placehog,2,3,1,1);

    TBW_sharedammo = new ToggleButtonWidget(gbGameModes, ":/res/btnSharedAmmo.png");
    TBW_sharedammo->setToolTip("<b>" + ToggleButtonWidget::tr("Clan Shares Ammo") + "</b>:<br />" + tr("Ammo is shared between all teams that share a colour."));
    glGMLayout->addWidget(TBW_sharedammo,2,4,1,1);

    TBW_disablegirders = new ToggleButtonWidget(gbGameModes, ":/res/btnDisableGirders.png");
    TBW_disablegirders->setToolTip("<b>" + ToggleButtonWidget::tr("Disable Girders") + "</b>:<br />" + tr("Disable girders when generating random maps."));
    glGMLayout->addWidget(TBW_disablegirders,3,0,1,1);

    TBW_disablelandobjects = new ToggleButtonWidget(gbGameModes, ":/res/btnDisableLandObjects.png");
    TBW_disablelandobjects->setToolTip("<b>" + ToggleButtonWidget::tr("Disable Land Objects") + "</b>:<br />" + tr("Disable land objects when generating random maps."));
    glGMLayout->addWidget(TBW_disablelandobjects,3,1,1,1);

    TBW_aisurvival = new ToggleButtonWidget(gbGameModes, ":/res/btnAISurvival.png");
    TBW_aisurvival->setToolTip("<b>" + ToggleButtonWidget::tr("AI Survival Mode") + "</b>:<br />" + tr("AI respawns on death."));
    glGMLayout->addWidget(TBW_aisurvival,3,2,1,1);

    TBW_infattack = new ToggleButtonWidget(gbGameModes, ":/res/btnInfAttack.png");
    TBW_infattack->setToolTip("<b>" + ToggleButtonWidget::tr("Unlimited Attacks") + "</b>:<br />" + tr("Attacking does not end your turn."));
    glGMLayout->addWidget(TBW_infattack,3,3,1,1);

    TBW_resetweps = new ToggleButtonWidget(gbGameModes, ":/res/btnResetWeps.png");
    TBW_resetweps->setToolTip("<b>" + ToggleButtonWidget::tr("Reset Weapons") + "</b>:<br />" + tr("Weapons are reset to starting values each turn."));
    glGMLayout->addWidget(TBW_resetweps,3,4,1,1);

    TBW_perhogammo = new ToggleButtonWidget(gbGameModes, ":/res/btnPerHogAmmo.png");
    TBW_perhogammo->setToolTip("<b>" + ToggleButtonWidget::tr("Per Hedgehog Ammo") + "</b>:<br />" + tr("Each hedgehog has its own ammo. It does not share with the team."));
    glGMLayout->addWidget(TBW_perhogammo,4,0,1,1);

    TBW_nowind = new ToggleButtonWidget(gbGameModes, ":/res/btnNoWind.png");
    TBW_nowind->setToolTip("<b>" + ToggleButtonWidget::tr("Disable Wind") + "</b>:<br />" + tr("You will not have to worry about wind anymore."));
    glGMLayout->addWidget(TBW_nowind,4,1,1,1);

    TBW_morewind = new ToggleButtonWidget(gbGameModes, ":/res/btnMoreWind.png");
    TBW_morewind->setToolTip("<b>" + ToggleButtonWidget::tr("More Wind") + "</b>:<br />" + tr("Wind will affect almost everything."));
    glGMLayout->addWidget(TBW_morewind,4,2,1,1);

    TBW_tagteam = new ToggleButtonWidget(gbGameModes, ":/res/btnTagTeam.png");
    TBW_tagteam->setToolTip("<b>" + ToggleButtonWidget::tr("Tag Team") + "</b>:<br />" + tr("Teams in each clan take successive turns sharing their turn time."));
    glGMLayout->addWidget(TBW_tagteam,4,3,1,1);

    // Right
    QLabel * l;

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Damage Modifier"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,0,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconDamage.png"));
    glBSLayout->addWidget(l,0,1,1,1);
    SB_DamageModifier = new QSpinBox(gbBasicSettings);
    SB_DamageModifier->setRange(10, 300);
    SB_DamageModifier->setValue(100);
    SB_DamageModifier->setSingleStep(25);
    glBSLayout->addWidget(SB_DamageModifier,0,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Turn Time"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,1,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconTime.png"));
    glBSLayout->addWidget(l,1,1,1,1);
    SB_TurnTime = new QSpinBox(gbBasicSettings);
    SB_TurnTime->setRange(1, 9999);
    SB_TurnTime->setValue(45);
    SB_TurnTime->setSingleStep(15);
    glBSLayout->addWidget(SB_TurnTime,1,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Initial Health"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,2,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconHealth.png"));
    glBSLayout->addWidget(l,2,1,1,1);
    SB_InitHealth = new QSpinBox(gbBasicSettings);
    SB_InitHealth->setRange(50, 200);
    SB_InitHealth->setValue(100);
    SB_InitHealth->setSingleStep(25);
    glBSLayout->addWidget(SB_InitHealth,2,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Sudden Death Timeout"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,3,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconSuddenDeath.png"));
    glBSLayout->addWidget(l,3,1,1,1);
    SB_SuddenDeath = new QSpinBox(gbBasicSettings);
    SB_SuddenDeath->setRange(0, 50);
    SB_SuddenDeath->setValue(15);
    SB_SuddenDeath->setSingleStep(3);
    glBSLayout->addWidget(SB_SuddenDeath,3,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Sudden Death Water Rise"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,4,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconSuddenDeath.png")); // TODO: icon
    glBSLayout->addWidget(l,4,1,1,1);
    SB_WaterRise = new QSpinBox(gbBasicSettings);
    SB_WaterRise->setRange(0, 100);
    SB_WaterRise->setValue(47);
    SB_WaterRise->setSingleStep(5);
    glBSLayout->addWidget(SB_WaterRise,4,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Sudden Death Health Decrease"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,5,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconSuddenDeath.png")); // TODO: icon
    glBSLayout->addWidget(l,5,1,1,1);
    SB_HealthDecrease = new QSpinBox(gbBasicSettings);
    SB_HealthDecrease->setRange(0, 100);
    SB_HealthDecrease->setValue(5);
    SB_HealthDecrease->setSingleStep(1);
    glBSLayout->addWidget(SB_HealthDecrease,5,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("% Rope Length"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,6,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconRope.png"));
    glBSLayout->addWidget(l,6,1,1,1);
    SB_RopeModifier = new QSpinBox(gbBasicSettings);
    SB_RopeModifier->setRange(25, 999);
    SB_RopeModifier->setValue(100);
    SB_RopeModifier->setSingleStep(25);
    glBSLayout->addWidget(SB_RopeModifier,6,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Crate Drops"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,7,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconBox.png"));
    glBSLayout->addWidget(l,7,1,1,1);
    SB_CaseProb = new FreqSpinBox(gbBasicSettings);
    SB_CaseProb->setRange(0, 9);
    SB_CaseProb->setValue(5);
    glBSLayout->addWidget(SB_CaseProb,7,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("% Health Crates"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,8,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconHealth.png")); // TODO: icon
    glBSLayout->addWidget(l,8,1,1,1);
    SB_HealthCrates = new QSpinBox(gbBasicSettings);
    SB_HealthCrates->setRange(0, 100);
    SB_HealthCrates->setValue(35);
    SB_HealthCrates->setSingleStep(5);
    glBSLayout->addWidget(SB_HealthCrates,8,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Health in Crates"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,9,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconHealth.png")); // TODO: icon
    glBSLayout->addWidget(l,9,1,1,1);
    SB_CrateHealth = new QSpinBox(gbBasicSettings);
    SB_CrateHealth->setRange(0, 200);
    SB_CrateHealth->setValue(25);
    SB_CrateHealth->setSingleStep(5);
    glBSLayout->addWidget(SB_CrateHealth,9,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Mines Time"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,10,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconTime.png")); // TODO: icon
    glBSLayout->addWidget(l,10,1,1,1);
    SB_MinesTime = new QSpinBox(gbBasicSettings);
    SB_MinesTime->setRange(-1, 5);
    SB_MinesTime->setValue(3);
    SB_MinesTime->setSingleStep(1);
    SB_MinesTime->setSpecialValueText(tr("Random"));
    SB_MinesTime->setSuffix(" "+ tr("Seconds"));
    glBSLayout->addWidget(SB_MinesTime,10,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Mines"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,11,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconMine.png")); // TODO: icon
    glBSLayout->addWidget(l,11,1,1,1);
    SB_Mines = new QSpinBox(gbBasicSettings);
    SB_Mines->setRange(0, 80);
    SB_Mines->setValue(0);
    SB_Mines->setSingleStep(5);
    glBSLayout->addWidget(SB_Mines,11,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("% Dud Mines"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,12,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconDud.png"));
    glBSLayout->addWidget(l,12,1,1,1);
    SB_MineDuds = new QSpinBox(gbBasicSettings);
    SB_MineDuds->setRange(0, 100);
    SB_MineDuds->setValue(0);
    SB_MineDuds->setSingleStep(5);
    glBSLayout->addWidget(SB_MineDuds,12,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Explosives"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,13,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconDamage.png"));
    glBSLayout->addWidget(l,13,1,1,1);
    SB_Explosives = new QSpinBox(gbBasicSettings);
    SB_Explosives->setRange(0, 40);
    SB_Explosives->setValue(0);
    SB_Explosives->setSingleStep(1);
    glBSLayout->addWidget(SB_Explosives,13,2,1,1);

    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("% Get Away Time"));
    l->setWordWrap(true);
    glBSLayout->addWidget(l,14,0,1,1);
    l = new QLabel(gbBasicSettings);
    l->setFixedSize(32,32);
    l->setPixmap(QPixmap(":/res/iconTime.png"));
    glBSLayout->addWidget(l,14,1,1,1);
    SB_GetAwayTime = new QSpinBox(gbBasicSettings);
    SB_GetAwayTime->setRange(0, 999);
    SB_GetAwayTime->setValue(100);
    SB_GetAwayTime->setSingleStep(25);
    glBSLayout->addWidget(SB_GetAwayTime,14,2,1,1);
 
    l = new QLabel(gbBasicSettings);
    l->setText(QLabel::tr("Scheme Name:"));

    LE_name = new QLineEdit(this);

    gl->addWidget(LE_name,15,1,1,5);
    gl->addWidget(l,15,0,1,1);

    mapper = new QDataWidgetMapper(this);

    BtnBack = addButton(":/res/Exit.png", pageLayout, 16, 0, true);
    BtnCopy = addButton(tr("Copy"), pageLayout, 16, 2);
    BtnNew = addButton(tr("New"), pageLayout, 16, 3);
    BtnDelete = addButton(tr("Delete"), pageLayout, 16, 4);

    selectScheme = new QComboBox(this);
    pageLayout->addWidget(selectScheme, 16, 1);

    connect(BtnCopy, SIGNAL(clicked()), this, SLOT(copyRow()));
    connect(BtnNew, SIGNAL(clicked()), this, SLOT(newRow()));
    connect(BtnDelete, SIGNAL(clicked()), this, SLOT(deleteRow()));
    connect(selectScheme, SIGNAL(currentIndexChanged(int)), mapper, SLOT(setCurrentIndex(int)));
    connect(selectScheme, SIGNAL(currentIndexChanged(int)), this, SLOT(schemeSelected(int)));
}

void PageScheme::setModel(QAbstractItemModel * model)
{
    mapper->setModel(model);
    selectScheme->setModel(model);

    mapper->addMapping(LE_name, 0);
    mapper->addMapping(TBW_mode_Forts, 1);
    mapper->addMapping(TBW_teamsDivide, 2);
    mapper->addMapping(TBW_solid, 3);
    mapper->addMapping(TBW_border, 4);
    mapper->addMapping(TBW_lowGravity, 5);
    mapper->addMapping(TBW_laserSight, 6);
    mapper->addMapping(TBW_invulnerable, 7);
    mapper->addMapping(TBW_resethealth, 8);
    mapper->addMapping(TBW_vampiric, 9);
    mapper->addMapping(TBW_karma, 10);
    mapper->addMapping(TBW_artillery, 11);
    mapper->addMapping(TBW_randomorder, 12);
    mapper->addMapping(TBW_king, 13);
    mapper->addMapping(TBW_placehog, 14);
    mapper->addMapping(TBW_sharedammo, 15);
    mapper->addMapping(TBW_disablegirders, 16);
    mapper->addMapping(TBW_disablelandobjects, 17);
    mapper->addMapping(TBW_aisurvival, 18);
    mapper->addMapping(TBW_infattack, 19);
    mapper->addMapping(TBW_resetweps, 20);
    mapper->addMapping(TBW_perhogammo, 21);
    mapper->addMapping(TBW_nowind, 22);
    mapper->addMapping(TBW_morewind, 23);
    mapper->addMapping(TBW_tagteam, 24);
    mapper->addMapping(SB_DamageModifier, 25);
    mapper->addMapping(SB_TurnTime, 26);
    mapper->addMapping(SB_InitHealth, 27);
    mapper->addMapping(SB_SuddenDeath, 28);
    mapper->addMapping(SB_CaseProb, 29);
    mapper->addMapping(SB_MinesTime, 30);
    mapper->addMapping(SB_Mines, 31);
    mapper->addMapping(SB_MineDuds, 32);
    mapper->addMapping(SB_Explosives, 33);
    mapper->addMapping(SB_HealthCrates, 34);
    mapper->addMapping(SB_CrateHealth, 35);
    mapper->addMapping(SB_WaterRise, 36);
    mapper->addMapping(SB_HealthDecrease, 37);
    mapper->addMapping(SB_RopeModifier, 38);
		mapper->addMapping(SB_GetAwayTime, 39);

    mapper->toFirst();
}

void PageScheme::newRow()
{
    QAbstractItemModel * model = mapper->model();
    model->insertRow(-1);
    selectScheme->setCurrentIndex(model->rowCount() - 1);
}

void PageScheme::copyRow()
{
    QAbstractItemModel * model = mapper->model();
    model->insertRow(selectScheme->currentIndex());
    selectScheme->setCurrentIndex(model->rowCount() - 1);
}

void PageScheme::deleteRow()
{
    QMessageBox reallyDelete(QMessageBox::Question, QMessageBox::tr("Schemes"), QMessageBox::tr("Really delete this game scheme?"), QMessageBox::Ok | QMessageBox::Cancel);

    if (reallyDelete.exec() == QMessageBox::Ok) {
        QAbstractItemModel * model = mapper->model();
        model->removeRow(selectScheme->currentIndex());
    }
}

void PageScheme::schemeSelected(int n)
{
    int c = ((AmmoSchemeModel*)mapper->model())->numberOfDefaultSchemes;
    gbGameModes->setEnabled(n >= c);
    gbBasicSettings->setEnabled(n >= c);
    LE_name->setEnabled(n >= c);
}
