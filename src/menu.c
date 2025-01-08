/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "menu.h"

#include "mem.h"
#include "main.h"
#include "timer.h"
#include "io.h"
#include "gfx.h"
#include "audio.h"
#include "pad.h"
#include "archive.h"
#include "mutil.h"
#include "network.h"
#include "random.h"

#include "font.h"
#include "trans.h"
#include "loadscr.h"

#include "stage.h"
#include "save.h"

#include "stdlib.h"

#include "character/gf.h"
#include "character/mbf.h"
#include "character/mgf.h"
#include "character/djbf.h"
boolean up;
boolean down;
boolean nomem;
boolean switchscreen;

const Achievement achievements[NUM_ACHIEVEMENTS] = {
    {"Full Combo'd Week 1", 36},
    {"Full Combo'd Week 2", 37},
    {"Full Combo'd Week 3", 38},
    {"Full Combo'd Week 4", 39},
    {"Full Combo'd Week 5", 40},
    {"Full Combo'd Week 6", 41},
    {"Full Combo'd Week 7", 42},
    {"Full Combo'd Senbonzakura", 43},
    {"Perfect'd A Song In Nightmare Difficulty", 44},
    {"Using A Shitty Emu, Get Lost!!", 45},
    {"Beaten A Song With No Hud", 46},
    {"Played Swapped Mode", 47},
    {"Played With A Friend", 48},
    {"Debugger Entered The Debug Menu", 49}
};

void Achievement_Init(void) {

//  for (int i = 0; i < NUM_ACHIEVEMENTS; i++)
//      stage.prefs.unlockedAchievements[i] = false;
    //load from save file here instead (too lazy to put it in) //(handled in save.c)
}

Achievement Get_Achievement(Achievements i) {
    return achievements[i];
}

boolean Achievement_IsUnlocked(Achievements i) {
    return stage.prefs.unlockedAchievements[i];
}

boolean Achievement_IsLocked(Achievements i) {
    return !stage.prefs.unlockedAchievements[i];
}

void Achievement_Unlock(Achievements i) {
    stage.prefs.unlockedAchievements[i] = true;
}

void Achievement_Lock(Achievements i) {
    stage.prefs.unlockedAchievements[i] = false;
}

//Menu messages
static const char *funny_messages[][2] = {
    {"SHIT CODE WHO CARES", "SPICYJPEG"},
    {"RIP SHRIVELED GARFIELD", "ON PSXFUNKIN SERVER"},
    {"FUNKIN", "DEAD"},
    {"WHY ARE YOU HERE", "FNF DIED"},
    {"BACKEND GO BRR", "SECURITY ISSUES"},
    {"PSXFUNKIN", "STILL LIVING"},
    {"SONY WILL COME", "PSYQ SDK"},
    {"BETTER CALL SAUL", "SONY IS COMING"},
    {"BIG FLOPPA", "bingus"},
    {"NOT THREE DS FUNKIN", "ITS PSXFUNKIN"},
    {"ACHIEVEMENTS", "NOW THERES A GOAL"},
    {"WAKEY", "WAKEY"},
    {"CONTROLLER PLAYERS", "NEEDED"},
    {"RIP KEYBOARD PLAYERS", "HAHAHA"},
    {"PIRACY", "IS A CRIME"},
    {"SYSCLK", "RANDOM SEED"},
    {"WHERES THE MONEY", "KICKSTARTER"},
    {"FCEFUWEFUETWHCFUEZDSLVNSP", "PQRYQWENQWKBVZLZSLDNSVPBM"},
    {"SECRET SONGS", "WHERE"},
    {"PSXFUNKIN BY CUCKYDEV", "SUCK IT DOWN"},
    {"PLAYING ON EPSXE HUH", "YOURE THE PROBLEM"},
    {"NEXT IN LINE", "ATARI"},
    {"THREE DS", "ITS WORSE"},
    {"HAHAHA", "I DONT CARE"},
    {"GET ME TO STOP", "TRY"},
    {"FNF MUKBANG GIF", "THATS UNRULY"},
    {"OPEN SOURCE", "FOREVER"},
    {"ITS A PORT", "ITS BETTER"},
    {"WOW GATO", "WOW GATO"},
    {"UNBAN", "SHRIVELED GARFIELD"},
};

#ifdef PSXF_NETWORK

//Menu string type
#define MENUSTR_CHARS 0x20
typedef char MenuStr[MENUSTR_CHARS + 1];

#endif

//Menu state
static struct
{
    //Menu state
    u8 page, next_page;
    boolean page_swap;
    u8 select, next_select;

    fixed_t scroll;
    fixed_t trans_time;

    //Page specific state
    union
    {
        struct
        {
            u8 funny_message;
        } opening;
        struct
        {
            fixed_t logo_bump;
            fixed_t fade, fadespd;
        } title;
        struct
        {
            fixed_t fade, fadespd;
        } story;
        struct
        {
            fixed_t back_r, back_g, back_b;
        } freeplay;
    #ifdef PSXF_NETWORK
        struct
        {
            boolean type;
            MenuStr port;
            MenuStr pass;
        } net_host;
        struct
        {
            boolean type;
            MenuStr ip;
            MenuStr port;
            MenuStr pass;
        } net_join;
        struct
        {
            boolean swap;
        } net_op;
    #endif
    } page_state;

    union
    {
        struct
        {
            u8 id, diff;
            int setselect;
            boolean story;
        } stage;
    } page_param;

    //Menu assets
    Gfx_Tex tex_back, tex_ng, tex_story, tex_title, tex_options, tex_week;
    IO_Data weeks, weeks_ptrs[128];
    int curweek;

    FontData font_bold, font_arial, font_kata;

    Character *gf; //Title Girlfriend
    Character *mbf; //Menu Bf
    Character *mgf; //Menu Gf
    Character *djbf; //Freeplay bf / Dj bf
} menu;

/*static void ButtonStr buttons[] = {
    {PAD_UP,{157, 0, 14, 14}}, //PAD_UP
    {PAD_DOWN,{172, 0, 14, 14}}, //PAD_DOWN
    {PAD_LEFT,{157, 15, 14, 14}}, //PAD_LEFT
    {PAD_RIGHT,{172, 15, 14, 14}}, //PAD_RIGHT

    {PAD_TRIANGLE,{138, 17, 15, 15}}, //PAD_TRIANGLE
    {PAD_CIRCLE,{140, 0, 16, 16}}, //PAD_CIRCLE
    {PAD_CROSS,{123, 0, 16, 16}}, //PAD_CROSS
    {PAD_SQUARE,{123, 17, 14, 14}}, //PAD_SQUARE

    {PAD_L1,{72, 16, 23, 15}}, //PAD_L1
    {PAD_L2,{72, 0, 24, 15}}, //PAD_L2
    {PAD_R1,{97, 16, 24, 15}}, //PAD_R1
    {PAD_R2,{97, 0, 25, 15}}, //PAD_R2
};*/

static void CheckAndLoadWeek(int week)
{
    if (menu.curweek != week)
    {
        char weektxt[20];
        sprintf(weektxt, "week%d.tim", week);
        Gfx_LoadTex(&menu.tex_week, Archive_Find(menu.weeks, weektxt), 0);
        menu.curweek = week;
    }

}
#ifdef PSXF_NETWORK

//Menu string functions
static void MenuStr_Process(MenuStr *this, s32 x, s32 y, const char *fmt, boolean select, boolean type)
{
    //Append typed input
    if (select && type)
    {
        if (pad_type[0] != '\0')
            strncat(*this, pad_type, MENUSTR_CHARS - strlen(*this));
        if (pad_backspace)
        {
            size_t i = strlen(*this);
            if (i != 0)
                (*this)[i - 1] = '\0';
        }
    }

    //Get text to draw
    char buf[0x100];
    sprintf(buf, fmt, *this);
    if (select && type && (animf_count & 2))
        strcat(buf, "_");

    //Draw text
    menu.font_arial.draw_col(&menu.font_arial, buf, x, y, FontAlign_Left, 0x80, 0x80, select ? 0x00 : 0x80);
    menu.font_arial.draw_col(&menu.font_arial, buf, x+1, y+1, FontAlign_Left, 0x00, 0x00, 0x00);

    //Draw text
    menu.font_arialr.draw_col(&menu.font_arialr, buf, x, y, FontAlign_Left, 0x80, 0x80, select ? 0x00 : 0x80);
    menu.font_arialr.draw_col(&menu.font_arialr, buf, x+1, y+1, FontAlign_Left, 0x00, 0x00, 0x00);

    //Draw text
    menu.font_arialb.draw_col(&menu.font_arialb, buf, x, y, FontAlign_Left, 0x80, 0x80, select ? 0x00 : 0x80);
    menu.font_arialb.draw_col(&menu.font_arialb, buf, x+1, y+1, FontAlign_Left, 0x00, 0x00, 0x00);
}

#endif


//Internal menu functions
char menu_text_buffer[0x100];

static const char *Menu_LowerIf(const char *text, boolean lower)
{
    //Copy text
    char *dstp = menu_text_buffer;
    if (lower)
    {
        for (const char *srcp = text; *srcp != '\0'; srcp++)
        {
            if (*srcp >= 'A' && *srcp <= 'Z')
                *dstp++ = *srcp | 0x20;
            else
                *dstp++ = *srcp;
        }
    }
    else
    {
        for (const char *srcp = text; *srcp != '\0'; srcp++)
        {
            if (*srcp >= 'a' && *srcp <= 'z')
                *dstp++ = *srcp & ~0x20;
            else
                *dstp++ = *srcp;
        }
    }

    //Terminate text
    *dstp++ = '\0';
    return menu_text_buffer;
}

static void Menu_DrawBack(boolean flash, s32 scroll, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1)
{
    RECT back_src = {0, 0, 255, 255};
    RECT back_dst = {0, -scroll - SCREEN_WIDEADD2, SCREEN_WIDTH, SCREEN_WIDTH * 4 / 5};

    if (flash || (animf_count & 4) == 0)
        Gfx_DrawTexCol(&menu.tex_back, &back_src, &back_dst, r0, g0, b0);
    else
        Gfx_DrawTexCol(&menu.tex_back, &back_src, &back_dst, r1, g1, b1);
}
static void Menu_DifficultySelector(s32 x, s32 y)
{
    //Change difficulty
    if (menu.next_page == menu.page && Trans_Idle())
    {
        if (pad_state.press & PAD_LEFT)
        {
            if (menu.page_param.stage.diff > StageDiff_Easy)
                menu.page_param.stage.diff--;
            else
                menu.page_param.stage.diff = StageDiff_Hard;
        }
        if (pad_state.press & PAD_RIGHT)
        {
            if (menu.page_param.stage.diff < StageDiff_Hard)
                menu.page_param.stage.diff++;
            else
                menu.page_param.stage.diff = StageDiff_Easy;
        }
    }

    //Draw difficulty arrows
    static const RECT arrow_src[2][2] = {
        {{224, 64, 16, 32}, {224, 96, 16, 32}}, //left
        {{240, 64, 16, 32}, {240, 96, 16, 32}}, //right
    };

    Gfx_BlitTex(&menu.tex_story, &arrow_src[0][(pad_state.held & PAD_LEFT) != 0], x - 40 - 16, y - 16);
    Gfx_BlitTex(&menu.tex_story, &arrow_src[1][(pad_state.held & PAD_RIGHT) != 0], x + 40, y - 16);

    //Draw difficulty
    static const RECT diff_srcs[] = {
        {  0, 96, 64, 18},
        { 64, 96, 80, 18},
        {144, 96, 64, 18},
    };

    const RECT *diff_src = &diff_srcs[menu.page_param.stage.diff];
    Gfx_BlitTex(&menu.tex_story, diff_src, x - (diff_src->w >> 1), y - 9 + ((pad_state.press & (PAD_LEFT | PAD_RIGHT)) != 0));
}

