/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_MENU_H
#define PSXF_GUARD_MENU_H

#include "stage.h"

//Menu enums
typedef enum
{
    MenuPage_Opening,
    MenuPage_Title,
    MenuPage_Main,
    MenuPage_Story,
    MenuPage_Freeplay,
    MenuPage_Mods,
    MenuPage_Awards,
    MenuPage_Credits,
    MenuPage_Options,
    MenuPage_Debug,
    
    #ifdef PSXF_NETWORK
        MenuPage_NetJoin,
        MenuPage_NetHost,
        MenuPage_NetConnect,
        MenuPage_NetFail,
        MenuPage_NetOpWait,
        MenuPage_NetOp,
        MenuPage_NetLobby,
        MenuPage_NetInitFail,
    #endif
    
    MenuPage_Stage, //Changes game loop
} MenuPage;

//Menu functions
void Menu_Load(MenuPage page);
void Menu_Unload();
void Menu_ToStage(StageId id, StageDiff diff, boolean story);
void Menu_Tick();

extern const Achievement achievements[NUM_ACHIEVEMENTS];

void Achievement_Init(void);
Achievement Get_Achievement(Achievements i);
boolean Achievement_IsUnlocked(Achievements i);
boolean Achievement_IsLocked(Achievements i);
void Achievement_Unlock(Achievements i);
void Achievement_Lock(Achievements i);


#endif
