                                                                                                                                                                                                    /*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_STAGE_H
#define PSXF_GUARD_STAGE_H

#include "io.h"
#include "gfx.h"
#include "pad.h"

#include "fixed.h"
#include "character.h"
#include "player.h"
#include "object.h"
#include "font.h"

#include "network.h"

//Stage constants
#define INPUT_LEFT  (PAD_LEFT  | PAD_SQUARE | PAD_L2)
#define INPUT_DOWN  (PAD_DOWN  | PAD_CROSS | PAD_L1)
#define INPUT_UP    (PAD_UP    | PAD_TRIANGLE | PAD_R1)
#define INPUT_RIGHT (PAD_RIGHT | PAD_CIRCLE | PAD_R2)

#define STAGE_FLAG_JUST_STEP     (1 << 0) //Song just stepped this frame
#define STAGE_FLAG_VOCAL_ACTIVE  (1 << 1) //Song's vocal track is currently active
#define STAGE_FLAG_SCORE_REFRESH (1 << 2) //Score text should be refreshed

#define STAGE_LOAD_PLAYER     (1 << 0) //Reload player character
#define STAGE_LOAD_OPPONENT   (1 << 1) //Reload opponent character
#define STAGE_LOAD_GIRLFRIEND (1 << 2) //Reload girlfriend character
#define STAGE_LOAD_STAGE      (1 << 3) //Reload stage
#define STAGE_LOAD_FLAG       (1 << 7)

typedef struct {
    const char *name;
    int iconIndex;
} Achievement;

//add new ones here
#define NUM_ACHIEVEMENTS 13

typedef enum {
    Full_Combo_Week_1,
    Full_Combo_Week_2,
    Full_Combo_Week_3,
    Full_Combo_Week_4,
    Full_Combo_Week_5,
    Full_Combo_Week_6,
    Full_Combo_Week_7,
    Full_Combo_Senbonzakura,
    Perfect_Any_Song_In_Nightmare_Difficulty,
    ds_User_Turn_On_Low_Quality_Mode,
    Play_Swapped_Mode,
    Play_With_A_Friend,
    Debugger_Enter_The_Debug_Menu
} Achievements;

//Stage enums
typedef enum
{
    StageId_1_1, //Bopeebo
    StageId_1_2, //Fresh
    StageId_1_3, //Dadbattle
    StageId_1_4, //Tutorial
    
    StageId_2_1, //Spookeez
    StageId_2_2, //South
    StageId_2_3, //Monster
    
    StageId_3_1, //Pico
    StageId_3_2, //Philly
    StageId_3_3, //Blammed
    
    StageId_4_1, //Satin Panties
    StageId_4_2, //High
    StageId_4_3, //MILF
    
    StageId_4_4, //Test
    
    StageId_5_1, //Cocoa
    StageId_5_2, //Eggnog
    StageId_5_3, //Winter Horrorland
    
    StageId_6_1, //Senpai
    StageId_6_2, //Roses
    StageId_6_3, //Thorns
    
    StageId_7_1, //Ugh
    StageId_7_2, //Guns
    StageId_7_3, //Stress
    
    StageId_8_1, //Still Alive
    StageId_8_2, //Senbonzakura
    
    StageId_2_4, //Clucked
    
    StageId_Max
} StageId;

#define StageId_LastVanilla StageId_7_3

typedef enum
{
    StageDiff_Easy,
    StageDiff_Normal,
    StageDiff_Hard,
} StageDiff;

typedef enum
{
    StageMode_Normal,
    StageMode_Swap,
    StageMode_2P,
    StageMode_Net1,
    StageMode_Net2,
} StageMode;

typedef enum
{
    StageTrans_Menu,
    StageTrans_NextSong,
    StageTrans_Reload,
    StageTrans_Disconnect,
} StageTrans;

//Stage background
typedef struct StageBack
{
    //Stage background functions
    void (*draw_fg)(struct StageBack*);
    void (*draw_md)(struct StageBack*);
    void (*draw_bg)(struct StageBack*);
    void (*free)(struct StageBack*);
} StageBack;

// Dialogue Struct
typedef struct
{
    const char *text; //The text that is displayed
    u8 camera; //Who the camera is pointing at, 0 for bf, 1 for dad
    u8 portrait;
    int diaboxes;
} Dialogue_Struct;

//Stage definitions
typedef struct
{
    //song name
    const char *pausestage;
    //Characters
    struct
    {
        Character* (*new)();
        fixed_t x, y;
    } pchar, ochar, gchar;
    
    //Stage background
    StageBack* (*back)();
    
    //Song info
    fixed_t speed[3];
    
    u8 week, week_song;
    u8 music_track, music_channel;
    u8 dialogue;
    u8 diasong, dia_channel;
    const char* portrait_path[2];
    
    StageId next_stage;
    u8 next_load;
} StageDef;

//Stage state
#define SECTION_FLAG_OPPFOCUS (1 << 15) //Focus on opponent
#define SECTION_FLAG_BPM_MASK 0x7FFF //1/24

typedef struct
{
    u16 end; //1/12 steps
    u16 flag;
} Section;

#define NOTE_FLAG_OPPONENT    (1 << 2) //Note is opponent's
#define NOTE_FLAG_SUSTAIN     (1 << 3) //Note is a sustain note
#define NOTE_FLAG_SUSTAIN_END (1 << 4) //Is either end of sustain
#define NOTE_FLAG_ALT_ANIM    (1 << 5) //Note plays alt animation
#define NOTE_FLAG_MINE        (1 << 6) //Note is a mine
#define NOTE_FLAG_HIT         (1 << 7) //Note has been hit


typedef struct
{
    u16 pos; //1/12 steps
    u16 type;
} Note;

typedef struct
{
    Character *character;
    
    fixed_t arrow_hitan[4]; //Arrow hit animation for presses
    
    s16 health;
    u16 combo;
    
    boolean refresh_score;
    boolean refresh_misses;
    boolean refresh_pause;
    u8 misses;
    s32 score, max_score;
    char score_text[33];
    char misses_text[20];
    char pause_text[50];
    
    u16 pad_held, pad_press;
} PlayerState;

typedef struct
{
    struct
    {
        //Stage settings
        boolean ghost, downscroll, botplay, lowquality, flashing, expsync;
        boolean hell;
        int menumusic;
        
        s32 mode;
        s32 noteskin;
        
        //Achievements
        u8 unlockedAchievements[NUM_ACHIEVEMENTS];            
    }prefs;
    
    boolean nohud;
    
    char Results_text1[20];
    char Results_text2[20];
    char Results_text3[30];
    
    boolean loadsaveonce;
    
    u32 offset;
    
    u16 grmisses;
    
    const char *storyname;
    
    int pixelmode;
    
    u16 ssmisses;
    
    u16 brmisses;
    
    u16 sickk, goodd, badd, shitt;
    
    u8 hud1shit;
    u8 hud1shit2;
    u8 iconshit;
    
    //HUD textures
    Gfx_Tex tex_hud0, tex_hud1, tex_hud2, tex_results, tex_sprites, tex_saving;
    
    //Stage data
    const StageDef *stage_def;
    StageId stage_id;
    StageDiff stage_diff;
    
    IO_Data chart_data;
    Section *sections;
    Note *notes;
    size_t num_notes;
    
    fixed_t speed;
    fixed_t step_crochet, step_time;
    fixed_t early_safe, late_safe, early_sus_safe, late_sus_safe;
    
    //Stage state
    boolean story;
    u8 flag;
    StageTrans trans;
    
    struct
    {
        fixed_t x, y, zoom;
        fixed_t tx, ty, tz, td;
        fixed_t bzoom;
    } camera;
    fixed_t bump, sbump;
    
    StageBack *back;
    
    Character *player;
    Character *opponent;
    Character *gf;
    
    Section *cur_section; //Current section
    Note *cur_note; //First visible and hittable note, used for drawing and hit detection
    
    fixed_t note_scroll, song_time, interp_time, interp_ms, interp_speed;
    
    u16 last_bpm;
    
    fixed_t time_base;
    u16 step_base;
    Section *section_base;
    
    u8 delect;
    
    //u8 paused;
    
    int cutdia;
    
    boolean paused;
    
    boolean song_completed;
    
    s16 song_step;
    
    u8 gf_speed; //Typically 4 steps, changes in Fresh
    
    PlayerState player_state[2];
    s32 max_score;
    
    u16 misses;
    
    int ending;
    
    int pause_select, pause_scroll;
    
    FontData font_arial, font_cdr, font_bold;
    
    enum
    {
        StageState_Play, //Game is playing as normal
        StageState_Dead,       //Start BREAK animation and reading extra data from CD
        StageState_DeadLoad,   //Wait for said data to be read
        StageState_DeadDrop,   //Mic drop
        StageState_DeadRetry,  //Retry prompt
        StageState_DeadDecide, //Decided
        StageState_Dialogue,   //Dialogue
        StageState_Results,
    } state;
    
    u8 note_swap;
    
    //Object lists
    ObjectList objlist_splash, objlist_fg, objlist_bg;
    
    //Portrait data
    struct
    {
        int current, next;
        u8 tex_id;
        Gfx_Tex tex;
        IO_Data data[2];
    } portrait;
} Stage;

extern Stage stage;

//Stage drawing functions
void Stage_DrawTexCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, u8 r, u8 g, u8 b);
void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom);
void Stage_DrawTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom);
void Stage_BlendTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, u8 mode);

//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story);
void Stage_LoadDia();
void Stage_Unload();
void Stage_Tick();

#ifdef PSXF_NETWORK
void Stage_NetHit(Packet *packet);
void Stage_NetMiss(Packet *packet);
#endif

#endif