static void Menu_Options(s32 x, s32 y)
{
    //Change difficulty
    if (menu.next_page == menu.page && Trans_Idle())
    {
        if (pad_state.press & PAD_L1)
        {
            if (menu.page_param.stage.setselect >0)
            {
                menu.page_param.stage.setselect--;
                menu.select = 0;
            }
            else
            {
                menu.page_param.stage.setselect = 1;
                menu.select = 0;
            }
        }
        if (pad_state.press & PAD_R1)
        {
            if (menu.page_param.stage.setselect <1)
            {
                menu.page_param.stage.setselect++;
                menu.select = 0;
            }
            else
            {   menu.page_param.stage.setselect = 0;
                menu.select = 0;
            }
        }
    }
    //Draw difficulty arrows
    static const RECT arrow_src[2][2] = {
        {{224, 64, 16, 32}, {224, 96, 16, 32}}, //left
        {{240, 64, 16, 32}, {240, 96, 16, 32}}, //right
    };

    Gfx_BlitTex(&menu.tex_options, &arrow_src[0][(pad_state.held & PAD_L1) != 0], x - 40 - 16, y - 16);
    Gfx_BlitTex(&menu.tex_options, &arrow_src[1][(pad_state.held & PAD_R1) != 0], x + 40, y - 16);

    //Draw difficulty
    static const RECT sets_srcs[] = {
        {  0, 96, 72, 18},
        { 72, 96, 72, 18},
        {144, 96, 67, 18},
    };

    const RECT *sets_src = &sets_srcs[menu.page_param.stage.setselect];
    Gfx_BlitTex(&menu.tex_options, sets_src, x - (sets_src->w >> 1), y - 9 + ((pad_state.press & (PAD_L1 | PAD_R1)) != 0));
}

static void Menu_DrawWeek(const char *week, s32 x, s32 y)
{
    //Draw label
    if (week == NULL)
    {
        //Tutorial
        RECT label_src = {0, 0, 112, 32};
        Gfx_BlitTex(&menu.tex_story, &label_src, x, y);
    }
    else
    {
        //Week
        RECT label_src = {0, 32, 80, 32};
        Gfx_BlitTex(&menu.tex_story, &label_src, x+24, y);

        //Number
        for (; *week != '\0'; week++)
        {
            //Draw number
            u8 i = *week - '0';

            RECT num_src = {128 + ((i & 3) << 5), ((i >> 2) << 5), 32, 32};
            Gfx_BlitTex(&menu.tex_story, &num_src, x, y);
            x = 80;
        }
    }
}

static void Menu_DrawBG(s32 x, s32 y)
{
    //Draw Track
    RECT week1_src = {0, 0, 256, 136};
    RECT week1_dst = { x, y, 320, 136};
    Gfx_DrawTex(&menu.tex_week, &week1_src, &week1_dst);
}
static void Menu_DrawTrack(s32 x, s32 y)
{
    //Draw Track
    RECT track_src = {0, 64, 80, 16};
    Gfx_BlitTex(&menu.tex_story, &track_src, x, y);
}
static void Menu_DrawSavingicon(s32 x, s32 y)
{
    //Draw Track
    RECT save_src = {192, 224, 32, 32};
    Gfx_BlitTex(&stage.tex_hud1, &save_src, x, y);
}
static void Menu_DrawBigCredits(u8 i, s16 x, s16 y)
{
    //Icon Size
    u8 icon_size = 64;

    //Get src and dst
    RECT src = {
        (i % 4) * icon_size,
        (i / 4) * icon_size,
        icon_size,
        icon_size
    };
    RECT dst = {
        x,
        y,
        64,
        64
    };

    //Draw health icon
    Gfx_DrawTex(&stage.tex_hud2, &src, &dst);
}
static void Menu_DrawOptions(u8 i, s16 x, s16 y, boolean is_bool)
{
    //Icon Size
    u8 iconx_size = (is_bool) ? 32 : 64;
    u8 icony_size = 32;

    //Get src and dst
    RECT src = {
        (i % 8) * iconx_size,
        (i / 8) * icony_size,
        iconx_size,
        icony_size
    };
    RECT dst = {
        x,
        y,
        iconx_size,
        32
    };

    //Draw health icon
    Gfx_DrawTex(&menu.tex_options, &src, &dst);
}
static void Menu_DrawHealth(u8 i, s16 x, s16 y, boolean is_selected)
{
    //Icon Size
    u8 icon_size = 32;

    u8 col = (is_selected) ? 128 : 64;

    //Get src and dst
    RECT src = {
        (i % 7) * icon_size,
        (i / 7) * icon_size,
        icon_size,
        icon_size
    };
    RECT dst = {
        x,
        y,
        32,
        32
    };

    //Draw health icon
    Gfx_DrawTexCol(&stage.tex_hud1, &src, &dst, col, col, col);
}

//Menu functions
void Menu_Load(MenuPage page)
{
    switchscreen = false;
    stage.cutdia=0;
    stage.paused = false;
    if (stage.loadsaveonce == false)
    {
        readSaveFile();
        stage.loadsaveonce = true;
    }
    //Load menu assets
    IO_Data menu_arc = IO_Read("\\MENU\\MENU.ARC;1");
    Gfx_LoadTex(&menu.tex_back,  Archive_Find(menu_arc, "back.tim"),  0);
    Gfx_LoadTex(&menu.tex_ng,    Archive_Find(menu_arc, "ng.tim"),    0);
    Gfx_LoadTex(&menu.tex_story, Archive_Find(menu_arc, "story.tim"), 0);
    Gfx_LoadTex(&menu.tex_options, Archive_Find(menu_arc, "options.tim"), 0);
    Gfx_LoadTex(&menu.tex_title, Archive_Find(menu_arc, "title.tim"), 0);
    Gfx_LoadTex(&menu.tex_options, Archive_Find(menu_arc, "options.tim"), 0);
    Gfx_LoadTex(&stage.tex_hud1, Archive_Find(menu_arc, "hud1.tim"), 0);
    Gfx_LoadTex(&stage.tex_hud2, Archive_Find(menu_arc, "hud2.tim"), 0);
    Mem_Free(menu_arc);

    menu.weeks = IO_Read("\\MENU\\WEEK.ARC;1");
    Gfx_LoadTex(&menu.tex_week, Archive_Find(menu.weeks, "week0.tim"), 0);

    FontData_Load(&menu.font_bold, Font_Bold);
    FontData_Load(&menu.font_arial, Font_Arial);
    FontData_Load(&menu.font_kata, Font_KATA);

    menu.gf = Char_GF_New(FIXED_DEC(62,1), FIXED_DEC(-12,1));
    menu.mbf = Char_Mbf_New(FIXED_DEC(11,1), FIXED_DEC(40,1));
    menu.mgf = Char_Mgf_New(FIXED_DEC(91,1), FIXED_DEC(13,1));
    menu.djbf = Char_Djbf_New(FIXED_DEC(102,1), FIXED_DEC(76,1));

    stage.camera.x = stage.camera.y = FIXED_DEC(0,1);
    stage.camera.bzoom = FIXED_UNIT;
    stage.gf_speed = 4;

    //Initialize menu state
    menu.select = menu.next_select = 0;

    switch (menu.page = menu.next_page = page)
    {
        case MenuPage_Opening:
            //Get funny message to use
            //Do this here so timing is less reliant on VSync
            #ifdef PSXF_PC
                menu.page_state.opening.funny_message = time(NULL) % COUNT_OF(funny_messages);
            #else
                menu.page_state.opening.funny_message = ((*((volatile u32*)0xBF801120)) >> 3) % COUNT_OF(funny_messages); //sysclk seeding
            #endif
            break;
        default:
            break;
    }
    menu.page_swap = true;

    menu.trans_time = 0;
    Trans_Clear();

    stage.song_step = 0;

    /*//Play menu music
        Audio_PlayXA_Track(XA_GettinFreaky, 0x40, 0, 1);
        Audio_WaitPlayXA();*/
    if (stage.prefs.menumusic == 1)
    {
        //Play menu music
        Audio_PlayXA_Track(XA_Freeky, 0x40, 2, 1);
        Audio_WaitPlayXA();
    }
    else if (stage.prefs.menumusic ==2)
    {
        //Play menu music
        Audio_PlayXA_Track(XA_Ludum, 0x40, 3, 1);
        Audio_WaitPlayXA();
    }
    else
    {
        //Play menu music
        Audio_PlayXA_Track(XA_GettinFreaky, 0x40, 0, 1);
        Audio_WaitPlayXA();
    }

    //Set background colour
    Gfx_SetClear(0, 0, 0);
}

void Menu_Unload(void)
{
    //Free title Girlfriend
    Character_Free(menu.gf);
    Character_Free(menu.mgf);
    Character_Free(menu.mbf);
    Character_Free(menu.djbf);
    Mem_Free(menu.weeks);
}

void Menu_ToStage(StageId id, StageDiff diff, boolean story)
{
    menu.next_page = MenuPage_Stage;
    menu.page_param.stage.id = id;
    menu.page_param.stage.story = story;
    menu.page_param.stage.diff = diff;
    Trans_Start();
}

void Menu_Tick(void)
{
    //Clear per-frame flags
    stage.flag &= ~STAGE_FLAG_JUST_STEP;

    //Get song position
    u16 next_step = Audio_TellXA_Milli() / 147; //100 BPM
    if (next_step != stage.song_step)
    {
        if (next_step >= stage.song_step)
            stage.flag |= STAGE_FLAG_JUST_STEP;
        stage.song_step = next_step;
    }

    //Handle transition out
    if (Trans_Tick())
    {
        //Change to set next page
        menu.page_swap = true;
        menu.page = menu.next_page;
        menu.select = menu.next_select;
    }

    //Tick menu page
    MenuPage exec_page;
    switch (exec_page = menu.page)
    {
        case MenuPage_Opening:
        {
            u16 beat = stage.song_step >> 2;

            //Start title screen if opening ended
            if (beat >= 16)
            {
                menu.page = menu.next_page = MenuPage_Title;
                menu.page_swap = true;
                //Fallthrough
            }
            else
            {
                //Start title screen if start pressed
                if (pad_state.held & PAD_START)
                    menu.page = menu.next_page = MenuPage_Title;

                //Draw different text depending on beat
                RECT src_ng = {0, 0, 128, 128};
                const char **funny_message = funny_messages[menu.page_state.opening.funny_message];

                switch (beat)
                {
                    case 3:
                        menu.font_bold.draw(&menu.font_bold, "NINTENDO BRO", SCREEN_WIDTH2, SCREEN_HEIGHT2 - 40, FontAlign_Center);
                        menu.font_bold.draw(&menu.font_bold, "CUCKYDEV",      SCREEN_WIDTH2, SCREEN_HEIGHT2  +24, FontAlign_Center);
                        Menu_DrawHealth(21, 240, SCREEN_HEIGHT2 - 42, true);
                        Menu_DrawHealth(26, 215, SCREEN_HEIGHT2 + 16, true);
                //Fallthrough
                    case 2:
                    case 1:
                        menu.font_bold.draw(&menu.font_bold, "FLOP ENGINE BY", SCREEN_WIDTH2, SCREEN_HEIGHT2 - 64, FontAlign_Center);
                        menu.font_bold.draw(&menu.font_bold, "PSX FUNKIN BY", SCREEN_WIDTH2, SCREEN_HEIGHT2, FontAlign_Center);
                        break;
                    case 7:
                        menu.font_bold.draw(&menu.font_bold, "NEWGROUNDS",    SCREEN_WIDTH2, SCREEN_HEIGHT2 - 32, FontAlign_Center);
                        Gfx_BlitTex(&menu.tex_ng, &src_ng, (SCREEN_WIDTH - 128) >> 1, SCREEN_HEIGHT2 - 16);
                //Fallthrough
                    case 6:
                    case 5:
                        menu.font_bold.draw(&menu.font_bold, "ASSOCIATED", SCREEN_WIDTH2, SCREEN_HEIGHT2 - 64, FontAlign_Center);
                        menu.font_bold.draw(&menu.font_bold, "WITH",           SCREEN_WIDTH2, SCREEN_HEIGHT2 - 48, FontAlign_Center);
                        break;

                    case 11:
                        menu.font_bold.draw(&menu.font_bold, funny_message[1], SCREEN_WIDTH2, SCREEN_HEIGHT2, FontAlign_Center);
                //Fallthrough
                    case 10:
                    case 9:
                        menu.font_bold.draw(&menu.font_bold, funny_message[0], SCREEN_WIDTH2, SCREEN_HEIGHT2 - 16, FontAlign_Center);
                        break;

                    case 15:
                        menu.font_bold.draw(&menu.font_bold, "FUNKIN", SCREEN_WIDTH2, SCREEN_HEIGHT2 + 8, FontAlign_Center);
                //Fallthrough
                    case 14:
                        menu.font_bold.draw(&menu.font_bold, "NIGHT", SCREEN_WIDTH2, SCREEN_HEIGHT2 - 8, FontAlign_Center);
                //Fallthrough
                    case 13:
                        menu.font_bold.draw(&menu.font_bold, "FRIDAY", SCREEN_WIDTH2, SCREEN_HEIGHT2 - 24, FontAlign_Center);
                        break;
                }
                break;
            }
        }
    //Fallthrough
        case MenuPage_Title:
        {
            //Initialize page
            if (menu.page_swap)
            {
                menu.page_state.title.logo_bump = (FIXED_DEC(7,1) / 24) - 1;
                menu.page_state.title.fade = FIXED_DEC(255,1);
                menu.page_state.title.fadespd = FIXED_DEC(90,1);
            }

            //Draw white fade
            if (menu.page_state.title.fade > 0)
            {
                if(stage.prefs.flashing ==0)
                {
                    static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                    u8 flash_col = menu.page_state.title.fade >> FIXED_SHIFT;
                    Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 1);
                    menu.page_state.title.fade -= FIXED_MUL(menu.page_state.title.fadespd, timer_dt);
                }
            }

            //Go to main menu when start is pressed
            if (menu.trans_time > 0 && (menu.trans_time -= timer_dt) <= 0)
                Trans_Start();

            if ((pad_state.press & PAD_START) && menu.next_page == menu.page && Trans_Idle())
            {
                menu.trans_time = FIXED_UNIT;
                menu.page_state.title.fade = FIXED_DEC(255,1);
                menu.page_state.title.fadespd = FIXED_DEC(300,1);
                menu.next_page = MenuPage_Main;
                menu.next_select = 0;
            }

            //Draw Friday Night Funkin' logo
            if ((stage.flag & STAGE_FLAG_JUST_STEP) && (stage.song_step & 0x3) == 0 && menu.page_state.title.logo_bump == 0)
                menu.page_state.title.logo_bump = (FIXED_DEC(7,1) / 24) - 1;

            static const fixed_t logo_scales[] = {
                FIXED_DEC(1,1),
                FIXED_DEC(101,100),
                FIXED_DEC(102,100),
                FIXED_DEC(103,100),
                FIXED_DEC(105,100),
                FIXED_DEC(110,100),
                FIXED_DEC(97,100),
            };
            fixed_t logo_scale = logo_scales[(menu.page_state.title.logo_bump * 24) >> FIXED_SHIFT];
            u32 x_rad = (logo_scale * (176 >> 1)) >> FIXED_SHIFT;
            u32 y_rad = (logo_scale * (112 >> 1)) >> FIXED_SHIFT;

            RECT logo_src = {0, 0, 176, 112};
            RECT logo_dst = {
                100 - x_rad + (SCREEN_WIDEADD2 >> 1),
                68 - y_rad,
                x_rad << 1,
                y_rad << 1
            };
            Gfx_DrawTex(&menu.tex_title, &logo_src, &logo_dst);

            if (menu.page_state.title.logo_bump > 0)
                if ((menu.page_state.title.logo_bump -= timer_dt) < 0)
                    menu.page_state.title.logo_bump = 0;

            //Draw "Press Start to Begin"
            if (menu.next_page == menu.page)
            {
                //Blinking blue
                s16 press_lerp = (MUtil_Cos(animf_count << 3) + 0x100) >> 1;
                u8 press_r = 51 >> 1;
                u8 press_g = (58  + ((press_lerp * (255 - 58))  >> 8)) >> 1;
                u8 press_b = (206 + ((press_lerp * (255 - 206)) >> 8)) >> 1;

                RECT press_src = {0, 112, 256, 32};
                Gfx_BlitTexCol(&menu.tex_title, &press_src, (SCREEN_WIDTH - 256) / 2, SCREEN_HEIGHT - 48, press_r, press_g, press_b);
            }
            else
            {
                //Flash white
                RECT press_src = {0, (animf_count & 1) ? 144 : 112, 256, 32};
                Gfx_BlitTex(&menu.tex_title, &press_src, (SCREEN_WIDTH - 256) / 2, SCREEN_HEIGHT - 48);
            }

            //Draw Girlfriend
            menu.gf->tick(menu.gf);
            break;
        }
        case MenuPage_Debug:
        {
                static const struct
                {
                    const char *text;
                } menu_options[] = {
                    {"BIG FLOPPA"},
                    {"UNLOCK ACHIEVEMENTS"},
                    {"DISABLE HUD"},
                };
                Achievement_Unlock(Debugger_Enter_The_Debug_Menu);

                //Initialize page
                if (menu.page_swap)
                {
                    menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                }

                //Handle option and selection
                if (menu.next_page == menu.page && Trans_Idle())
                {
                    //Change option
                    if (pad_state.press & PAD_UP)
                    {
                        if (menu.select > 0)
                            menu.select--;
                        else
                            menu.select = COUNT_OF(menu_options) - 1;
                    }
                    if (pad_state.press & PAD_DOWN)
                    {
                        if (menu.select < COUNT_OF(menu_options) - 1)
                            menu.select++;
                        else
                            menu.select = 0;
                    }
	            menu.font_arial.draw(&menu.font_arial,
			"Press X to enable and Triangle to disable",
			16,
			217,
		        FontAlign_Left
	           );
		   //Select option if cross is pressed
	           if (pad_state.press & (PAD_START | PAD_CROSS))
	           {
	                switch (menu.select)
	                {
	                	case 0:
	                		break;
	                	case 1:
	                		for (int i = 0; i < NUM_ACHIEVEMENTS; i++)
	                			stage.prefs.unlockedAchievements[i] = true;
	                		break;
	                	case 2:
	                		stage.nohud = true;
	                		break;
	                }
	            }
                    //Return to main menu if circle is pressed
                    if (pad_state.press & PAD_CIRCLE)
                    {
                        menu.next_page = MenuPage_Main;
                        menu.next_select = 0;
                        menu.select = 0;
                        Trans_Start();
                    }
                }

                //Draw options
                s32 next_scroll = menu.select * FIXED_DEC(40,1);
                menu.scroll += (next_scroll - menu.scroll) >> 4;

                for (u8 i = 0; i < COUNT_OF(menu_options); i++)
                {
                    //Get position on screen
                    s32 y = (i * 40) - 8 - (menu.scroll >> FIXED_SHIFT);
                    if (y <= -SCREEN_HEIGHT2 - 8)
                        continue;
                    if (y >= SCREEN_HEIGHT2 + 8)
                        break;

                    //Draw text
                    menu.font_bold.draw_col(&menu.font_bold,
                        Menu_LowerIf(menu_options[i].text, NULL),
                        30 + (y >> 2),
                        SCREEN_HEIGHT2 + y - 8,
                        FontAlign_Left,
                        (menu.select == i) ? 128 : 64,
                        (menu.select == i) ? 128 : 64,
                        (menu.select == i) ? 128 : 64
                    );
                }
                //Draw background
                    Menu_DrawBack(
                    true,
                    8,
                    41 >> 1, 41 >> 1, 41 >> 1,
                    0, 0, 0
                    );
                break;
        }
        case MenuPage_Main:
        {
            static const char *menu_options[] = {
                "mt-M-H-(",
                "AM-6Ob",
                "H9!",
                "aQ-(",
                "hOZ9t",
                "e6l2S",
                "n-+",
                #ifdef PSXF_NETWORK
                    "JOIN SERVER",
                    "HOST SERVER",
                #endif
            };

            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = menu.select *
                #ifndef PSXF_NETWORK
                    FIXED_DEC(8,1);
                #else
                    FIXED_DEC(12,1);
                #endif
            }

            //Draw version identification
            menu.font_arial.draw(&menu.font_arial,
                "Flop Engine v1.2",
                8,
                SCREEN_HEIGHT - 32,
                FontAlign_Left
            );
            if(stage.prefs.lowquality==1)
            {
                Achievement_Unlock(ds_User_Turn_On_Low_Quality_Mode);
            }
            if(nomem == true)
            {
            //Draw mind games port version identification
            menu.font_arial.draw(&menu.font_arial,
                "No memorycard inserted",
                8,
                8,
                FontAlign_Left
            );
            }

            //Handle option and selection
            if (menu.trans_time > 0 && (menu.trans_time -= timer_dt) <= 0)
                Trans_Start();

            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }
                if(Achievement_IsUnlocked(Full_Combo_Senbonzakura))
                	Menu_DrawHealth(13, 181, 8, true);

                //Select option if cross is pressed
                if (pad_state.press & (PAD_START | PAD_CIRCLE))
                {
                    switch (menu.select)
                    {
                        case 0: //Story Mode
                            menu.next_page = MenuPage_Story;
                            menu.next_select = 0;
                            break;
                        case 1: //Freeplay
                            menu.next_page = MenuPage_Freeplay;
                            menu.next_select = 0;
                            break;
                        case 2: //Mods
                            menu.next_page = MenuPage_Mods;
                            menu.next_select = 0;
                            break;
                        case 3: //Awards
                            menu.next_page = MenuPage_Awards;
                            menu.next_select = 0;
                            break;
                        case 4: //Credits
                            menu.next_page = MenuPage_Credits;
                            menu.next_select = 0;
                            break;
                        case 5: //Options
                            menu.next_page = MenuPage_Options;
                            menu.next_select = 0;
                            menu.page_param.stage.setselect = 0;
                            break;
                        case 6: //save game
                            Menu_DrawSavingicon( 284, 204);
                            WriteSaveDataStructToBinaryAndSaveItOnTheFuckingMemoryCard();
                        break;
                    #ifdef PSXF_NETWORK
                        case 6: //Join Server
                            menu.next_page = Network_Inited() ? MenuPage_NetJoin : MenuPage_NetInitFail;
                            break;
                        case 7: //Host Server
                            menu.next_page = Network_Inited() ? MenuPage_NetHost : MenuPage_NetInitFail;
                            break;
                    #endif
                    }
                    if (menu.select !=6)
                    {
                        menu.next_select = 0;
                        menu.trans_time = FIXED_UNIT;
                    }
                }

                //Return to title screen if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Title;
                    Trans_Start();
                }
                if (pad_state.press & PAD_TRIANGLE)
                {
                    menu.next_page = MenuPage_Debug;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw options
            s32 next_scroll = menu.select *
            #ifndef PSXF_NETWORK
                FIXED_DEC(8,1);
            #else
                FIXED_DEC(12,1);
            #endif
            menu.scroll += (next_scroll - menu.scroll) >> 1;

            if (menu.next_page == menu.page || (menu.next_page == MenuPage_Title || menu.next_page == MenuPage_Debug))
            {
                //Draw all options
                for (u8 i = 0; i < COUNT_OF(menu_options); i++)
                {
                    //Draw text
                    menu.font_kata.draw_col(&menu.font_kata,
                        menu_options[i],
                        SCREEN_WIDTH2,
                        SCREEN_HEIGHT2 + (i << 5) - 48 - (menu.scroll >> FIXED_SHIFT),
                        FontAlign_Center,
                        (menu.select == i) ? 128 : 64,
                        (menu.select == i) ? 128 : 64,
                        (menu.select == i) ? 128 : 64
                    );
                }
            }
            else if (animf_count & 2)
            {
                //Draw selected option
                menu.font_kata.draw(&menu.font_kata,
                    menu_options[menu.select],
                    SCREEN_WIDTH2,
                    SCREEN_HEIGHT2 + (menu.select << 5) - 48 - (menu.scroll >> FIXED_SHIFT),
                    FontAlign_Center
                );
            }

            //Draw background
            if(stage.prefs.flashing ==0)
            {
            Menu_DrawBack(
                menu.next_page == menu.page || (menu.next_page == MenuPage_Title || menu.next_page == MenuPage_Debug),
            #ifndef PSXF_NETWORK
                menu.scroll >> (FIXED_SHIFT + 1),
            #else
                menu.scroll >> (FIXED_SHIFT + 3),
            #endif
                253 >> 1, 231 >> 1, 113 >> 1,
            253 >> 1, 113 >> 1, 155 >> 1
            );
            }
            else
            {
                Menu_DrawBack(
                menu.next_page == menu.page || (menu.next_page == MenuPage_Title || menu.next_page == MenuPage_Debug),
            #ifndef PSXF_NETWORK
                menu.scroll >> (FIXED_SHIFT + 1),
            #else
                menu.scroll >> (FIXED_SHIFT + 3),
            #endif
                253 >> 1, 231 >> 1, 113 >> 1,
            253 >> 1, 231 >> 1, 113 >> 1
            );
            }
            break;
        }
        case MenuPage_Story:
        {
            static const struct
        {
            const char *week;
            StageId stage;
            const char *name;
            const char *tracks[3];
        } menu_options[] = {
                {NULL, StageId_1_4, "TUTORIAL", {"TUTORIAL", NULL, NULL}},
                {"1", StageId_1_1, "DADDY DEAREST", {"BOPEEBO", "FRESH", "DADBATTLE"}},
                {"2", StageId_2_1, "SPOOKY MONTH", {"SPOOKEEZ", "SOUTH", "MONSTER"}},
                {"3", StageId_3_1, "PICO", {"PICO", "PHILLY NICE", "BLAMMED"}},
                {"4", StageId_4_1, "MOMMY MUST MURDER", {"SATIN PANTIES", "HIGH", "MILF"}},
                {"5", StageId_5_1, "RED SNOW", {"COCOA", "EGGNOG", "WINTER HORRORLAND"}},
                {"6", StageId_6_1, "HATING SIMULATOR", {"SENPAI", "ROSES", "THORNS"}},
                {"7", StageId_7_1, "TANKMAN", {"UGH", "GUNS", "STRESS"}},
        };

            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = 0;
                menu.page_param.stage.diff = StageDiff_Normal;
                menu.page_state.title.fade = FIXED_DEC(0,1);
                menu.page_state.title.fadespd = FIXED_DEC(0,1);
            }

            //Draw white fade
            if (menu.page_state.title.fade > 0)
            {
                if(stage.prefs.flashing==0)
                {
                static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                u8 flash_col = menu.page_state.title.fade >> FIXED_SHIFT;
                Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 1);
                menu.page_state.title.fade -= FIXED_MUL(menu.page_state.title.fadespd, timer_dt);
                }
            }

            stage.storyname = menu_options[menu.select].name;

            //Draw difficulty selector
            Menu_DifficultySelector(SCREEN_WIDTH - 55, 176);

            //Handle option and selection
            if (menu.trans_time > 0 && (menu.trans_time -= timer_dt) <= 0)
                Trans_Start();

            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                    CheckAndLoadWeek(menu.select);
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                    CheckAndLoadWeek(menu.select);
                }
            //Select option if cross is pressed
            if (pad_state.press & (PAD_START | PAD_CIRCLE))
            {
                menu.next_page = MenuPage_Stage;
                menu.page_param.stage.id = menu_options[menu.select].stage;
                menu.page_param.stage.story = true;
                menu.trans_time = FIXED_UNIT;
                menu.page_state.title.fade = FIXED_DEC(255,1);
                menu.page_state.title.fadespd = FIXED_DEC(510,1);
                menu.mbf->set_anim(menu.mbf, CharAnim_Up);
            }

                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 0; //Story Mode
                    Trans_Start();
                }
            }

            //Draw week name and tracks
            menu.font_arial.draw(&menu.font_arial,
                menu_options[menu.select].name,
                SCREEN_WIDTH - 6,
                6,
                FontAlign_Right
            );
            if(nomem == true)
            {
            //Draw mind games port version identification
            menu.font_arial.draw(&menu.font_arial,
                "No memorycard inserted",
                8,
                8,
                FontAlign_Left
            );
            }

            const char * const *trackp = menu_options[menu.select].tracks;
            for (size_t i = 0; i < COUNT_OF(menu_options[menu.select].tracks); i++, trackp++)
            {
                if (*trackp != NULL)
                    menu.font_arial.draw_col(&menu.font_arial,
                        *trackp,
                        2,
                        SCREEN_HEIGHT - (4 * 14) + (i * 10),
                        FontAlign_Left,
                        229 >> 1,
                        87 >> 1,
                        119 >> 1
                    );
            }
            //Draw menu characters
            menu.mbf->tick(menu.mbf);

            //Draw menu characters
            menu.mgf->tick(menu.mgf);

            Menu_DrawBG( 1, 24);

            char weektext[30];
            sprintf(weektext, "\\MENU\\WEEK%d.TIM;1", menu.select);

            //Draw behind week name strip
            RECT coverup_bar = {0, 0, SCREEN_WIDTH, 24};
            Gfx_DrawRect(&coverup_bar, 0, 0, 0);

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(42,1);
            menu.scroll += (next_scroll - menu.scroll) >> 3;

            Menu_DrawTrack( 0, 165);

            if (menu.next_page == menu.page || menu.next_page == MenuPage_Main)
            {
                //Draw all options
                for (u8 i = 0; i < COUNT_OF(menu_options); i++)
                {
                    s32 y = 161 + (i * 42) - (menu.scroll >> FIXED_SHIFT);
                    if (y <= 16)
                        continue;
                    if (y >= SCREEN_HEIGHT)
                        break;
                    Menu_DrawWeek(menu_options[i].week, SCREEN_WIDTH - 230, y);
                }
            }
            else if (animf_count & 2)
            {
                //Draw selected option
                Menu_DrawWeek(menu_options[menu.select].week, SCREEN_WIDTH - 230, 161 + (menu.select * 42) - (menu.scroll >> FIXED_SHIFT));
            }

            break;
        }
        case MenuPage_Freeplay:
        {
            static const struct
            {
                StageId stage;
                u32 col;
                const char *text;
                u8 icon;
                int music;
            } menu_options[] = {
                {StageId_4_4, 0xFFFC96D7, "smt", 1, 22},
                {StageId_1_4, 0xFF9271FD, "q1-tMaN", 0, 0},
                {StageId_1_1, 0xFF9271FD, "35-3", 2, 1},
                {StageId_1_2, 0xFF9271FD, "AO9l1", 2, 2},
                {StageId_1_3, 0xFF9271FD, "$()tN", 2, 3},
                {StageId_2_1, 0xFF223344, "m6-g-!", 3, 4},
                {StageId_2_2, 0xFF223344, "kcm", 3, 5},
                {StageId_2_3, 0xFF223344, "HSmp-", 7, 6},
                {StageId_3_1, 0xFF941653, "5j", 4, 7},
                {StageId_3_2, 0xFF941653, "A:M.v-m", 4, 8},
                {StageId_3_3, 0xFF941653, "+OF(", 4, 9},
                {StageId_4_1, 0xFFFC96D7, "ksS.4Ss:-", 5, 10},
                {StageId_4_2, 0xFFFC96D7, "zb", 5, 11},
                {StageId_4_3, 0xFFFC96D7, "ENA", 5, 12},
                {StageId_5_1, 0xFFA0D1FF, "jja", 6, 13},
                {StageId_5_2, 0xFFA0D1FF, "d9Vy9V", 6, 14},
                {StageId_5_3, 0xFFA0D1FF, "c:Sp-CL-LS(", 7, 15},
                {StageId_6_1, 0xFFFF78BF, "^_", 9, 16},
                {StageId_6_2, 0xFFFF78BF, ")L", 9, 17},
                {StageId_6_3, 0xFFFF78BF, "b)L", 10, 18},
                {StageId_7_1, 0xFFF6B604, "[]", 11, 19},
                {StageId_7_2, 0xFFF6B604, "TS!", 11, 20},
                {StageId_7_3, 0xFFF6B604, "mtOm", 11, 21},
            };
            //Audio_SeekXA_Track(stage.stage_def[menu_options[menu.select].stage].music_track);
            if (pad_state.press & PAD_R1)
            {
            	    Audio_StopXA();
		    switch (menu_options[menu.select].music)
		    {
			case 0:
			    //Play Tutorial Music
			    Audio_PlayXA_Track(XA_Tutorial, 0x40, 3, 1);
			    break;
			case 1:
			    //Play Bopeebo Music
			    Audio_PlayXA_Track(XA_Bopeebo, 0x40, 0, 1);
			    break;
			case 2:
			    //Play Fresh Music
			    Audio_PlayXA_Track(XA_Fresh, 0x40, 2, 1);
			    break;
			case 3:
			    //Play Dadbattle Music
			    Audio_PlayXA_Track(XA_Dadbattle, 0x40, 0, 1);
			    break;
			case 4:
			    //Play Spookeez Music
			    Audio_PlayXA_Track(XA_Spookeez, 0x40, 0, 1);
			    break;
			case 5:
			    //Play South Music
			    Audio_PlayXA_Track(XA_South, 0x40, 2, 1);
			    break;
			case 6:
			    //Play Monster Music
			    Audio_PlayXA_Track(XA_Monster, 0x40, 0, 1);
			    break;
			case 7:
			    //Play Pico Music
			    Audio_PlayXA_Track(XA_Pico, 0x40, 0, 1);
			    break;
			case 8:
			    //Play Philly Music
			    Audio_PlayXA_Track(XA_Philly, 0x40, 2, 1);
			    break;
			case 9:
			    //Play Blammed Music
			    Audio_PlayXA_Track(XA_Blammed, 0x40, 0, 1);
			    break;
			case 10:
			    //Play Satin Panties Music
			    Audio_PlayXA_Track(XA_SatinPanties, 0x40, 0, 1);
			    break;
			case 11:
		            //Play High Music
			    Audio_PlayXA_Track(XA_High, 0x40, 2, 1);
			    break;
			case 12:
			    //Play Milf Music
			    Audio_PlayXA_Track(XA_MILF, 0x40, 0, 1);
			    break;
			case 13:
			    //Play Cocoa Music
			    Audio_PlayXA_Track(XA_Cocoa, 0x40, 0, 1);
			    break;
			case 14:
			    //Play Eggnog Music
			    Audio_PlayXA_Track(XA_Eggnog, 0x40, 2, 1);
			    break;
			case 15:
			    //Play Winter Horrorland Music
			    Audio_PlayXA_Track(XA_WinterHorrorland, 0x40, 0, 1);
			    break;
			case 16:
			    //Play Senpai Music
			    Audio_PlayXA_Track(XA_Senpai, 0x40, 0, 1);
			    break;
			case 17:
			    //Play Roses Music
			    Audio_PlayXA_Track(XA_Roses, 0x40, 2, 1);
			    break;
			case 18:
			    //Play Thorns Music
			    Audio_PlayXA_Track(XA_Thorns, 0x40, 0, 1);
			    break;
			case 19:
			    //Play Ugh Music
			    Audio_PlayXA_Track(XA_Ugh, 0x40, 0, 1);
			    break;
			case 20:
			    //Play Guns Music
			    Audio_PlayXA_Track(XA_Guns, 0x40, 2, 1);
			    break;
			case 21:
			    //Play Tutorial Music
			    Audio_PlayXA_Track(XA_Stress, 0x40, 0, 1);
			    break;
			case 22:
			    //Play Test Music
			    Audio_PlayXA_Track(XA_Test, 0x40, 2, 1);
			    break;
		    }
		    Audio_WaitPlayXA();
	    }
            if (pad_state.press & PAD_L1)
            {
                //Play Tutorial Music
                Audio_StopXA();
                Audio_PlayXA_Track(XA_GettinFreaky, 0x40, 0, 1);
                Audio_WaitPlayXA();
            }
            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
                menu.page_state.freeplay.back_r = FIXED_DEC(255,1);
                menu.page_state.freeplay.back_g = FIXED_DEC(255,1);
                menu.page_state.freeplay.back_b = FIXED_DEC(255,1);
            }

            //Draw page label
            menu.font_arial.draw(&menu.font_arial,
                "Press R1 to listen to song/Press L1 to stop song",
                16,
                217,
                FontAlign_Left
            );

            //Draw difficulty selector
            Menu_DifficultySelector(SCREEN_WIDTH - 62, 74);

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Select option if cross is pressed
                if (pad_state.press & (PAD_START | PAD_CIRCLE) && !switchscreen)
                {
                    menu.djbf->set_anim(menu.djbf, CharAnim_Up);
                    switchscreen = true;
                }

                if (switchscreen && Animatable_Ended(&menu.djbf->animatable))
                {
                    menu.next_page = MenuPage_Stage;
                    menu.page_param.stage.id = menu_options[menu.select].stage;
                    menu.page_param.stage.story = false;
                    Trans_Start();
                    switchscreen = false;
                }
                //Select option if cross is pressed
                if (pad_state.press & PAD_TRIANGLE)
                {
                    menu.next_page = MenuPage_Stage;
                    menu.page_param.stage.id = menu_options[menu.select].stage;
                    menu.page_param.stage.story = true;
                    Trans_Start();
                }

                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 1; //Freeplay
                    Trans_Start();
                }
            }

            //Draw box at the bottom
            RECT song_box = {0, 210, 320, 30};
            Gfx_DrawRect(&song_box, 0, 0, 0);

            //Draw page label
            menu.font_kata.draw(&menu.font_kata,
                "AM-6Ob",
                8,
                7,
                FontAlign_Left
            );
            //Draw box at the bottom
            RECT top_box = {0, 0, 320, 30};
            Gfx_DrawRect(&top_box, 0, 0, 0);

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(32,1);
            menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                //Get position on screen
                s32 y = (i * 31) - 8 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;
                //Draw Icon
                Menu_DrawHealth(menu_options[i].icon, strlen(menu_options[i].text) * 14 + 32 + 8 -18, SCREEN_HEIGHT2 + y - 17 -4, menu.select == i);

                //Draw text
                menu.font_kata.draw_col(&menu.font_kata,
                    menu_options[i].text,
                    15,
                    SCREEN_HEIGHT2 + y - 10,
                    FontAlign_Left,
                    (menu.select == i) ? 128 : 64,
                    (menu.select == i) ? 128 : 64,
                    (menu.select == i) ? 128 : 64
                );
            }
            //dj bf spin record
            if (pad_state.press & (PAD_R1 | PAD_L1))
                menu.djbf->set_anim(menu.djbf, CharAnim_Left);

            //Draw dj bf
                    menu.djbf->tick(menu.djbf);

            //Draw background
            fixed_t tgt_r = (fixed_t)((menu_options[menu.select].col >> 16) & 0xFF) << FIXED_SHIFT;
            fixed_t tgt_g = (fixed_t)((menu_options[menu.select].col >>  8) & 0xFF) << FIXED_SHIFT;
            fixed_t tgt_b = (fixed_t)((menu_options[menu.select].col >>  0) & 0xFF) << FIXED_SHIFT;

            menu.page_state.freeplay.back_r += (tgt_r - menu.page_state.freeplay.back_r) >> 4;
            menu.page_state.freeplay.back_g += (tgt_g - menu.page_state.freeplay.back_g) >> 4;
            menu.page_state.freeplay.back_b += (tgt_b - menu.page_state.freeplay.back_b) >> 4;

            Menu_DrawBack(
                true,
                8,
                menu.page_state.freeplay.back_r >> (FIXED_SHIFT + 1),
                menu.page_state.freeplay.back_g >> (FIXED_SHIFT + 1),
                menu.page_state.freeplay.back_b >> (FIXED_SHIFT + 1),
                0, 0, 0
            );
            break;
        }
        case MenuPage_Mods:
        {
            static const struct
            {
                StageId stage;
                const char *text;
                boolean difficulty;
                u8 icon;
            } menu_options[] = {
                {StageId_8_1, "STILL ALIVE", true, 12},
                {StageId_8_2, "|}~", false, 1},
                {StageId_2_4,    "jij9j-", false, 26},
            };

            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
            }

            //Draw box at the bottom
            RECT song_box = {0, 210, 320, 30};
            Gfx_DrawRect(&song_box, 0, 0, 0);

            //Draw page label
            menu.font_kata.draw(&menu.font_kata,
                "H9!",
                8,
                7,
                FontAlign_Left
            );

            //Draw box at the bottom
            RECT top_box = {0, 0, 320, 30};
            Gfx_DrawRect(&top_box, 0, 0, 0);

            //Draw difficulty selector
            if (menu_options[menu.select].difficulty)
                Menu_DifficultySelector(SCREEN_WIDTH - 62, 74);

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP && !switchscreen)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN && !switchscreen)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Select option if cross is pressed
                if (pad_state.press & (PAD_START | PAD_CIRCLE) && !switchscreen)
                {
                    menu.djbf->set_anim(menu.djbf, CharAnim_Up);
                    switchscreen = true;
                }

                if (switchscreen && Animatable_Ended(&menu.djbf->animatable))
                {
                    menu.next_page = MenuPage_Stage;
                    menu.page_param.stage.id = menu_options[menu.select].stage;
                    menu.page_param.stage.story = false;
                    if (!menu_options[menu.select].difficulty)
                        menu.page_param.stage.diff = StageDiff_Hard;
                    Trans_Start();
                    switchscreen = false;
                }

                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 2; //Mods
                    menu.select = 0;
                    Trans_Start();
                }
            }

            //Draw dj bf
                    menu.djbf->tick(menu.djbf);

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(24,1);
            menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                //Get position on screen
                s32 y = (i * 24) - 8 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;

                //Draw Icon
                Menu_DrawHealth(menu_options[i].icon, strlen(menu_options[i].text) * 14 + 32 + 8 -18, SCREEN_HEIGHT2 + y - 17 -4, menu.select == i);

                //Draw text
                menu.font_kata.draw_col(&menu.font_kata,
                    menu_options[i].text,
                    15,
                    SCREEN_HEIGHT2 + y - 10,
                    FontAlign_Left,
                    (menu.select == i) ? 128 : 64,
                    (menu.select == i) ? 128 : 64,
                    (menu.select == i) ? 128 : 64
                );
            }

            //Draw background
            Menu_DrawBack(
                true,
                8,
                197 >> 1, 240 >> 1, 95 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_Awards:
        {
            struct
            {
                const char *achievementtext;
            } menu_options[] = {
                {"Full Combo Week 1 On Hard Difficulty"},
                {"Full Combo Week 2 On Hard Difficulty"},
                {"Full Combo Week 3 On Hard Difficulty"},
                {"Full Combo Week 4 On Hard Difficulty"},
                {"Full Combo Week 5 On Hard Difficulty"},
                {"Full Combo Week 6 On Hard Difficulty"},
                {"Full Combo Week 7 On Hard Difficulty"},
                {"Full Combo Senbonzakura On Hard Difficulty"},
                {"Perfect Any Song In Nightmare Difficulty"},
                {"3ds User? Turn On Low Quality Mode"},
                {"Beat Any Song With No Hud On Hard Difficulty"},
                {"Play Swapped Mode"},
                {"Play with a friend"},
                {"Debugger enter the debug menu"},
            };

            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
            }

            //Draw page label
        menu.font_arial.draw(&menu.font_arial,
            menu_options[menu.select].achievementtext,
            16,
            217,
            FontAlign_Left
        );

        //Draw box at the bottom
        RECT song_box = {0, 210, 320, 30};
        Gfx_DrawRect(&song_box, 0, 0, 0);

        //Draw page label
        menu.font_kata.draw(&menu.font_kata,
            "aQ-(",
            8,
            7,
            FontAlign_Left
        );
        if(menu.select==10)
        {
        	if(stage.prefs.unlockedAchievements[10] == false)
        		stage.prefs.nohudsong ="NONE";
        	//Draw page label
        	sprintf(stage.nohudsong_text, "Song Beaten: %s",stage.prefs.nohudsong);
		menu.font_arial.draw(&menu.font_arial,
		    stage.nohudsong_text,
		    318,
		    32,
		    FontAlign_Right
		);
		menu.font_arial.draw(&menu.font_arial,
		    "Press Square To Reset Achievement",
		    318,
		    200,
		    FontAlign_Right
		);
		if (pad_state.press & PAD_SQUARE)
		{
			stage.prefs.unlockedAchievements[10] = false;
			stage.prefs.nohudsong ="NONE";
			WriteSaveDataStructToBinaryAndSaveItOnTheFuckingMemoryCard();
		}
	}
        //Draw box at the bottom
        RECT top_box = {0, 0, 320, 30};
        Gfx_DrawRect(&top_box, 0, 0, 0);

            /*if(nomem == true)
            {
            //Draw mind games port version identification
            menu.font_arial.draw(&menu.font_arial,
                "No memorycard inserted",
                8,
                8,
                FontAlign_Left
            );
            }*/

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = NUM_ACHIEVEMENTS - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < NUM_ACHIEVEMENTS - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 3; //Awards
                    Trans_Start();
                }
            }

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(40,1);
            menu.scroll += (next_scroll - menu.scroll) >> 3;

            for (int i = 0; i < NUM_ACHIEVEMENTS; i++)
            {
                //Get position on screen
                s32 y = (i * 40) - 8 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;

        Achievement ach = Get_Achievement(i);

        if (Achievement_IsUnlocked(i))
        {
                //Draw Icon
                Menu_DrawHealth(ach.iconIndex, 48 + (y / 4) -18, SCREEN_HEIGHT2 + y - 17, menu.select == i);

                //Draw text
                menu.font_arial.draw_col(&menu.font_arial,
                    Menu_LowerIf(ach.name, NULL),
                    100 + (y / 4) -18,
                    SCREEN_HEIGHT2 + y - 6,
                    FontAlign_Left,
                    0 >> 1,
                    0 >> 1,
                    0 >> 1
                );
                }
                else
                {
                    //Draw Icon
                Menu_DrawHealth(35, 48 + (y / 4) -18, SCREEN_HEIGHT2 + y - 17, menu.select == i);

                //Draw text
                menu.font_arial.draw_col(&menu.font_arial,
                    "?",
                    100 + (y / 4) -18,
                    SCREEN_HEIGHT2 + y - 6,
                    FontAlign_Left,
                    0 >> 1,
                    0 >> 1,
                    0 >> 1
                );
                }

            }

            //Draw background
            Menu_DrawBack(
                true,
                8,
                153 >> 1, 118 >> 1, 236 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_Credits:
        {
            static const struct
            {
                const char *bold;
            } menu_headers[] = {
                {"flop engine"},
                {" "},
                {" "},
                {" "},
                {" "},
                {" "},
                {" "},
                {"psxfunkin"},
                {" "},
                {" "},
            };

            static const struct
            {
                const char *text;
                int abouticon;
                u8 icon;
                u32 col;
                const char *aboutname;
                const char *aboutdesc1;
                const char *aboutdesc2;
                const char *aboutdesc3;
                const char *aboutdesc4;
            } menu_options[] = {
                {"nintendo bro", 0, 21, 0xFF51ffb3, "Nintendo Bro385", "Flop Engine", NULL, NULL, NULL},
                {"igorsou3000", 1, 22, 0xFFfb6c23, "IgorSou3000", "Freeplay & Credits", "Icon code", "Dialogue", "Implemented Movies"},
                {"unstop4ble", 2, 23, 0xFF639bff, "UNSTOP4BLE", "Coding Help", "Story Backgrounds", "Achievement Code", NULL},
                {"spicyjpeg", 3, 24, 0xFFfb6c23, "spicyjpeg", "Coding Help", "Movie & Save", "code", NULL},
                {"luka", 4, 25, 0xe7ce00, "Luka", "Results Screen Music", NULL, NULL, NULL},
                {" ", 14, 30, 0xFF51ffb3, NULL, NULL, NULL, NULL, NULL},
                {" ", 14, 30, 0xFFc5f05f, NULL, NULL, NULL, NULL, NULL},
                {"ckdev", 5, 26,0xFFc5f05f, "CKDEV", "PSXFunkin Creator", NULL, NULL, NULL},
            };
            //Draw text
                    menu.font_arial.draw(&menu.font_arial,
                        menu_options[menu.select].aboutname,
                        256,
                        128,
                        FontAlign_Center
                    );
                    menu.font_arial.draw(&menu.font_arial,
                        menu_options[menu.select].aboutdesc1,
                        256,
                        140,
                        FontAlign_Center
                    );
                    menu.font_arial.draw(&menu.font_arial,
                        menu_options[menu.select].aboutdesc2,
                        256,
                        152,
                        FontAlign_Center
                    );
                    menu.font_arial.draw(&menu.font_arial,
                        menu_options[menu.select].aboutdesc3,
                        256,
                        164,
                        FontAlign_Center
                    );
                    menu.font_arial.draw(&menu.font_arial,
                        menu_options[menu.select].aboutdesc4,
                        256,
                        176,
                        FontAlign_Center
                    );
                    Menu_DrawBigCredits(menu_options[menu.select].abouticon, 224, 46);
            if(menu_options[menu.select].abouticon == 14)
	    {
	        if (up==true)
	        {
	                menu.select--;
	        }
	        if (down ==true)
	        {
	                menu.select++;
	        }
	    }
            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
                menu.page_state.freeplay.back_r = FIXED_DEC(255,1);
                menu.page_state.freeplay.back_g = FIXED_DEC(255,1);
                menu.page_state.freeplay.back_b = FIXED_DEC(255,1);
            }

            //Draw box at the bottom
            RECT song_box = {0, 210, 320, 30};
            Gfx_DrawRect(&song_box, 0, 0, 0);

            //Draw page label
            menu.font_kata.draw(&menu.font_kata,
                "hOZ9t",
                8,
                7,
                FontAlign_Left
            );
            //Draw box at the bottom
            RECT top_box = {0, 0, 320, 30};
            Gfx_DrawRect(&top_box, 0, 0, 0);

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 4; //Credits
                    menu.select = 0;
                    Trans_Start();
                }
            }
            if (pad_state.press & PAD_UP){down=false; up=true;}
            if (pad_state.press & PAD_DOWN){down=true; up=false;}

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(24,1);
            menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                //Get position on screen
                s32 y = (i * 24) - 6 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 6)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 6)
                    break;
                //Draw Icon
                Menu_DrawHealth(menu_options[i].icon, strlen(menu_options[i].text) * 7 + 32 + 35, SCREEN_HEIGHT2 + y - 15, menu.select == i);

                //Draw headers
                /*menu.font_bold.draw(&menu.font_bold,
                    Menu_LowerIf(menu_headers[i].bold, NULL),
                    10,
                    SCREEN_HEIGHT2 + y - 8 - 24,
                    FontAlign_Left
                );*/
                menu.font_bold.draw(&menu.font_bold,
                        Menu_LowerIf(menu_headers[i].bold, NULL),
                        10,
                    	SCREEN_HEIGHT2 + y - 8 - 24,
                        FontAlign_Left
                    );

                //Draw text
                menu.font_arial.draw_col(&menu.font_arial,
                    Menu_LowerIf(menu_options[i].text, menu.select != i),
                    60,
                    SCREEN_HEIGHT2 + y - 6,
                    FontAlign_Left,
                    0 >> 1,
                    0 >> 1,
                    0 >> 1
                );

            }

            //Draw box for about credits
            RECT about_box = {192, 0, 128, 240};
            Gfx_BlendRect(&about_box, 62, 62, 62, 0);

            //Draw background
            fixed_t tgt_r = (fixed_t)((menu_options[menu.select].col >> 16) & 0xFF) << FIXED_SHIFT;
            fixed_t tgt_g = (fixed_t)((menu_options[menu.select].col >>  8) & 0xFF) << FIXED_SHIFT;
            fixed_t tgt_b = (fixed_t)((menu_options[menu.select].col >>  0) & 0xFF) << FIXED_SHIFT;

            menu.page_state.freeplay.back_r += (tgt_r - menu.page_state.freeplay.back_r) >> 4;
            menu.page_state.freeplay.back_g += (tgt_g - menu.page_state.freeplay.back_g) >> 4;
            menu.page_state.freeplay.back_b += (tgt_b - menu.page_state.freeplay.back_b) >> 4;

            Menu_DrawBack(
                true,
                8,
                menu.page_state.freeplay.back_r >> (FIXED_SHIFT + 1),
                menu.page_state.freeplay.back_g >> (FIXED_SHIFT + 1),
                menu.page_state.freeplay.back_b >> (FIXED_SHIFT + 1),
                0, 0, 0
            );
            break;
        }
        case MenuPage_Options:
        {
            //Draw difficulty selector
        Menu_Options(160, 58);
        switch(menu.page_param.stage.setselect)
        {
            case 0:
            {
            static const char *gamemode_strs[] = {"NORMAL", "SWAP", "TWO PLAYER"};
            static const char *notes_strs[] = {"NORMAL", "PIXEL", "CLUCKY", "DDR"};
            static const struct
            {
                enum
                {
                    OptType_Boolean,
                    OptType_Enum,
                } type;
                const char *text;
                void *value;
                union
                {
                    struct
                    {
                        int a;
                    } spec_boolean;
                    struct
                    {
                        s32 max;
                        const char **strs;
                    } spec_enum;
                } spec;
                int optionsicon;
            } menu_options[] = {
                {OptType_Enum,    "GAMEMODE", &stage.prefs.mode, {.spec_enum = {COUNT_OF(gamemode_strs), gamemode_strs}}, 0},
                {OptType_Enum,    "NOTE SKIN", &stage.prefs.noteskin, {.spec_enum = {COUNT_OF(notes_strs), notes_strs}}, 32},
                //{OptType_Boolean, "INTERPOLATION", &stage.prefs.expsync, 15},
                //{OptType_Boolean, "GHOST TAP ", &stage.prefs.ghost, {.spec_boolean = {0}}, 13},
                {OptType_Boolean, "DOWNSCROLL", &stage.prefs.downscroll, {.spec_boolean = {0}}, 6},
                {OptType_Boolean, "BOTPLAY", &stage.prefs.botplay, {.spec_boolean = {0}}, 8},
                {OptType_Boolean, "LOW QUALITY", &stage.prefs.lowquality, {.spec_boolean = {0}}, 10},
                {OptType_Boolean, "NO FLASHING", &stage.prefs.flashing, {.spec_boolean = {0}}, 16},
                {OptType_Boolean, "HELL", &stage.prefs.hell, {.spec_boolean = {0}}, 18},
                {OptType_Boolean, "ONLY ARROWS", &stage.prefs.onlyarrows, {.spec_boolean = {0}}, 18},
            };

            //Initialize page
            if (menu.page_swap){
                menu.scroll = -31;
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "gameplay",
                312,
                217,
                FontAlign_Right
            );
            if(nomem == true)
            {
                //Draw mind games port version identification
                menu.font_arial.draw(&menu.font_arial,
                "No memorycard inserted",
                8,
                8,
                FontAlign_Left
                );
            }

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Handle option changing
                switch (menu_options[menu.select].type)
                {
                    case OptType_Boolean:
                        if (pad_state.press & (PAD_CIRCLE | PAD_LEFT | PAD_RIGHT))
                            *((boolean*)menu_options[menu.select].value) ^= 1;
                        break;
                    case OptType_Enum:
                if (pad_state.press & PAD_LEFT)
                {
                            if (--*((s32*)menu_options[menu.select].value) < 0)
                                *((s32*)menu_options[menu.select].value) = menu_options[menu.select].spec.spec_enum.max - 1;
                }
                        if (pad_state.press & PAD_RIGHT)
                            {
                            if (++*((s32*)menu_options[menu.select].value) >= menu_options[menu.select].spec.spec_enum.max)
                                *((s32*)menu_options[menu.select].value) = 0;
                            }
                        break;
                }
                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 5; //Options
                    Trans_Start();
                }
            }
             //Draw box at the bottom
                //Draw box at the bottom
                RECT song_box = {0, 210, 320, 30};
                Gfx_DrawRect(&song_box, 0, 0, 0);

                //Draw page label
                menu.font_kata.draw(&menu.font_kata,
                    "e6l2S",
                    8,
                    7,
                    FontAlign_Left
                );
                //Draw box at the bottom
                RECT top_box = {0, 0, 320, 30};
                Gfx_DrawRect(&top_box, 0, 0, 0);


           //Draw options
           s32 next_scroll = menu.select * FIXED_DEC(32,1);
           menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                 //Get position on screen
                s32 y = (i * 31) - 8 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;

                //Draw text
                char text[0x80];
                switch (menu_options[i].type)
                {
                    case OptType_Boolean:
                    {
                        Menu_DrawOptions(*((boolean*)menu_options[i].value) + menu_options[i].optionsicon, 18, SCREEN_HEIGHT2 + y - 15 - 28, true);
                    sprintf(text, "%s %s", menu_options[i].text, *((boolean*)menu_options[i].value) ? "ON" : "OFF");
                }
                        break;
                    case OptType_Enum:
                    {
                            Menu_DrawOptions(*((s32*)menu_options[i].value) + menu_options[i].optionsicon, 2, SCREEN_HEIGHT2 + y - 15 - 28, false);
                            sprintf(text, "%s %s", menu_options[i].text, menu_options[i].spec.spec_enum.strs[*((s32*)menu_options[i].value)]);
                        }
                        break;
                }
                /*menu.font_bold.draw(&menu.font_bold,
                    Menu_LowerIf(text, menu.select != i),
                    48 + (y >> 2),
                    SCREEN_HEIGHT2 + y - 8,
                    FontAlign_Left
                );*/
                //Draw text
            menu.font_bold.draw_col(&menu.font_bold,
                Menu_LowerIf(text, NULL),
                70,
                SCREEN_HEIGHT2 + y - 34,
                FontAlign_Left,
                (menu.select == i) ? 128 : 64,
                (menu.select == i) ? 128 : 64,
                (menu.select == i) ? 128 : 64
            );
            }

            //Draw background
            Menu_DrawBack(
                true,
                8,
                253 >> 1, 113 >> 1, 155 >> 1,
                0, 0, 0
            );
            break;
                }
                case 1:
                {
                    static const struct
            {
            const char *text;
            int music;
            int icons;
            } menu_options[] = {
                {"GETTIN FREAKY", 0, 13},
                {"OG GETTIN FREAKY", 1, 14},
                {"LUDUM", 2, 15},
            };
            switch (menu_options[menu.select].music)
                {
                case 0:
                {
                    if (pad_state.press & (PAD_START | PAD_CIRCLE))
                    {
                        //Play Tutorial Music
                        Audio_StopXA();
                        Audio_PlayXA_Track(XA_GettinFreaky, 0x40, 0, 1);
                        Audio_WaitPlayXA();
                        stage.prefs.menumusic=0;
                        /*menu.next_page = MenuPage_Opening;
                        menu.next_select = 0; //Mods
                        Trans_Start();*/
                    }
                    break;
                }
                case 1:
                {
                    if (pad_state.press & (PAD_START | PAD_CIRCLE))
                    {
                        //Play Tutorial Music
                        Audio_StopXA();
                        Audio_PlayXA_Track(XA_Freeky, 0x40, 2, 1);
                        Audio_WaitPlayXA();
                        stage.prefs.menumusic=1;
                        /*menu.next_page = MenuPage_Opening;
                        menu.next_select = 0; //Mods

                        Trans_Start();*/
                    }
                    break;
                }
                case 2:
                {
                    if (pad_state.press & (PAD_START | PAD_CIRCLE))
                    {
                        //Play Tutorial Music
                        Audio_StopXA();
                        Audio_PlayXA_Track(XA_Ludum, 0x40, 3, 1);
                        Audio_WaitPlayXA();
                        stage.prefs.menumusic=2;
                        /*menu.next_page = MenuPage_Opening;
                        menu.next_select = 0; //Mods
                        Trans_Start();*/
                    }
                    break;
                }
                }
            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
            }
            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "music",
                312,
                217,
                FontAlign_Right
            );
            if(nomem == true)
            {
                //Draw mind games port version identification
                menu.font_arial.draw(&menu.font_arial,
                "No memorycard inserted",
                8,
                8,
                FontAlign_Left
                );
            }
            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }


                //Return to main menu if circle is pressed
                if (pad_state.press & PAD_CROSS)
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 5; //Options
                    Trans_Start();
                }
            }
            //Draw box at the bottom
                //Draw box at the bottom
                RECT song_box = {0, 210, 320, 30};
                Gfx_DrawRect(&song_box, 0, 0, 0);

                //Draw page label
                menu.font_kata.draw(&menu.font_kata,
                    "e6l2S",
                    8,
                    7,
                    FontAlign_Left
                );
                //Draw box at the bottom
                RECT top_box = {0, 0, 320, 30};
                Gfx_DrawRect(&top_box, 0, 0, 0);


            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(24,1);
            menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                //Get position on screen
                s32 y = (i * 24) - 8;
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;

                Menu_DrawOptions(menu_options[i].icons, 18, SCREEN_HEIGHT2 + y - 15 - 28, true);

                //Draw text
            menu.font_bold.draw_col(&menu.font_bold,
                Menu_LowerIf(menu_options[i].text, NULL),
                70,
                SCREEN_HEIGHT2 + y - 34,
                FontAlign_Left,
                (menu.select == i) ? 128 : 64,
                (menu.select == i) ? 128 : 64,
                (menu.select == i) ? 128 : 64
            );
            }

            //Draw background
            Menu_DrawBack(
                true,
                8,
                253 >> 1, 113 >> 1, 155 >> 1,
                0, 0, 0
            );
            break;
            }
            }
            break;
        }
    #ifdef PSXF_NETWORK
        case MenuPage_NetHost:
        {
            const size_t menu_options = 3;

            //Initialize page
            if (menu.page_swap)
            {
                menu.page_state.net_host.type = false;
                menu.page_state.net_host.port[0] = '\0';
                menu.page_state.net_host.pass[0] = '\0';
            }

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (!menu.page_state.net_host.type)
                {
                    //Change option
                    if (pad_state.press & PAD_UP)
                    {
                        if (menu.select > 0)
                            menu.select--;
                        else
                            menu.select = menu_options - 1;
                    }
                    if (pad_state.press & PAD_DOWN)
                    {
                        if (menu.select < menu_options - 1)
                            menu.select++;
                        else
                            menu.select = 0;
                    }

                    //Handle selection when cross is pressed
                    if (pad_state.press & (PAD_START | PAD_CROSS))
                    {
                        switch (menu.select)
                        {
                            case 0: //Port
                            case 1: //Pass
                                menu.page_state.net_host.type = true;
                                break;
                            case 2: //Host
                                if (!Network_HostPort(menu.page_state.net_host.port, menu.page_state.net_host.pass))
                                {
                                    menu.next_page = MenuPage_NetOpWait;
                                    menu.next_select = 0;
                                    Trans_Start();
                                }
                                break;
                        }
                    }

                    //Return to main menu if circle is pressed
                    if (pad_state.press & PAD_CIRCLE)
                    {
                        menu.next_page = MenuPage_Main;
                        menu.next_select = 5; //Host Server
                        Trans_Start();
                    }
                }
                else
                {
                    //Stop typing when start is pressed
                    if (pad_state.press & PAD_START)
                    {
                        switch (menu.select)
                        {
                            case 0: //Port
                            case 1: //Pass
                                menu.page_state.net_host.type = false;
                                break;
                        }
                    }
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "HOST SERVER",
                16,
                SCREEN_HEIGHT - 32,
                FontAlign_Left
            );

            //Draw options
            MenuStr_Process(&menu.page_state.net_host.port, 64 + 3 * 0, 64 + 16 * 0, "Port: %s", menu.select == 0, menu.page_state.net_host.type);
            MenuStr_Process(&menu.page_state.net_host.pass, 64 + 3 * 1, 64 + 16 * 1, "Pass: %s", menu.select == 1, menu.page_state.net_host.type);
            menu.font_bold.draw(&menu.font_bold, Menu_LowerIf("HOST", menu.select != 2), 64 + 3 * 2, 64 + 16 * 2, FontAlign_Left);

            //Draw background
            Menu_DrawBack(
                true,
                8,
                146 >> 1, 113 >> 1, 253 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetJoin:
        {
            const size_t menu_options = 4;

            //Initialize page
            if (menu.page_swap)
            {
                menu.page_state.net_join.type = false;
                menu.page_state.net_join.ip[0] = '\0';
                menu.page_state.net_join.port[0] = '\0';
                menu.page_state.net_join.pass[0] = '\0';
            }

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (!menu.page_state.net_join.type)
                {
                    //Change option
                    if (pad_state.press & PAD_UP)
                    {
                        if (menu.select > 0)
                            menu.select--;
                        else
                            menu.select = menu_options - 1;
                    }
                    if (pad_state.press & PAD_DOWN)
                    {
                        if (menu.select < menu_options - 1)
                            menu.select++;
                        else
                            menu.select = 0;
                    }

                    //Handle selection when cross is pressed
                    if (pad_state.press & (PAD_START | PAD_CROSS))
                    {
                        switch (menu.select)
                        {
                            case 0: //Ip
                            case 1: //Port
                            case 2: //Pass
                                menu.page_state.net_join.type = true;
                                break;
                            case 3: //Join
                                if (!Network_Join(menu.page_state.net_join.ip, menu.page_state.net_join.port, menu.page_state.net_join.pass))
                                {
                                    menu.next_page = MenuPage_NetConnect;
                                    menu.next_select = 0;
                                    Trans_Start();
                                }
                                break;
                        }
                    }

                    //Return to main menu if circle is pressed
                    if (pad_state.press & PAD_CIRCLE)
                    {
                        menu.next_page = MenuPage_Main;
                        menu.next_select = 4; //Join Server
                        Trans_Start();
                    }
                }
                else
                {
                    //Stop typing when start is pressed
                    if (pad_state.press & PAD_START)
                    {
                        switch (menu.select)
                        {
                            case 0: //Join
                            case 1: //Port
                            case 2: //Pass
                                menu.page_state.net_join.type = false;
                                break;
                        }
                    }
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "JOIN SERVER",
                16,
                SCREEN_HEIGHT - 32,
                FontAlign_Left
            );

            //Draw options
            MenuStr_Process(&menu.page_state.net_join.ip, 64 + 3 * 0, 64 + 16 * 0, "Address: %s", menu.select == 0, menu.page_state.net_join.type);
            MenuStr_Process(&menu.page_state.net_join.port, 64 + 3 * 1, 64 + 16 * 1, "Port: %s", menu.select == 1, menu.page_state.net_join.type);
            MenuStr_Process(&menu.page_state.net_join.pass, 64 + 3 * 2, 64 + 16 * 2, "Pass: %s", menu.select == 2, menu.page_state.net_join.type);
            menu.font_bold.draw(&menu.font_bold, Menu_LowerIf("JOIN", menu.select != 3), 64 + 3 * 3, 64 + 16 * 3, FontAlign_Left);

            //Draw background
            Menu_DrawBack(
                true,
                8,
                146 >> 1, 113 >> 1, 253 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetConnect:
        {
            //Change state according to network state
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (!Network_Connected())
                {
                    //Disconnected
                    menu.next_page = MenuPage_NetFail;
                    menu.next_select = 0;
                    Trans_Start();
                }
                else if (Network_Allowed())
                {
                    //Allowed to join
                    menu.next_page = MenuPage_NetLobby;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "CONNECTING",
                SCREEN_WIDTH2,
                SCREEN_HEIGHT2 - 8,
                FontAlign_Center
            );

            //Draw background
            Menu_DrawBack(
                true,
                8,
                113 >> 1, 146 >> 1, 253 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetOpWait:
        {
            //Change state according to network state
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (!Network_Connected())
                {
                    //Disconnected
                    menu.next_page = MenuPage_NetFail;
                    menu.next_select = 0;
                    Trans_Start();
                }
                else if (Network_HasPeer())
                {
                    //Peer has joined
                    menu.next_page = MenuPage_NetOp;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "WAITING FOR PEER",
                SCREEN_WIDTH2,
                SCREEN_HEIGHT2 - 8,
                FontAlign_Center
            );

            //Draw background
            Menu_DrawBack(
                true,
                8,
                113 >> 1, 146 >> 1, 253 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetOp:
        {
            static const struct
            {
                boolean diff;
                StageId stage;
                const char *text;
            } menu_options[] = {
                //{StageId_4_4, "TEST"},
                {true,  StageId_1_4, "TUTORIAL"},
                {true,  StageId_1_1, "BOPEEBO"},
                {true,  StageId_1_2, "FRESH"},
                {true,  StageId_1_3, "DADBATTLE"},
                {true,  StageId_2_1, "SPOOKEEZ"},
                {true,  StageId_2_2, "SOUTH"},
                {true,  StageId_2_3, "MONSTER"},
                {true,  StageId_3_1, "PICO"},
                {true,  StageId_3_2, "PHILLY NICE"},
                {true,  StageId_3_3, "BLAMMED"},
                {true,  StageId_4_1, "SATIN PANTIES"},
                {true,  StageId_4_2, "HIGH"},
                {true,  StageId_4_3, "MILF"},
                {true,  StageId_5_1, "COCOA"},
                {true,  StageId_5_2, "EGGNOG"},
                {true,  StageId_5_3, "WINTER HORRORLAND"},
                {true,  StageId_6_1, "SENPAI"},
                {true,  StageId_6_2, "ROSES"},
                {true,  StageId_6_3, "THORNS"},
                {true,  StageId_7_1, "UGH"},
                {true,  StageId_7_2, "GUNS"},
                {true,  StageId_7_3, "STRESS"},
                {false, StageId_Kapi_1, "WOCKY"},
                {false, StageId_Kapi_2, "BEATHOVEN"},
                {false, StageId_Kapi_3, "HAIRBALL"},
                {false, StageId_Kapi_4, "NYAW"},
                {true,  StageId_Clwn_1, "IMPROBABLE OUTSET"},
                {true,  StageId_Clwn_2, "MADNESS"},
                {true,  StageId_Clwn_3, "HELLCLOWN"},
                {false, StageId_Clwn_4, "EXPURGATION"},
                {false, StageId_2_4, "CLUCKED"},
            };

            //Initialize page
            if (menu.page_swap)
            {
                menu.scroll = COUNT_OF(menu_options) * FIXED_DEC(24 + SCREEN_HEIGHT2,1);
                menu.page_param.stage.diff = StageDiff_Normal;
                menu.page_state.net_op.swap = false;
            }

            //Handle option and selection
            if (menu.next_page == menu.page && Trans_Idle())
            {
                //Check network state
                if (!Network_Connected())
                {
                    //Disconnected
                    menu.next_page = MenuPage_NetFail;
                    menu.next_select = 0;
                    Trans_Start();
                }
                else if (!Network_HasPeer())
                {
                    //Peer disconnected
                    menu.next_page = MenuPage_NetOpWait;
                    menu.next_select = 0;
                    Trans_Start();
                }

                //Change option
                if (pad_state.press & PAD_UP)
                {
                    if (menu.select > 0)
                        menu.select--;
                    else
                        menu.select = COUNT_OF(menu_options) - 1;
                }
                if (pad_state.press & PAD_DOWN)
                {
                    if (menu.select < COUNT_OF(menu_options) - 1)
                        menu.select++;
                    else
                        menu.select = 0;
                }

                //Select option if cross is pressed
                if (pad_state.press & (PAD_START | PAD_CROSS))
                {
                    //Load stage
                    Network_SetReady(false);
                    stage.mode = menu.page_state.net_op.swap ? StageMode_Net2 : StageMode_Net1;
                    menu.next_page = MenuPage_Stage;
                    menu.page_param.stage.id = menu_options[menu.select].stage;
                    if (!menu_options[menu.select].diff)
                        menu.page_param.stage.diff = StageDiff_Hard;
                    menu.page_param.stage.story = false;
                    Trans_Start();
                }

                //Swap characters if triangle is pressed
                if (pad_state.press & PAD_TRIANGLE)
                    menu.page_state.net_op.swap ^= true;
            }

            //Draw op controls
            const char *control_txt;

            control_txt = menu.page_state.net_op.swap ? "You will be Player 2. Press Triangle to swap." : "You will be Player 1. Press Triangle to swap.";
            menu.font_arial.draw_col(&menu.font_arial, control_txt, 24, SCREEN_HEIGHT - 24 - 12, FontAlign_Left, 0x80, 0x80, 0x80);
            menu.font_arial.draw_col(&menu.font_arial, control_txt, 24 + 1, SCREEN_HEIGHT - 24 - 12 + 1, FontAlign_Left, 0x00, 0x00, 0x00);

            //Draw difficulty selector
            if (menu_options[menu.select].diff)
                Menu_DifficultySelector(SCREEN_WIDTH - 100, SCREEN_HEIGHT2 - 48);

            //Draw options
            s32 next_scroll = menu.select * FIXED_DEC(24,1);
            menu.scroll += (next_scroll - menu.scroll) >> 4;

            for (u8 i = 0; i < COUNT_OF(menu_options); i++)
            {
                //Get position on screen
                s32 y = (i * 24) - 8 - (menu.scroll >> FIXED_SHIFT);
                if (y <= -SCREEN_HEIGHT2 - 8)
                    continue;
                if (y >= SCREEN_HEIGHT2 + 8)
                    break;

                //Draw text
                menu.font_bold.draw(&menu.font_bold,
                    Menu_LowerIf(menu_options[i].text, menu.select != i),
                    48 + (y >> 2),
                    SCREEN_HEIGHT2 + y - 8,
                    FontAlign_Left
                );
            }

            //Draw background
            Menu_DrawBack(
                true,
                8,
                113 >> 1, 253 >> 1, 146 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetLobby:
        {
            //Check network state
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (!Network_Connected())
                {
                    //Disconnected
                    menu.next_page = MenuPage_NetFail;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "WAITING FOR HOST",
                SCREEN_WIDTH2,
                SCREEN_HEIGHT2 - 8,
                FontAlign_Center
            );

            //Draw background
            Menu_DrawBack(
                true,
                8,
                253 >> 1, 146 >> 1, 113 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetFail:
        {
            //Return to main menu if circle or start is pressed
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (pad_state.press & (PAD_CIRCLE | PAD_START))
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "DISCONNECTED",
                SCREEN_WIDTH2,
                SCREEN_HEIGHT2 - 8,
                FontAlign_Center
            );

            //Draw background
            Menu_DrawBack(
                true,
                8,
                253 >> 1, 30 >> 1, 15 >> 1,
                0, 0, 0
            );
            break;
        }
        case MenuPage_NetInitFail:
        {
            //Return to main menu if circle or start is pressed
            if (menu.next_page == menu.page && Trans_Idle())
            {
                if (pad_state.press & (PAD_CIRCLE | PAD_START))
                {
                    menu.next_page = MenuPage_Main;
                    menu.next_select = 0;
                    Trans_Start();
                }
            }

            //Draw page label
            menu.font_bold.draw(&menu.font_bold,
                "WSA INIT FAILED",
                SCREEN_WIDTH2,
                SCREEN_HEIGHT2 - 8,
                FontAlign_Center
            );

            //Draw background
            Menu_DrawBack(
                true,
                8,
                253 >> 1, 30 >> 1, 15 >> 1,
                0, 0, 0
            );
            break;
        }
    #endif
        case MenuPage_Stage:
        {
            //Unload menu state
            Menu_Unload();

            //Load new stage
            LoadScr_Start();
            Stage_Load(menu.page_param.stage.id, menu.page_param.stage.diff, menu.page_param.stage.story);
            gameloop = GameLoop_Stage;
            LoadScr_End();
            break;
        }
        default:
            break;
    }

    //Clear page swap flag
    menu.page_swap = menu.page != exec_page;
}
