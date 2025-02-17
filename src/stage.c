/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "stage.h"

#include "mem.h"
#include "timer.h"
#include "audio.h"
#include "pad.h"
#include "main.h"
#include "random.h"
#include "movie.h"
#include "network.h"
#include "save.h"
//#include "pause.h"

#include "menu.h"
#include "trans.h"
#include "loadscr.h"
#include "str.h"

#include "object/combo.h"
#include "object/splash.h"
#include "character/ddr.h"

//Stage constants
boolean noheadbump;
u8 cambump;

const char *pausediff = "EASY";
const char *anotherfuckingvarible = "EASY";
//const char *pausestage = "PSYCHIC";
int pausediff2;
int diabox;
int unpausedelay;
int delay;
int sickmove = 304;
int goodmove = 352;
int badmove = 400;
int shitmove =448;
int bffall = -144;
int highcombo;
int ddrmode;

static const RECT thescreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

boolean normo;
//boolean paused;

//Stage constants
//#define STAGE_PERFECT //Play all notes perfectly
//#define STAGE_stage.nohud //Disable the HUD

//Dialogue character portraits

Character *ddr; //DDR arrows
enum
{
	Dialogue_BF_Normal,

	Dialogue_Angy_Senpai,
	Dialogue_Senpai_Normal,
	Dialogue_Spirit,

	Dialogue_Max,
};

static const char** portrait_tims = (const char *[]){
	"bf.tim",
	"senpai.tim",
	NULL,
};

static const struct
{
	u8 tex;
	RECT src;
	POINT pos; //Position
} portraits[] = {
	{0, {  0,   0, 102,  65}, {86, 36}}, //BF normal

	{1, {  0,  48, 64,  58}, {65, 35}}, //Senpai normal
	{1, { 64,  48, 64,  58}, {65, 35}}, //Angy Senpai
	{1, {128,   0, 47, 106}, {65, 35}}, //Spirit
};

//#define STAGE_FREECAM //Freecam
static const fixed_t note_x[8] = {
	//BF
	 FIXED_DEC(26,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(60,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(94,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	FIXED_DEC(-128,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-94,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(-60,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-26,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};
static const fixed_t note_y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);

static const u16 note_key[] = {INPUT_LEFT, INPUT_DOWN, INPUT_UP, INPUT_RIGHT};
static const u8 note_anims[4][3] = {
    {CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
    {CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
    {CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
    {CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//Stage definitions
//boolean stage.nohud;

//Stage definitions
#include "character/bf.h"
#include "character/bfcar.h"
#include "character/bfweeb.h"
#include "character/dad.h"
#include "character/spook.h"
#include "character/monster.h"
#include "character/pico.h"
#include "character/mom.h"
#include "character/xmasbf.h"
#include "character/xmasp.h"
#include "character/monsterx.h"
#include "character/senpai.h"
#include "character/senpaim.h"
#include "character/spirit.h"
#include "character/tank.h"
#include "character/gf.h"
#include "character/xmasgf.h"
#include "character/gfweeb.h"
#include "character/clucky.h"
#include "character/mbf.h"
#include "character/mgf.h"

#include "stage/dummy.h"
#include "stage/week1.h"
#include "stage/week2.h"
#include "stage/week3.h"
#include "stage/week4.h"
#include "stage/week5.h"
#include "stage/week6.h"
#include "stage/week7.h"


static const StageDef stage_defs[StageId_Max] = {
    #include "stagedef_disc1.h"
};

//Stage state
Stage stage;
//sick 0 good 1 bad 2 shit 3 misses 4 score 5 highcombo 6
static int storeresults[7];

//noteskins
static const char *const noteskinPaths[] = {
	    "\\STAGE\\HUD0.TIM;1",
	    "\\STAGE\\HUD2.TIM;1",
	    "\\STAGE\\HUD3.TIM;1",
	    "\\STAGE\\HUD4.TIM;1",
};
//noteskins huds
static const char *const skinshudPaths[] = {
	    "\\STAGE\\HUD1.TIM;1",
	    "\\STAGE\\HUD1.TIM;1",
	    "\\STAGE\\HUDC.TIM;1",
	    "\\STAGE\\HUD1DDR.TIM;1",
};

//Stage music functions
static void Stage_StartVocal(void)
{
    if (!(stage.flag & STAGE_FLAG_VOCAL_ACTIVE))
    {
        Audio_ChannelXA(stage.stage_def->music_channel);
        stage.flag |= STAGE_FLAG_VOCAL_ACTIVE;
    }
}

static void Stage_CutVocal(void)
{
    if (stage.flag & STAGE_FLAG_VOCAL_ACTIVE)
    {
        Audio_ChannelXA(stage.stage_def->music_channel + 1);
        stage.flag &= ~STAGE_FLAG_VOCAL_ACTIVE;
    }
}

//Stage camera functions
static void Stage_FocusCharacter(Character *ch, fixed_t div)
{
    //Use character focus settings to update target position and zoom
    stage.camera.tx = ch->x + ch->focus_x;
    stage.camera.ty = ch->y + ch->focus_y;
    stage.camera.tz = ch->focus_zoom;
    stage.camera.td = div;
}

static void Stage_ScrollCamera(void)
{
    #ifdef STAGE_FREECAM
        if (pad_state.held & PAD_LEFT)
            stage.camera.x -= FIXED_DEC(2,1);
        if (pad_state.held & PAD_UP)
            stage.camera.y -= FIXED_DEC(2,1);
        if (pad_state.held & PAD_RIGHT)
            stage.camera.x += FIXED_DEC(2,1);
        if (pad_state.held & PAD_DOWN)
            stage.camera.y += FIXED_DEC(2,1);
        if (pad_state.held & PAD_TRIANGLE)
            stage.camera.zoom -= FIXED_DEC(1,100);
        if (pad_state.held & PAD_CROSS)
            stage.camera.zoom += FIXED_DEC(1,100);
    #else
        //Get delta position
        fixed_t dx = stage.camera.tx - stage.camera.x;
        fixed_t dy = stage.camera.ty - stage.camera.y;
        fixed_t dz = stage.camera.tz - stage.camera.zoom;

        //Scroll based off current divisor
        stage.camera.x += FIXED_MUL(dx, stage.camera.td);
        stage.camera.y += FIXED_MUL(dy, stage.camera.td);
        stage.camera.zoom += FIXED_MUL(dz, stage.camera.td);
    #endif

	stage.camera.x += FIXED_MUL(dx, stage.camera.td);
	stage.camera.y += FIXED_MUL(dy, stage.camera.td);
	stage.camera.zoom += FIXED_MUL(dz, stage.camera.td);

    //Update other camera stuff
    stage.camera.bzoom = FIXED_MUL(stage.camera.zoom, stage.bump);
}

//Stage section functions
static void Stage_ChangeBPM(u16 bpm, u16 step)
{
    //Update last BPM
    stage.last_bpm = bpm;

    //Update timing base
    if (stage.step_crochet)
        stage.time_base += FIXED_DIV(((fixed_t)step - stage.step_base) << FIXED_SHIFT, stage.step_crochet);
    stage.step_base = step;

    //Get new crochet and times
    stage.step_crochet = ((fixed_t)bpm << FIXED_SHIFT) * 8 / 240; //15/12/24
    stage.step_time = FIXED_DIV(FIXED_DEC(12,1), stage.step_crochet);

    //Get new crochet based values
    stage.early_safe = stage.late_safe = stage.step_crochet / 6; //10 frames
    stage.late_sus_safe = stage.late_safe;
    stage.early_sus_safe = stage.early_safe * 2 / 5;
}

static Section *Stage_GetPrevSection(Section *section)
{
    if (section > stage.sections)
        return section - 1;
    return NULL;
}

static u16 Stage_GetSectionStart(Section *section)
{
    Section *prev = Stage_GetPrevSection(section);
    if (prev == NULL)
        return 0;
    return prev->end;
}

//Section scroll structure
typedef struct
{
    fixed_t start;   //Seconds
    fixed_t length;  //Seconds
    u16 start_step;  //Sub-steps
    u16 length_step; //Sub-steps

    fixed_t size; //Note height
} SectionScroll;

static void Stage_GetSectionScroll(SectionScroll *scroll, Section *section)
{
    //Get BPM
    u16 bpm = section->flag & SECTION_FLAG_BPM_MASK;

    //Get section step info
    scroll->start_step = Stage_GetSectionStart(section);
    scroll->length_step = section->end - scroll->start_step;

    //Get section time length
    scroll->length = (scroll->length_step * FIXED_DEC(15,1) / 12) * 24 / bpm;

    //Get note height
    scroll->size = FIXED_MUL(stage.speed, scroll->length * (12 * 150) / scroll->length_step) + FIXED_UNIT;
}

//Note hit detection
static u8 Stage_HitNote(PlayerState *this, u8 type, fixed_t offset)
{
    //Get hit type
    if (offset < 0)
        offset = -offset;

    u8 hit_type;
    if(stage.prefs.hell)
    {
            if (offset > stage.late_safe * 6 / 11)
            {
                this->refresh_pause = true;
                stage.shitt++;
                hit_type = 3; //SHIT
                this->combo=0;
                this->health -= 1000;
            }
            else if (offset > stage.late_safe * 4 / 11)
            {
                this->refresh_pause = true;
                stage.badd++;
                hit_type = 2; //BAD
                this->combo=0;
                if(stage.prefs.hell==1)
                this->health -= 700;
            }
            else if (offset > stage.late_safe * 2 / 11)
            {
                this->refresh_pause = true;
                stage.goodd++;
                hit_type = 1; //GOOD
            }
            else
            {
                this->refresh_pause = true;
                stage.sickk++;
                hit_type = 0; //SICK
            }
    }
    else
    {
        if (offset > stage.late_safe * 9 / 11)
        {
            this->refresh_pause = true;
            stage.shitt++;
            hit_type = 3; //SHIT
            this->combo=0;
            if(stage.prefs.hell==1)
            this->health -= 700;
        }
        else if (offset > stage.late_safe * 6 / 11)
        {
            this->refresh_pause = true;
            stage.badd++;
            hit_type = 2; //BAD
            this->combo=0;
            if(stage.prefs.hell==1)
            this->health -= 500;
        }
        else if (offset > stage.late_safe * 3 / 11)
        {
            this->refresh_pause = true;
            stage.goodd++;
            hit_type = 1; //GOOD
        }
        else
        {
            this->refresh_pause = true;
            stage.sickk++;
            hit_type = 0; //SICK
        }
    }


    //Increment combo and score
    this->combo++;
    if(this->combo > highcombo)
    	highcombo+=1;

    static const s32 score_inc[] = {
        35, //SICK
        20, //GOOD
        -20, //BAD
         -35, //SHIT
    };
    this->score += score_inc[hit_type];
    this->refresh_score = true;

    //Restore vocals and health
    Stage_StartVocal();
    if(!(hit_type == 3 || hit_type == 2))
	    {
	    if(stage.prefs.hell==1)
	    {
	    		this->health += 200;
	    }
	    else
	    {
	    		this->health += 400;
	    }
    }

    //Create combo object telling of our combo
    Obj_Combo *combo = Obj_Combo_New(
        this->character->focus_x,
        this->character->focus_y,
        hit_type,
        this->combo >= 10 ? this->combo : 0xFFFF
    );
    if (combo != NULL)
        ObjectList_Add(&stage.objlist_fg, (Object*)combo);

    //Create note splashes if SICK
    if(!stage.prefs.onlyarrows)
    {
        if (hit_type == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                //Create splash object
                Obj_Splash *splash = Obj_Splash_New(

                    note_x[type ^ stage.note_swap],
                    note_y * (stage.prefs.downscroll ? -1 : 1),
                    type & 0x3
                );
                if (splash != NULL)
                    ObjectList_Add(&stage.objlist_splash, (Object*)splash);
            }
        }
    }
    else
    {
        if ((hit_type == 0 || hit_type == 1 || hit_type == 2 || hit_type == 3))
        {
            for (int i = 0; i < 3; i++)
            {
                //Create splash object
                Obj_Splash *splash = Obj_Splash_New(

                    note_x[type ^ stage.note_swap],
                    note_y * (stage.prefs.downscroll ? -1 : 1),
                    type & 0x3
                );
                if (splash != NULL)
                    ObjectList_Add(&stage.objlist_splash, (Object*)splash);
            }
        }
    }


	return hit_type;
}

static void Stage_MissNote(PlayerState *this)
{
    if (this->combo)
    {
        //Kill combo
        if (stage.gf != NULL && this->combo > 5)
            stage.gf->set_anim(stage.gf, CharAnim_DownAlt); //Cry if we lost a large combo
        this->combo = 0;

        //Create combo object telling of our lost combo
        Obj_Combo *combo = Obj_Combo_New(
            this->character->focus_x,
            this->character->focus_y,
            0xFF,
            0
        );
        if (combo != NULL)
            ObjectList_Add(&stage.objlist_fg, (Object*)combo);
    }
}

static void Stage_NoteCheck(PlayerState *this, u8 type)
{
    //Perform note check
    for (Note *note = stage.cur_note;; note++)
    {
        if (!(note->type & NOTE_FLAG_MINE))
		{
			//Check if note can be hit
			fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
			if (note_fp - stage.early_safe > stage.note_scroll)
				break;
			if (note_fp + stage.late_safe < stage.note_scroll)
				continue;
			if ((note->type & NOTE_FLAG_HIT) || (note->type & (NOTE_FLAG_OPPONENT | 0x3)) != type || (note->type & NOTE_FLAG_SUSTAIN))
				continue;

			//Hit the note
			note->type |= NOTE_FLAG_HIT;

			this->character->set_anim(this->character, note_anims[type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);
			u8 hit_type = Stage_HitNote(this, type, stage.note_scroll - note_fp);
			this->arrow_hitan[type & 0x3] = stage.step_time;

            #ifdef PSXF_NETWORK
                if (stage.prefs.mode >= StageMode_Net1)
                {
                    //Send note hit packet
                    Packet note_hit;
                    note_hit[0] = PacketType_NoteHit;

                    u16 note_i = note - stage.notes;
                    note_hit[1] = note_i >> 0;
                    note_hit[2] = note_i >> 8;

                    note_hit[3] = this->score >> 0;
                    note_hit[4] = this->score >> 8;
                    note_hit[5] = this->score >> 16;
                    note_hit[6] = this->score >> 24;

                    note_hit[7] = hit_type;

                    note_hit[8] = this->combo >> 0;
                    note_hit[9] = this->combo >> 8;

                    Network_Send(&note_hit);
                }
            #else
                (void)hit_type;
            #endif
            return;
        }
        else
        {
            //Check if mine can be hit
            fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
            if (note_fp - (stage.late_safe * 3 / 5) > stage.note_scroll)
                break;
            if (note_fp + (stage.late_safe * 2 / 5) < stage.note_scroll)
                continue;
            if ((note->type & NOTE_FLAG_HIT) || (note->type & (NOTE_FLAG_OPPONENT | 0x3)) != type || (note->type & NOTE_FLAG_SUSTAIN))
                continue;

            //Hit the mine
		note->type |= NOTE_FLAG_HIT;

            #ifdef PSXF_NETWORK
                if (stage.prefs.mode >= StageMode_Net1)
                {
                    //Send note hit packet
                    Packet note_hit;
                    note_hit[0] = PacketType_NoteHit;

                    u16 note_i = note - stage.notes;
                    note_hit[1] = note_i >> 0;
                    note_hit[2] = note_i >> 8;

                    note_hit[3] = this->score >> 0;
                    note_hit[4] = this->score >> 8;
                    note_hit[5] = this->score >> 16;
                    note_hit[6] = this->score >> 24;

                    /*
                    note_hit[7] = 0xFF;

                    note_hit[8] = this->combo >> 0;
                    note_hit[9] = this->combo >> 8;
                    */

                    Network_Send(&note_hit);
                }
            #endif
            return;
        }
    }
    if (this->character->spec & CHAR_SPEC_MISSANIM)
    {
            this->character->set_anim(this->character, note_anims[type & 0x3][2]);
            this->score -= 1;
        /*if(stage.prefs.hell==1)
		{
			this->health -=1000;
		}*/
            this->refresh_score = true;
    }


    //Missed a note
    this->arrow_hitan[type & 0x3] = -1;
        this->refresh_score = true;

        #ifdef PSXF_NETWORK
            if (stage.prefs.mode >= StageMode_Net1)
            {
                //Send note hit packet
                Packet note_hit;
                note_hit[0] = PacketType_NoteMiss;
                note_hit[1] = type & 0x3;

                note_hit[2] = this->score >> 0;
                note_hit[3] = this->score >> 8;
                note_hit[4] = this->score >> 16;
                note_hit[5] = this->score >> 24;

                Network_Send(&note_hit);
            }
        #endif
}

static void Stage_SustainCheck(PlayerState *this, u8 type)
{
    //Perform note check
    for (Note *note = stage.cur_note;; note++)
    {
        //Check if note can be hit
        fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
        if (note_fp - stage.early_sus_safe > stage.note_scroll)
            break;
        if (note_fp + stage.late_sus_safe < stage.note_scroll)
            continue;
        if ((note->type & NOTE_FLAG_HIT) || (note->type & (NOTE_FLAG_OPPONENT | 0x3)) != type || !(note->type & NOTE_FLAG_SUSTAIN))
            continue;

        //Hit the note
        note->type |= NOTE_FLAG_HIT;

        this->character->set_anim(this->character, note_anims[type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);

        Stage_StartVocal();
        this->score += 20;
        this->refresh_score = true;
        /*if(stage.prefs.hell==1)
        {
        	this->health += 200;
        }
        else
        {
        	this->health += 400;
        }*/
        this->arrow_hitan[type & 0x3] = stage.step_time;

        #ifdef PSXF_NETWORK
            if (stage.prefs.mode >= StageMode_Net1)
            {
                //Send note hit packet
                Packet note_hit;
                note_hit[0] = PacketType_NoteHit;

                u16 note_i = note - stage.notes;
                note_hit[1] = note_i >> 0;
                note_hit[2] = note_i >> 8;

                note_hit[3] = this->score >> 0;
                note_hit[4] = this->score >> 8;
                note_hit[5] = this->score >> 16;
                note_hit[6] = this->score >> 24;

                /*
                note_hit[7] = 0xFF;

                note_hit[8] = this->combo >> 0;
                note_hit[9] = this->combo >> 8;
                */

                Network_Send(&note_hit);
            }
        #endif
    }
}

static void Stage_ProcessPlayer(PlayerState *this, Pad *pad, boolean playing)
{
	//Handle player note presses
	if (stage.prefs.botplay == 0) {
		if (playing)
		{
			u8 i = (this->character == stage.opponent) ? NOTE_FLAG_OPPONENT : 0;

			this->pad_held = this->character->pad_held = pad->held;
			this->pad_press = pad->press;

			if (this->pad_held & INPUT_LEFT)
				Stage_SustainCheck(this, 0 | i);
			if (this->pad_held & INPUT_DOWN)
				Stage_SustainCheck(this, 1 | i);
			if (this->pad_held & INPUT_UP)
				Stage_SustainCheck(this, 2 | i);
			if (this->pad_held & INPUT_RIGHT)
				Stage_SustainCheck(this, 3 | i);

			if (this->pad_press & INPUT_LEFT)
				Stage_NoteCheck(this, 0 | i);
			if (this->pad_press & INPUT_DOWN)
				Stage_NoteCheck(this, 1 | i);
			if (this->pad_press & INPUT_UP)
				Stage_NoteCheck(this, 2 | i);
			if (this->pad_press & INPUT_RIGHT)
				Stage_NoteCheck(this, 3 | i);
		}
		else
		{
			this->pad_held = this->character->pad_held = 0;
			this->pad_press = 0;
		}
	}

	if (stage.prefs.botplay == 1) {
		//Do perfect note checks
		if (playing)
		{
			u8 i = (this->character == stage.opponent) ? NOTE_FLAG_OPPONENT : 0;

			u8 hit[4] = {0, 0, 0, 0};
			for (Note *note = stage.cur_note;; note++)
			{
				//Check if note can be hit
				fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
				if (note_fp - stage.early_safe - FIXED_DEC(12,1) > stage.note_scroll)
					break;
				if (note_fp + stage.late_safe < stage.note_scroll)
					continue;
				if ((note->type & NOTE_FLAG_OPPONENT) != i)
					continue;

				//Handle note hit
				if (!(note->type & NOTE_FLAG_SUSTAIN))
				{
					if (note->type & NOTE_FLAG_HIT)
						continue;
					if (stage.note_scroll >= note_fp)
						hit[note->type & 0x3] |= 1;
					else if (!(hit[note->type & 0x3] & 8))
						hit[note->type & 0x3] |= 2;
				}
				else if (!(hit[note->type & 0x3] & 2))
				{
					if (stage.note_scroll <= note_fp)
						hit[note->type & 0x3] |= 4;
					hit[note->type & 0x3] |= 8;
				}
			}

			//Handle input
			this->pad_held = 0;
			this->pad_press = 0;

			for (u8 j = 0; j < 4; j++)
			{
				if (hit[j] & 5)
				{
					this->pad_held |= note_key[j];
					Stage_SustainCheck(this, j | i);
				}
				if (hit[j] & 1)
				{
					this->pad_press |= note_key[j];
					Stage_NoteCheck(this, j | i);
				}
			}

			this->character->pad_held = this->pad_held;
		}
		else
		{
			this->pad_held = this->character->pad_held = 0;
			this->pad_press = 0;
		}
	}
}


//Stage drawing functions
void Stage_DrawTexCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb)
{
	fixed_t xz = dst->x;
	fixed_t yz = dst->y;
	fixed_t wz = dst->w;
	fixed_t hz = dst->h;

	//Don't draw if HUD and is disabled
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		if (stage.nohud)
			return;
	}

	fixed_t l = (SCREEN_WIDTH2  << FIXED_SHIFT) + FIXED_MUL(xz, zoom);// + FIXED_DEC(1,2);
	fixed_t t = (SCREEN_HEIGHT2 << FIXED_SHIFT) + FIXED_MUL(yz, zoom);// + FIXED_DEC(1,2);
	fixed_t r = l + FIXED_MUL(wz, zoom);
	fixed_t b = t + FIXED_MUL(hz, zoom);

	l >>= FIXED_SHIFT;
	t >>= FIXED_SHIFT;
	r >>= FIXED_SHIFT;
	b >>= FIXED_SHIFT;

	RECT sdst = {
		l,
		t,
		r - l,
		b - t,
	};
	Gfx_DrawTexCol(tex, src, &sdst, cr, cg, cb);
}

void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom)
{
    Stage_DrawTexCol(tex, src, dst, zoom, 0x80, 0x80, 0x80);
}

void Stage_DrawTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom)
{
	//Don't draw if HUD and HUD is disabled
	if (stage.nohud) {
		if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
			return;
	}

	//Get screen-space points
	POINT s0 = {SCREEN_WIDTH2 + (FIXED_MUL(p0->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p0->y, zoom) >> FIXED_SHIFT)};
	POINT s1 = {SCREEN_WIDTH2 + (FIXED_MUL(p1->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p1->y, zoom) >> FIXED_SHIFT)};
	POINT s2 = {SCREEN_WIDTH2 + (FIXED_MUL(p2->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p2->y, zoom) >> FIXED_SHIFT)};
	POINT s3 = {SCREEN_WIDTH2 + (FIXED_MUL(p3->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p3->y, zoom) >> FIXED_SHIFT)};

	Gfx_DrawTexArb(tex, src, &s0, &s1, &s2, &s3);
}

void Stage_BlendTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, u8 mode)
{
	//Don't draw if HUD and HUD is disabled
	if (stage.nohud) {
		if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
			return;
	}

	//Get screen-space points
	POINT s0 = {SCREEN_WIDTH2 + (FIXED_MUL(p0->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p0->y, zoom) >> FIXED_SHIFT)};
	POINT s1 = {SCREEN_WIDTH2 + (FIXED_MUL(p1->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p1->y, zoom) >> FIXED_SHIFT)};
	POINT s2 = {SCREEN_WIDTH2 + (FIXED_MUL(p2->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p2->y, zoom) >> FIXED_SHIFT)};
	POINT s3 = {SCREEN_WIDTH2 + (FIXED_MUL(p3->x, zoom) >> FIXED_SHIFT), SCREEN_HEIGHT2 + (FIXED_MUL(p3->y, zoom) >> FIXED_SHIFT)};

	Gfx_BlendTexArb(tex, src, &s0, &s1, &s2, &s3, mode);
}

//Stage HUD functions
static void Stage_DrawHealth(s16 health, u8 i, s8 ox)
{
    //Check if we should use 'dying' frame
    s8 dying;
    if (ox < 0)
        dying = (health >= 18000) * 32;
    else
        dying = (health <= 2000) * 32;

    //Get src and dst
    fixed_t hx = (128 << FIXED_SHIFT) * (10000 - health) / 10000;
    RECT src = {
        (i % 4) * 64 + dying,
        stage.iconshit + (i / 4) * 32,
        32,
        32
    };
    if (stage.prefs.downscroll)
    {
    RECT_FIXED dst = {
        hx + ox * FIXED_DEC(15,1) - FIXED_DEC(16,1),
        FIXED_DEC(SCREEN_HEIGHT2 - 230, 1),
        src.w << FIXED_SHIFT,
        src.h << FIXED_SHIFT
    };
    //Draw health icon
    Stage_DrawTex(&stage.tex_hud1, &src, &dst, FIXED_MUL(stage.bump, stage.sbump));
    }
    else
    {
    RECT_FIXED dst = {
        hx + ox * FIXED_DEC(15,1) - FIXED_DEC(16,1),
        FIXED_DEC(SCREEN_HEIGHT2 - 35 + 4 - 16, 1),
        src.w << FIXED_SHIFT,
        src.h << FIXED_SHIFT
    };
    //Draw health icon
    Stage_DrawTex(&stage.tex_hud1, &src, &dst, FIXED_MUL(stage.bump, stage.sbump));
    }

}
static void Result_DisplayComboNumber(int number, int x, int y)
{
	//Regular combo numbers
	RECT num_src = {
	    80  + ((number % 5) * 32),
	    160 + ((number / 5) * 32),
	    32,
	    32
	};

	RECT num_dst = {
	    x,
	    y,
	    32,
	    32
	};
	//Pixel combo numbers
	RECT pixelnum_src = {
	    72  + (number * 12),
	    152,
	    11,
	    12
	};

	RECT pixelnum_dst = {
	    x,
	    y,
	    12,
	    12
	};
	if(stage.pixelmode ==1)
		Gfx_DrawTex(&stage.tex_hud0, &pixelnum_src, &pixelnum_dst);
	else
		Gfx_DrawTex(&stage.tex_hud0, &num_src, &num_dst);
}
static void Rating_DisplayComboHits()
{
		//Display First Column
		if(sickmove ==40)
		{
			Result_DisplayComboNumber((storeresults[0] / 1000) % 10, 112, 40);
			Result_DisplayComboNumber((storeresults[1] / 1000) % 10, 112, 72);
			Result_DisplayComboNumber((storeresults[2] / 1000) % 10, 112, 104);
			Result_DisplayComboNumber((storeresults[3] / 1000) % 10, 112, 136);
		}
		//Display Second Column
		if(goodmove ==72)
		{
			Result_DisplayComboNumber((storeresults[0] / 100) % 10, 144, 40);
			Result_DisplayComboNumber((storeresults[1] / 100) % 10, 144, 72);
			Result_DisplayComboNumber((storeresults[2] / 100) % 10, 144, 104);
			Result_DisplayComboNumber((storeresults[3] / 100) % 10, 144, 136);
		}
		//Display Third Column
		if(badmove ==104)
		{
			Result_DisplayComboNumber((storeresults[0] / 10) % 10, 176, 40);
			Result_DisplayComboNumber((storeresults[1] / 10) % 10, 176, 72);
			Result_DisplayComboNumber((storeresults[2] / 10) % 10, 176, 104);
			Result_DisplayComboNumber((storeresults[3] / 10) % 10, 176, 136);
		}
		//Display Forth Column
		if(shitmove ==136)
		{
			Result_DisplayComboNumber(storeresults[0] % 10, 208, 40);
			Result_DisplayComboNumber(storeresults[1] % 10, 208, 72);
			Result_DisplayComboNumber(storeresults[2] % 10, 208, 104);
			Result_DisplayComboNumber(storeresults[3] % 10, 208, 136);
		}
}
static void Stage_DrawStrum(u8 i, RECT *note_src, RECT_FIXED *note_dst)
{
    (void)note_dst;

    PlayerState *this = &stage.player_state[(i & NOTE_FLAG_OPPONENT) != 0];
    i &= 0x3;

    if (this->arrow_hitan[i] > 0)
    {
        //Play hit animation
        u8 frame = ((this->arrow_hitan[i] << 1) / stage.step_time) & 1;
        note_src->x = (i + 1) << 5;
        note_src->y = 64 - (frame << 5);

        this->arrow_hitan[i] -= timer_dt;
        if (this->arrow_hitan[i] <= 0)
        {
            if (this->pad_held & note_key[i])
                this->arrow_hitan[i] = 1;
            else
                this->arrow_hitan[i] = 0;
        }
    }
    else if (this->arrow_hitan[i] < 0)
    {
        //Play depress animation
        note_src->x = (i + 1) << 5;
        note_src->y = 96;
        if (!(this->pad_held & note_key[i]))
            this->arrow_hitan[i] = 0;
    }
    else
    {
        note_src->x = 0;
        note_src->y = i << 5;
    }
}
static void Stage_DrawNotes(void)
{
	//Check if opponent should draw as bot
	u8 bot = (stage.prefs.mode >= StageMode_2P) ? 0 : NOTE_FLAG_OPPONENT;

	//Initialize scroll state
	SectionScroll scroll;
	scroll.start = stage.time_base;

	Section *scroll_section = stage.section_base;
	Stage_GetSectionScroll(&scroll, scroll_section);

	//Push scroll back until cur_note is properly contained
	while (scroll.start_step > stage.cur_note->pos)
	{
		//Look for previous section
		Section *prev_section = Stage_GetPrevSection(scroll_section);
		if (prev_section == NULL)
			break;

		//Push scroll back
		scroll_section = prev_section;
		Stage_GetSectionScroll(&scroll, scroll_section);
		scroll.start -= scroll.length;
	}

	//Draw notes
	for (Note *note = stage.cur_note; note->pos != 0xFFFF; note++)
	{
		//Update scroll
		while (note->pos >= scroll_section->end)
		{
			//Push scroll forward
			scroll.start += scroll.length;
			Stage_GetSectionScroll(&scroll, ++scroll_section);
		}

		//Get note information
		u8 i = (note->type & NOTE_FLAG_OPPONENT) != 0;
		PlayerState *this = &stage.player_state[i];

		fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
		fixed_t time = (scroll.start - stage.song_time) + (scroll.length * (note->pos - scroll.start_step) / scroll.length_step);
		fixed_t y = note_y + FIXED_MUL(stage.speed, time * 150);

		//Check if went above screen
		if (y < FIXED_DEC(-16 - SCREEN_HEIGHT2, 1))
		{
			//Wait for note to exit late time
			if (note_fp + stage.late_safe >= stage.note_scroll)
				continue;

			//Miss note if player's note
			if (!(note->type & (bot | NOTE_FLAG_HIT | NOTE_FLAG_MINE)))
			{
				if (stage.prefs.mode < StageMode_Net1 || i == ((stage.prefs.mode == StageMode_Net1) ? 0 : 1))
				{
					//Missed note
					Stage_CutVocal();
					Stage_MissNote(this);
					if(stage.prefs.hell==1)
					{
						this->health -= 1200;
					}
					else
					{
						this->health -= 1000;
					}
					if (!(note->type & NOTE_FLAG_SUSTAIN))
						stage.misses++;
					this->score -= 1;
					this->refresh_misses=true;

					//Send miss packet
					#ifdef PSXF_NETWORK
						if (stage.prefs.mode >= StageMode_Net1)
						{
							//Send note hit packet
							Packet note_hit;
							note_hit[0] = PacketType_NoteMiss;
							note_hit[1] = 0xFF;

							note_hit[2] = this->score >> 0;
							note_hit[3] = this->score >> 8;
							note_hit[4] = this->score >> 16;
							note_hit[5] = this->score >> 24;

							Network_Send(&note_hit);
						}
					#endif
				}
			}

			//Update current note
			stage.cur_note++;
		}
		else
		{
			//Don't draw if below screen
			RECT note_src;
			RECT_FIXED note_dst;
			if (y > (FIXED_DEC(SCREEN_HEIGHT,2) + scroll.size) || note->pos == 0xFFFF)
				break;

			//Draw note
			if (note->type & NOTE_FLAG_SUSTAIN)
			{
				//Check for sustain clipping
				fixed_t clip;
				y -= scroll.size;
				if ((note->type & (bot | NOTE_FLAG_HIT)) || ((this->pad_held & note_key[note->type & 0x3]) && (note_fp + stage.late_sus_safe >= stage.note_scroll)))
				{
					clip = FIXED_DEC(32 - SCREEN_HEIGHT2, 1) - y;
					if (clip < 0)
						clip = 0;
				}
				else
				{
					clip = 0;
				}

				//Draw sustain
				if (note->type & NOTE_FLAG_SUSTAIN_END)
				{
					if (clip < (24 << FIXED_SHIFT))
					{
						note_src.x = 160;
						note_src.y = ((note->type & 0x3) << 5) + 4 + (clip >> FIXED_SHIFT);
						note_src.w = 32;
						note_src.h = 28 - (clip >> FIXED_SHIFT);

						note_dst.x = note_x[(note->type & 0x7) ^ stage.note_swap] - FIXED_DEC(16,1);
						note_dst.y = y + clip;
						note_dst.w = note_src.w << FIXED_SHIFT;
						note_dst.h = (note_src.h << FIXED_SHIFT);

						if (stage.prefs.downscroll)
						{
							note_dst.y = -note_dst.y;
							note_dst.h = -note_dst.h;
						}
						if(stage.bluenotes)
							Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
						else
							Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);
					}
				}
				else
				{
					//Get note height
					fixed_t next_time = (scroll.start - stage.song_time) + (scroll.length * (note->pos + 12 - scroll.start_step) / scroll.length_step);
					fixed_t next_y = note_y + FIXED_MUL(stage.speed, next_time * 150) - scroll.size;
					fixed_t next_size = next_y - y;

					if (clip < next_size)
					{
						note_src.x = 160;
						note_src.y = ((note->type & 0x3) << 5);
						note_src.w = 32;
						note_src.h = 16;

						note_dst.x = note_x[(note->type & 0x7) ^ stage.note_swap] - FIXED_DEC(16,1);
						note_dst.y = y + clip;
						note_dst.w = note_src.w << FIXED_SHIFT;
						note_dst.h = (next_y - y) - clip;

						if (stage.prefs.downscroll)
							note_dst.y = -note_dst.y - note_dst.h;
						if(stage.bluenotes)
							Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
						else
							Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);
					}
				}
			}
			else if (note->type & NOTE_FLAG_MINE)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note shine

				note_src.x = 160 + ((animf_count & 0x1) << 4);
				note_src.y = 128;
				note_src.w = 32;
				note_src.h = 32;

				note_dst.x = note_x[(note->type & 0x7) ^ stage.note_swap] - FIXED_DEC(16,1);
				note_dst.y = y - FIXED_DEC(16,1);
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;

				if(stage.bluenotes)
					Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
				else
					Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);

				//Draw note body
				note_src.x = 192 + ((note->type & 0x1) << 5);
				note_src.y = (note->type & 0x2) << 4;
				note_src.w = 32;
				note_src.h = 32;

				note_dst.x = note_x[(note->type & 0x7) ^ stage.note_swap] - FIXED_DEC(16,1);
				note_dst.y = y - FIXED_DEC(16,1);
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				if(stage.bluenotes)
					Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
				else
					Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);
			}
			else
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note
				note_src.x = 32 + ((note->type & 0x3) << 5);
				note_src.y = 0;
				note_src.w = 32;
				note_src.h = 32;

				note_dst.x = note_x[(note->type & 0x7) ^ stage.note_swap] - FIXED_DEC(16,1);
				note_dst.y = y - FIXED_DEC(16,1);
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				if(stage.bluenotes)
					Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
				else
					Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);
			}

		}
	}
}
//Stage loads
static void Stage_SwapChars(void)
{
    if (stage.prefs.mode == StageMode_Swap)
    {
        Character *temp = stage.player;
        stage.player = stage.opponent;
        stage.opponent = temp;
    }
}

static void Stage_LoadPlayer(void)
{
    //Load player character
    Character_Free(stage.player);
    stage.player = stage.stage_def->pchar.new(stage.stage_def->pchar.x, stage.stage_def->pchar.y);
}

static void Stage_LoadOpponent(void)
{
    //Load opponent character
    Character_Free(stage.opponent);
    stage.opponent = stage.stage_def->ochar.new(stage.stage_def->ochar.x, stage.stage_def->ochar.y);
}

static void Stage_LoadGirlfriend(void)
{
    //Load girlfriend character
    Character_Free(stage.gf);
    if (stage.stage_def->gchar.new != NULL)
        stage.gf = stage.stage_def->gchar.new(stage.stage_def->gchar.x, stage.stage_def->gchar.y);
    else
        stage.gf = NULL;
}

static void Stage_LoadStage(void)
{
    //Load back
    if (stage.back != NULL)
        stage.back->free(stage.back);
    stage.back = stage.stage_def->back();
}

static void Stage_LoadChart(void)
{
	//Load stage data
	char chart_path[64];

		//Use standard path convention
		sprintf(chart_path, "\\WEEK%d\\%d.%d%c.CHT;1", stage.stage_def->week, stage.stage_def->week, stage.stage_def->week_song, "ENH"[stage.stage_diff]);

	if (stage.chart_data != NULL)
		Mem_Free(stage.chart_data);
	stage.chart_data = IO_Read(chart_path);
	u8 *chart_byte = (u8*)stage.chart_data;

	#ifdef PSXF_PC
		//Get lengths
		u16 note_off = chart_byte[0] | (chart_byte[1] << 8);

		u8 *section_p = chart_byte + 2;
		u8 *note_p = chart_byte + note_off;

		u8 *section_pp;
		u8 *note_pp;

		size_t sections = (note_off - 2) >> 2;
		size_t notes = 0;

		for (note_pp = note_p;; note_pp += 4)
		{
			notes++;
			u16 pos = note_pp[0] | (note_pp[1] << 8);
			if (pos == 0xFFFF)
				break;
		}

		if (notes)
			stage.num_notes = notes - 1;
		else
			stage.num_notes = 0;

		//Realloc for separate structs
		size_t sections_size = sections * sizeof(Section);
		size_t notes_size = notes * sizeof(Note);
		size_t notes_off = MEM_ALIGN(sections_size);

		u8 *nchart = Mem_Alloc(notes_off + notes_size);

		Section *nsection_p = stage.sections = (Section*)nchart;
		section_pp = section_p;
		for (size_t i = 0; i < sections; i++, section_pp += 4, nsection_p++)
		{
			nsection_p->end = section_pp[0] | (section_pp[1] << 8);
			nsection_p->flag = section_pp[2] | (section_pp[3] << 8);
		}

		Note *nnote_p = stage.notes = (Note*)(nchart + notes_off);
		note_pp = note_p;
		for (size_t i = 0; i < notes; i++, note_pp += 4, nnote_p++)
		{
			nnote_p->pos = note_pp[0] | (note_pp[1] << 8);
			nnote_p->type = note_pp[2] | (note_pp[3] << 8);
		}

		//Use reformatted chart
		Mem_Free(stage.chart_data);
		stage.chart_data = (IO_Data)nchart;
	#else
		//Directly use section and notes pointers
		stage.sections = (Section*)(chart_byte + 2);
		stage.notes = (Note*)(chart_byte + *((u16*)stage.chart_data));

		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
			stage.num_notes++;
	#endif

	//Swap chart
	if (stage.prefs.mode == StageMode_Swap)
	{
		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
			note->type ^= NOTE_FLAG_OPPONENT;
		for (Section *section = stage.sections;; section++)
		{
			section->flag ^= SECTION_FLAG_OPPFOCUS;
			if (section->end == 0xFFFF)
				break;
		}
	}

	//Count max scores
	stage.player_state[0].max_score = 0;
	stage.player_state[1].max_score = 0;
	for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
	{
		if (note->type & (NOTE_FLAG_SUSTAIN | NOTE_FLAG_MINE))
			continue;
		if (note->type & NOTE_FLAG_OPPONENT)
			stage.player_state[1].max_score += 35;
		else
			stage.player_state[0].max_score += 35;
	}
	if (stage.prefs.mode >= StageMode_2P && stage.player_state[1].max_score > stage.player_state[0].max_score)
		stage.max_score = stage.player_state[1].max_score;
	else
		stage.max_score = stage.player_state[0].max_score;

	stage.cur_section = stage.sections;
	stage.cur_note = stage.notes;

	stage.speed = stage.stage_def->speed[stage.stage_diff];

	stage.step_crochet = 0;
	stage.time_base = 0;
	stage.step_base = 0;
	stage.section_base = stage.cur_section;
	Stage_ChangeBPM(stage.cur_section->flag & SECTION_FLAG_BPM_MASK, 0);
}

static void Stage_LoadMusic(void)
{
    //Offset sing ends
    stage.player->sing_end -= stage.note_scroll;
    stage.opponent->sing_end -= stage.note_scroll;
    if (stage.gf != NULL)
        stage.gf->sing_end -= stage.note_scroll;

    //Find music file and begin seeking to it
    Audio_SeekXA_Track(stage.stage_def->music_track);

    //Initialize music state
    stage.note_scroll = FIXED_DEC(-5 * 4 * 12,1);
    stage.song_time = FIXED_DIV(stage.note_scroll, stage.step_crochet);
    stage.interp_time = 0;
    stage.interp_ms = 0;
    stage.interp_speed = 0;

    //Offset sing ends again
    stage.player->sing_end += stage.note_scroll;
    stage.opponent->sing_end += stage.note_scroll;
    if (stage.gf != NULL)
        stage.gf->sing_end += stage.note_scroll;
}

static void Stage_LoadState(void)
{
    //Initialize stage state
    stage.flag = STAGE_FLAG_VOCAL_ACTIVE;

    stage.gf_speed = 1 << 2;

    //check if song has dialogue
    if (stage.story)
    {
        printf("Checking for dialogue...\n");
        if (stage.stage_def->dialogue == 1)
        {
            printf("Dialogue found!\n");
            Stage_LoadDia();
            stage.state = StageState_Dialogue;
        }
        else
        {
            printf("Dialogue not found.\n");
            stage.state = StageState_Play;
        }
    }
    else
    {
        stage.state = StageState_Play;
    }

    stage.player_state[0].character = stage.player;
    stage.player_state[1].character = stage.opponent;
    for (int i = 0; i < 2; i++)
    {
        memset(stage.player_state[i].arrow_hitan, 0, sizeof(stage.player_state[i].arrow_hitan));

        stage.player_state[i].health = 10000;

	stage.player_state[i].refresh_score = true;
	stage.player_state[i].refresh_misses = false;
	stage.player_state[i].refresh_pause = true;
	stage.player_state[i].combo = 0;
	stage.player_state[i].score = 0;
	strcpy(stage.player_state[i].score_text, "0");
	strcpy(stage.player_state[i].misses_text, "0");
	strcpy(stage.player_state[i].pause_text, "0");

        stage.delect = 0;

        stage.player_state[i].pad_held = stage.player_state[i].pad_press = 0;
    }

    ObjectList_Free(&stage.objlist_splash);
    ObjectList_Free(&stage.objlist_fg);
    ObjectList_Free(&stage.objlist_bg);

    stage.prefs.ghost = true;
    noheadbump = 0;
    //stage.nohud=0;

    normo = false;
    stage.paused = false;
    stage.misses = 0;
    stage.sickk=0;
    stage.goodd=0;
    stage.badd=0;
    stage.shitt=0;

    if(!stage.story)
    {
    	for (int i = 0; i < 7; i++)
        	storeresults[i] =0;
    }

    //reset results screen movement
    sickmove = 304;
    goodmove = 352;
    badmove = 400;
    shitmove =448;
    bffall = -144;
    highcombo =0;
    stage.bluenotes=0;
    //stage.nohud=1;

	if(stage.stage_diff == StageDiff_Easy){pausediff = "EASY";}
	if(stage.stage_diff == StageDiff_Normal){pausediff = "NORMAL";}
	if(stage.stage_diff == StageDiff_Hard){pausediff = "HARD";}

	switch(stage.stage_diff)
	    {
	    	case StageDiff_Easy:
	    	{
	    		stage.stage_diff = StageDiff_Easy;
	    		anotherfuckingvarible = "EASY";
	    		pausediff2=0;
	    		break;
	    	}
	    	case StageDiff_Normal:
	    	{
	    		stage.stage_diff = StageDiff_Normal;
	    		anotherfuckingvarible = "NORMAL";
	    		pausediff2=1;
	    		break;
	    	}
	    	case StageDiff_Hard:
	    	{
	    		stage.stage_diff = StageDiff_Hard;
	    		anotherfuckingvarible = "HARD";
	    		pausediff2=2;
	    		break;
	    	}
	    }

    FontData_Load(&stage.font_bold, Font_Bold);
    FontData_Load(&stage.font_cdr, Font_CDR);
}
static void Menu_DrawBot(s32 x, s32 y)
{
    //Draw Track
    RECT bot_src = {189, 240, 67, 16};
    Gfx_BlitTex(&stage.tex_hud0, &bot_src, x, y);
}
static void Stage_DrawSickForResults(s32 x, s32 y)
{
    //Draw Track
    RECT res_src = {0, 128, 80, 32};
    RECT pres_src = {0, 128, 64, 24};

    if(stage.pixelmode == 1)
    	Gfx_BlitTex(&stage.tex_hud0, &pres_src, x, y);
    else
    	Gfx_BlitTex(&stage.tex_hud0, &res_src, x, y);
}
static void Stage_DrawGoodForResults(s32 x, s32 y)
{
    //Draw Track
    RECT res1_src = {0, 160, 80, 32};
    RECT pres1_src = {0, 152, 64, 24};

    if(stage.pixelmode == 1)
    	Gfx_BlitTex(&stage.tex_hud0, &pres1_src, x, y);
    else
    	Gfx_BlitTex(&stage.tex_hud0, &res1_src, x, y);
}
static void Stage_DrawBadForResults(s32 x, s32 y)
{
    //Draw Track
    RECT res2_src = {0, 192, 80, 32};
    RECT pres2_src = {0, 176, 64, 24};

    if(stage.pixelmode == 1)
    	Gfx_BlitTex(&stage.tex_hud0, &pres2_src, x, y);
    else
    	Gfx_BlitTex(&stage.tex_hud0, &res2_src, x, y);
}
static void Stage_DrawShitForResults(s32 x, s32 y)
{
    //Draw Track
    RECT res3_src = {0, 224, 80, 32};
    RECT pres3_src = {0, 200, 64, 24};

    if(stage.pixelmode == 1)
    	Gfx_BlitTex(&stage.tex_hud0, &pres3_src, x, y);
    else
    	Gfx_BlitTex(&stage.tex_hud0, &res3_src, x, y);
}
static void Stage_DrawSavingIcon(u8 i, s16 x, s16 y)
{
    //save Size
    u8 save_size = 32;

    //Get src and dst
    RECT save_src = {
        (i % 4) * save_size,
        (i / 4) * save_size,
        save_size,
        save_size
    };
    RECT save_dst = {
        x,
        y,
        32,
        32
    };
    //save_dst.w = save_dst.w * 2;
    //save_dst.h = save_dst.h * 2;
    //Draw dia save
    Gfx_DrawTex(&stage.tex_hud1, &save_src, &save_dst);
}
//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story)
{
    //Get stage definition
    stage.stage_def = &stage_defs[stage.stage_id = id];
    stage.stage_diff = difficulty;
    stage.story = story;
    stage.song_completed = false;
    //Load HUD textures
    if(stage.stage_id == StageId_8_3)
    {
    	Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\SILYHUD0.TIM;1"), GFX_LOADTEX_FREE);
    	Gfx_LoadTex(&stage.tex_blue, IO_Read("\\STAGE\\BLUEHUD.TIM;1"), GFX_LOADTEX_FREE);
	Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\SILYHUD1.TIM;1"), GFX_LOADTEX_FREE);
	stage.hud1shit =50;
	stage.hud1shit2 =21;
	stage.hud1shit3 =160;
	stage.iconshit =160;
	stage.pixelmode =0;
	ddrmode=0;

    }
    else
    {
	    if(stage.stage_id == StageId_8_1)
	    {
		Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\PHUD.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\PHUD1.TIM;1"), GFX_LOADTEX_FREE);
		stage.hud1shit =8;
		stage.hud1shit2 =0;
		stage.hud1shit3 =256;
		stage.iconshit =16;
		stage.pixelmode =0;
		ddrmode=0;
	    }
	    else
	    {
		if (id >= StageId_6_1 && id <= StageId_6_3)
		{
			Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0WEEB.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\PIXELHUD.TIM;1"), GFX_LOADTEX_FREE);
			stage.hud1shit =8;
			stage.hud1shit2 =0;
			stage.hud1shit3 =256;
			stage.iconshit =16;
			stage.pixelmode =1;
			ddrmode=0;
		}
		else
		{
			stage.pixelmode =0;
			if(!(stage.prefs.noteskin ==3))
			{
				ddrmode=0;
				stage.hud1shit =8;
				stage.hud1shit2 =0;
				stage.hud1shit3 =256;
				stage.iconshit =16;
			}
			else
			{
                ddr = Char_DDR_New(FIXED_DEC(500,1), FIXED_DEC(500,1));
				stage.hud1shit =20;
				stage.hud1shit2 =6;
				stage.hud1shit3 =256;
				stage.iconshit =48;
				ddrmode=1;
			}
			Gfx_LoadTex(&stage.tex_hud0, IO_Read(noteskinPaths[stage.prefs.noteskin]), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hud1, IO_Read(skinshudPaths[stage.prefs.noteskin]), GFX_LOADTEX_FREE);
		}
	    }
    }

    // Dont play str if song has been reloaded
    if (stage.trans != StageTrans_Reload)
    {
        Str_CanPlayBegin();
    }
    //Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\HUD1.TIM;1"), GFX_LOADTEX_FREE);

    //Load stage background
    Stage_LoadStage();

    //Load characters
    Stage_LoadPlayer();
    Stage_LoadOpponent();
    Stage_LoadGirlfriend();
    Stage_SwapChars();

    //Load stage chart
    Stage_LoadChart();

    //Initialize stage state
    stage.story = story;

    Stage_LoadState();

    //Initialize camera
    if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
        Stage_FocusCharacter(stage.opponent, FIXED_UNIT);
    else
        Stage_FocusCharacter(stage.player, FIXED_UNIT);
    stage.camera.x = stage.camera.tx;
    stage.camera.y = stage.camera.ty;
    stage.camera.zoom = stage.camera.tz;

    stage.bump = FIXED_UNIT;
    stage.sbump = FIXED_UNIT;

    //Initialize stage according to mode
    stage.note_swap = (stage.prefs.mode == StageMode_Swap) ? 4 : 0;

    //Load music
    stage.note_scroll = 0;
    Stage_LoadMusic();

    //Test offset
    stage.offset = 0;

    #ifdef PSXF_NETWORK
    if (stage.prefs.mode >= StageMode_Net1 && Network_IsHost())
    {
        //Send ready packet to peer
        Packet ready;
        ready[0] = PacketType_Ready;
        ready[1] = id;
        ready[2] = difficulty;
        ready[3] = (stage.prefs.mode == StageMode_Net1) ? 1 : 0;
        Network_Send(&ready);
    }
    #endif
}

void Stage_Unload(void)
{
    //Disable net mode to not break the game
    if (stage.prefs.mode >= StageMode_Net1)
        stage.prefs.mode = StageMode_Normal;

    //Unload stage background
    if (stage.back != NULL)
        stage.back->free(stage.back);
    stage.back = NULL;

    //Unload stage data
    Mem_Free(stage.chart_data);
    stage.chart_data = NULL;

    //Free objects
    ObjectList_Free(&stage.objlist_splash);
    ObjectList_Free(&stage.objlist_fg);
    ObjectList_Free(&stage.objlist_bg);

    //Free characters
    Character_Free(stage.player);
    stage.player = NULL;
    Character_Free(stage.opponent);
    stage.opponent = NULL;
    Character_Free(stage.gf);
    stage.gf = NULL;
    Character_Free(ddr);
}

static boolean Stage_NextLoad(void)
{
    u8 load = stage.stage_def->next_load;
    if (load == 0)
    {
        //Do stage transition if full reload
        stage.trans = StageTrans_NextSong;
        Trans_Start();
        return false;
    }
    else
    {
        //Get stage definition
        stage.stage_def = &stage_defs[stage.stage_id = stage.stage_def->next_stage];

	Str_CanPlayBegin();

        stage.song_completed = false;

        //Load stage background
        if (load & STAGE_LOAD_STAGE)
            Stage_LoadStage();

        //Load characters
        Stage_SwapChars();
        if (load & STAGE_LOAD_PLAYER)
        {
            Stage_LoadPlayer();
        }
        else
        {
            stage.player->x = stage.stage_def->pchar.x;
            stage.player->y = stage.stage_def->pchar.y;
        }
        if (load & STAGE_LOAD_OPPONENT)
        {
            Stage_LoadOpponent();
        }
        else
        {
            stage.opponent->x = stage.stage_def->ochar.x;
            stage.opponent->y = stage.stage_def->ochar.y;
        }
        Stage_SwapChars();
        if (load & STAGE_LOAD_GIRLFRIEND)
        {
            Stage_LoadGirlfriend();
        }
        else if (stage.gf != NULL)
        {
            stage.gf->x = stage.stage_def->gchar.x;
            stage.gf->y = stage.stage_def->gchar.y;
        }

        //Load stage chart
        Stage_LoadChart();

        //Initialize stage state
        Stage_LoadState();

        //Load music
        Stage_LoadMusic();

        //Reset timer
        Timer_Reset();
        return true;
    }
}
void Stage_LoadDia(void)
{
	for (u8 i = 0; i < COUNT_OF(stage.portrait.data); i++)
		stage.portrait.data[i] = IO_Read(stage.stage_def->portrait_path[i]);

    //Gfx_LoadTex(&stage.tex_dia, IO_Read("\\STAGE\\DIA.TIM;1"), GFX_LOADTEX_FREE);

	stage.portrait.current = stage.portrait.next = 0xFF;
	stage.portrait.tex_id = 0xFF;

    FontData_Load(&stage.font_arial, Font_Arial);
}

//Unload dialogue related files
void Stage_UnLoadDia(void)
{
	for (u8 i = 0; i < COUNT_OF(stage.portrait.data); i++)
	{
		//Free portrait art
		Mem_Free(stage.portrait.data[i]);
		stage.portrait.data[i] = NULL;
	}
}
void Stage_LoadResults(void)
{
	Gfx_LoadTex(&stage.tex_results, IO_Read("\\STAGE\\RESULTS.TIM;1"), GFX_LOADTEX_FREE);
	Gfx_LoadTex(&stage.tex_sprites, IO_Read("\\STAGE\\SPRITES.TIM;1"), GFX_LOADTEX_FREE);
	//Play results music
	Audio_StopXA();
	Audio_PlayXA_Track(XA_Results, 0x40, 2, 1);
	Audio_WaitPlayXA();
}
void Stage_Tick(void)
{
    SeamLoad:;

    //Tick transition
    #ifdef PSXF_NETWORK
    if (stage.prefs.mode >= StageMode_Net1)
    {
        //Show disconnect screen when disconnected
        if (!(Network_Connected() && Network_HasPeer()))
        {
            stage.trans = StageTrans_Disconnect;
            Trans_Start();
        }
    }
    else
    #endif
    {
        //Return to menu when start is pressed
        /*if (stage.state != StageState_Dialogue){
        if (pad_state.press & PAD_START)
        {
            stage.trans = (stage.state == StageState_Play) ? StageTrans_Menu : StageTrans_Reload;
            Trans_Start();
        }
        }*/
    }

    if (Trans_Tick())
    {
        switch (stage.trans)
        {
            case StageTrans_Menu:
                //Load appropriate menu
                Stage_Unload();

                LoadScr_Start();
                #ifdef PSXF_NETWORK
                if (Network_Connected())
                {
                    if (Network_IsHost())
                        Menu_Load(MenuPage_NetOp);
                    else
                        Menu_Load(MenuPage_NetLobby);
                }
                else
                #endif
                {
                    if (stage.stage_id <= StageId_LastVanilla)
                    {
                        if (stage.story)
                            Menu_Load(MenuPage_Story);
                        else
                            Menu_Load(MenuPage_Freeplay);
                    }
                    else
                    {
                        Menu_Load(MenuPage_Main);
                    }
                }
                LoadScr_End();

                gameloop = GameLoop_Menu;
                return;
            case StageTrans_NextSong:
                //Load next song
                Stage_Unload();

                LoadScr_Start();
                Stage_Load(stage.stage_def->next_stage, stage.stage_diff, stage.story);
                LoadScr_End();
                break;
            case StageTrans_Reload:
                //Reload song
                Stage_Unload();

                LoadScr_Start();
                Stage_Load(stage.stage_id, stage.stage_diff, stage.story);
                LoadScr_End();
                break;
            case StageTrans_Disconnect:
        #ifdef PSXF_NETWORK
                //Disconnect screen
                Stage_Unload();

                LoadScr_Start();
                if (Network_Connected() && Network_IsHost())
                    Menu_Load(MenuPage_NetOpWait);
                else
                    Menu_Load(MenuPage_NetFail);
                LoadScr_End();

                gameloop = GameLoop_Menu;
        #endif
                return;
        }
    }

    switch (stage.state)
    {
        case StageState_Play:
        {
        	if(ddrmode==1)
        	{
			//Draw ddr arrows
		    	ddr->tick(ddr);
            	}
            	//printf(stage.song_step);

            	//Two Player Achievement
        	if(stage.prefs.mode == StageMode_2P && pad_state_2.press & PAD_CROSS)
					Achievement_Unlock(Play_With_A_Friend);
        	stage.cutdia=0;

        	if(stage.paused == false && Audio_PlayingXA())
        	{
			if (pad_state.press & PAD_START)
			{
			    stage.pause_select = 0;
			    unpausedelay=6;
			    delay=5;
			    stage.paused =true;
			}
		}
		for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];

				if(stage.prefs.mode == StageMode_2P)
						{
							if(stage.paused ==true && !stage.prefs.onlyarrows)
								stage.font_cdr.draw(&stage.font_cdr, this->pause_text, (i ^ (stage.prefs.mode == StageMode_Swap)) ? 170 - 80 : 170 + 80, 35, FontAlign_Center);
						}
						else
						{
							if(stage.paused ==true && !stage.prefs.onlyarrows)
								stage.font_cdr.draw(&stage.font_cdr, this->pause_text, 4, 18, FontAlign_Left);
						}
			}
	            if (stage.paused ==true)
			{
			    switch(pausediff2)
			    {
			    	case 0:
			    	{
			    		anotherfuckingvarible = "EASY";
			    		break;
			    	}
			    	case 1:
			    	{
			    		anotherfuckingvarible = "NORMAL";
			    		break;
			    	}
			    	case 2:
			    	{
			    		anotherfuckingvarible = "HARD";
			    		break;
			    	}
			    }

			    Audio_PauseXA();
			    static const char *stage_options[] = {
				"RESUME",
				"RESTART SONG",
				"CHANGE DIFFICULTY",
				"EXIT TO MENU"
			      };
			 //Draw stage
			    stage.font_bold.draw(&stage.font_bold,
				stage.stage_def->pausestage,
				316,
				18,
				FontAlign_Right
			    );
			 //Draw diff
			    stage.font_bold.draw(&stage.font_bold,
				pausediff,
				316,
				39,
				FontAlign_Right
			    );
			 //Change option
		        if (pad_state.press & PAD_UP)
		        {
		            if (stage.pause_select > 0)
		                stage.pause_select--;
		            else
		                stage.pause_select = 3;
		        }
		        if (pad_state.press & PAD_DOWN)
		        {
		            if (stage.pause_select < 3)
		                stage.pause_select++;
		            else
		                stage.pause_select = 0;
		        }
		        if(delay<=-1)
		        {
			  //Select option if cross or start is pressed
			    switch (stage.pause_select)
			    {
			      case 0: //Resume
			        if (pad_state.press & (PAD_START | PAD_CIRCLE))
			  	{
					unpausedelay=5;
				}
				if(unpausedelay>=0 && unpausedelay<=5)
				    {
				    	unpausedelay--;
				    }
				    if(unpausedelay<=-1)
				    {
				    	stage.paused = false;
					Audio_ResumeXA();
				    }
				break;
			      case 1: //Retry
			        if (pad_state.press & (PAD_START | PAD_CIRCLE))
			  	{
					stage.trans = StageTrans_Reload;
					Trans_Start();
					for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
				}
				break;
			      case 2: //Change difficulty
			        if (pad_state.press & PAD_LEFT)
				{
				    if (pausediff2 > 0)
					pausediff2--;
				    else
					pausediff2 = 2;
				}
				if (pad_state.press & PAD_RIGHT)
				{
				    if (pausediff2 < 2)
					pausediff2++;
				    else
					pausediff2 = 0;
				}
				if (pad_state.press & (PAD_START | PAD_CIRCLE))
			  	{
			  		if(pausediff2==0)
			  			stage.stage_diff = StageDiff_Easy;
			  		if(pausediff2==1)
			  			stage.stage_diff = StageDiff_Normal;
			  		else if(pausediff2==2)
			  			stage.stage_diff = StageDiff_Hard;
			  		stage.trans = StageTrans_Reload;
					Trans_Start();
					for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
			  	}
			  	//Draw stage
				    stage.font_bold.draw(&stage.font_bold,
					anotherfuckingvarible,
					160,
				        166,
					FontAlign_Center
				    );
				break;
			      case 3: //Quit
			        if (pad_state.press & (PAD_START | PAD_CIRCLE))
			  	{
					stage.trans = StageTrans_Menu;
					Trans_Start();
					for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
				}
				break;
			    }
			  }
			  else
			  {
			  delay--;
			  }
			  s32 ext_scroll = stage.pause_select * FIXED_DEC(12,1);
			  stage.pause_scroll += (ext_scroll - stage.pause_scroll) >> 2;
			  //Draw all options
				for (u8 i = 0; i < COUNT_OF(stage_options); i++)
				{
				    stage.font_bold.draw_col(&stage.font_bold,
				    stage_options[i],
				        SCREEN_WIDTH2,
				        SCREEN_HEIGHT2 + (i << 5) - 48 - (stage.pause_scroll >> FIXED_SHIFT),
				        FontAlign_Center,
				        (stage.pause_select == i) ? 128 : 64,
					(stage.pause_select == i) ? 128 : 64,
					(stage.pause_select == i) ? 128 : 64
				    );
				}
			  //50% Blend
			  RECT screen_src = {0, 0 ,SCREEN_WIDTH, SCREEN_HEIGHT};

			  Gfx_BlendRect(&screen_src, 0, 0, 0, 0);
			}//end of pause menu
            //Clear per-frame flags
            stage.flag &= ~(STAGE_FLAG_JUST_STEP | STAGE_FLAG_SCORE_REFRESH);

            //Get song position
            boolean playing;
            fixed_t next_scroll;

	    if (stage.prefs.botplay && stage.nohud==0)
	    {
    			Menu_DrawBot(126, 49);
	    }

            #ifdef PSXF_NETWORK
            if (stage.prefs.mode >= StageMode_Net1 && !Network_IsReady())
            {
                if (!Network_IsHost())
                {
                    //Send ready packet
                    Packet ready;
                    ready[0] = PacketType_Ready;
                    Network_Send(&ready);
                    Network_SetReady(true);
                }
                next_scroll = stage.note_scroll;
            }
            else
            #endif
            {
                const fixed_t interp_int = FIXED_UNIT * 8 / 75;
                if (stage.note_scroll < 0)
                {
                    //Play countdown sequence
                    stage.song_time += timer_dt;

                    //Update song
                    if (stage.song_time >= 0)
                    {
                        //Song has started
                        playing = true;
                        Audio_PlayXA_Track(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, 0);

                        //Update song time
                        fixed_t audio_time = (fixed_t)Audio_TellXA_Milli() - stage.offset;
                        if (audio_time < 0)
                            audio_time = 0;
                        stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
                        stage.interp_time = 0;
                        stage.song_time = stage.interp_ms;
                    }
                    else
                    {
                        //Still scrolling
                        playing = false;
                    }

                    //Update scroll
                    next_scroll = FIXED_MUL(stage.song_time, stage.step_crochet);
                }
                else if (Audio_PlayingXA())
                {
                    fixed_t audio_time_pof = (fixed_t)Audio_TellXA_Milli();
                    fixed_t audio_time = (audio_time_pof > 0) ? (audio_time_pof - stage.offset) : 0;

                    if (stage.prefs.expsync)
                    {
                        //Get playing song position
                        if (audio_time_pof > 0)
                        {
                            stage.song_time += timer_dt;
                            stage.interp_time += timer_dt;
                        }

                        if (stage.interp_time >= interp_int)
                        {
                            //Update interp state
                            while (stage.interp_time >= interp_int)
                                stage.interp_time -= interp_int;
                            stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
                        }

                        //Resync
                        fixed_t next_time = stage.interp_ms + stage.interp_time;
                        if (stage.song_time >= next_time + FIXED_DEC(25,1000) || stage.song_time <= next_time - FIXED_DEC(25,1000))
                        {
                            stage.song_time = next_time;
                        }
                        else
                        {
                            if (stage.song_time < next_time - FIXED_DEC(1,1000))
                                stage.song_time += FIXED_DEC(1,1000);
                            if (stage.song_time > next_time + FIXED_DEC(1,1000))
                                stage.song_time -= FIXED_DEC(1,1000);
                        }
                    }
                    else
                    {
                        //Old sync
                        stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
                        stage.interp_time = 0;
                        stage.song_time = stage.interp_ms;
                    }

                    playing = true;

                    //Update scroll
                    next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);
                }
                else
                {
                    if(stage.paused ==false)
                    {
                    //Song has ended
                    playing = false;
                    stage.song_time += timer_dt;

                    //Update scroll
                    next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);

				storeresults[0] += stage.sickk;
				storeresults[1] += stage.goodd;
				storeresults[2] += stage.badd;
				storeresults[3] += stage.shitt;
				storeresults[4] += stage.misses;
				for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
				{
					PlayerState *this = &stage.player_state[i];
					storeresults[5] += (this->score * stage.max_score / this->max_score);
				}
				storeresults[6] += highcombo;

			    if(!stage.story)
			    {
				    Stage_LoadResults();
				    stage.state = StageState_Results;
			    }
		            //Transition to menu or next song
		            if (stage.story && stage.stage_def->next_stage != stage.stage_id)
		            {
				        stage.song_completed = true;
				        if (Stage_NextLoad())
				            goto SeamLoad;
		            }
		            else
		            {
					Stage_LoadResults();
					stage.state = StageState_Results;
				        //stage.song_completed = true;
				        //stage.trans = StageTrans_Menu;
				        //Trans_Start();
		            }
                    }
                }
            }

            RecalcScroll:;
            //Update song scroll and step
            if (next_scroll > stage.note_scroll)
            {
                if (((stage.note_scroll / 12) & FIXED_UAND) != ((next_scroll / 12) & FIXED_UAND))
                    stage.flag |= STAGE_FLAG_JUST_STEP;
                stage.note_scroll = next_scroll;
                stage.song_step = (stage.note_scroll >> FIXED_SHIFT);
                if (stage.note_scroll < 0)
                    stage.song_step -= 11;
                stage.song_step /= 12;
            }

            //Update section
            if (stage.note_scroll >= 0)
            {
                //Check if current section has ended
                u16 end = stage.cur_section->end;
                if ((stage.note_scroll >> FIXED_SHIFT) >= end)
                {
                    //Increment section pointer
                    stage.cur_section++;

                    //Update BPM
                    u16 next_bpm = stage.cur_section->flag & SECTION_FLAG_BPM_MASK;
                    Stage_ChangeBPM(next_bpm, end);
                    stage.section_base = stage.cur_section;

                    //Recalculate scroll based off new BPM
                    next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);
                    goto RecalcScroll;
                }
            }

            //Handle bump
            if ((stage.bump = FIXED_UNIT + FIXED_MUL(stage.bump - FIXED_UNIT, FIXED_DEC(95,100))) <= FIXED_DEC(1003,1000))
                stage.bump = FIXED_UNIT;
            stage.sbump = FIXED_UNIT + FIXED_MUL(stage.sbump - FIXED_UNIT, FIXED_DEC(60,100));

            if (playing && (stage.flag & STAGE_FLAG_JUST_STEP))
            {
                //Check if screen should bump
                boolean is_bump_step = (stage.song_step & 0xF) == 0;

                switch (cambump)
                {
                    case 0: //normal, bump every 4 beats
                    {
                        if (is_bump_step)
                            stage.bump = FIXED_DEC(103,100);
                        break;
                    }
                    case 1: //no bumping
                    {
                        is_bump_step = NULL;
                        if (is_bump_step)
                            stage.bump = FIXED_DEC(103,100);
                        break;
                    }
                    case 2: //bump every 2 beats
                    {
                        is_bump_step = (stage.song_step  & 0x7) == 0;
                        if (is_bump_step)
                            stage.bump = FIXED_DEC(103,100);
                        break;
                    }
                    case 3: //bump violently every beat
                    {
                        is_bump_step = (stage.song_step & 0x3) == 0;
                        if (is_bump_step)
                            stage.bump = FIXED_DEC(109,100);
                        break;
                    }
                }

                //M.I.L.F bumps
                if (stage.stage_id == StageId_4_3 && stage.song_step >= (168 << 2) && stage.song_step < (200 << 2))
                	is_bump_step = (stage.song_step & 0x3) == 0;

                //Bump health every 4 steps
                if (noheadbump == 1)
                {
                    if ((stage.song_step & 0x3) == 0)
                    stage.sbump = FIXED_DEC(100,100);
                }
                else
                {
                    if ((stage.song_step & 0x3) == 0)
                    stage.sbump = FIXED_DEC(103,100);
                }
            }

            //Scroll camera
            if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
                Stage_FocusCharacter(stage.opponent, FIXED_UNIT / 24);
            else
                Stage_FocusCharacter(stage.player, FIXED_UNIT / 24);
            Stage_ScrollCamera();

            switch (stage.prefs.mode)
            {
                case StageMode_Normal:
                case StageMode_Swap:
                {
                    //Handle player 1 inputs
                    Stage_ProcessPlayer(&stage.player_state[0], &pad_state, playing);

                    //Handle opponent notes
                    u8 opponent_anote = CharAnim_Idle;
                    u8 opponent_snote = CharAnim_Idle;

                    for (Note *note = stage.cur_note;; note++)
                    {
                        if (note->pos > (stage.note_scroll >> FIXED_SHIFT))
                            break;

                        //Opponent note hits
                        if (playing && (note->type & NOTE_FLAG_OPPONENT) && !(note->type & NOTE_FLAG_HIT))
                        {
                            //Opponent hits note
                            Stage_StartVocal();
                            if (note->type & NOTE_FLAG_SUSTAIN)
                                opponent_snote = note_anims[note->type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
                            else
                                opponent_anote = note_anims[note->type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
                            note->type |= NOTE_FLAG_HIT;
                        }
                    }

                    if (opponent_anote != CharAnim_Idle)
                        stage.opponent->set_anim(stage.opponent, opponent_anote);
                    else if (opponent_snote != CharAnim_Idle)
                        stage.opponent->set_anim(stage.opponent, opponent_snote);
                    break;
                }
                case StageMode_2P:
                {
                    //Handle player 1 and 2 inputs
                    Stage_ProcessPlayer(&stage.player_state[0], &pad_state, playing);
                    Stage_ProcessPlayer(&stage.player_state[1], &pad_state_2, playing);
                    break;
                }
            #ifdef PSXF_NETWORK
                case StageMode_Net1:
                {
                    //Handle player 1 inputs
                    Stage_ProcessPlayer(&stage.player_state[0], &pad_state, playing);
                    break;
                }
                case StageMode_Net2:
                {
                    //Handle player 2 inputs
                    Stage_ProcessPlayer(&stage.player_state[1], &pad_state, playing);
                    break;
                }
            #endif
            }

            //Tick note splashes
            ObjectList_Tick(&stage.objlist_splash);

            //Draw stage notes
		Stage_DrawNotes();

		//Draw note HUD
		RECT note_src = {0, 0, 32, 32};
		RECT_FIXED note_dst = {0, note_y - FIXED_DEC(16,1), FIXED_DEC(32,1), FIXED_DEC(32,1)};
		if (stage.prefs.downscroll)
			note_dst.y = -note_dst.y - note_dst.h;

		for (u8 i = 0; i < 4; i++)
		{
			//BF
			note_dst.x = note_x[i ^ stage.note_swap] - FIXED_DEC(16,1);
			Stage_DrawStrum(i, &note_src, &note_dst);
			if(stage.bluenotes)
				Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
			else
				Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);

			//Opponent
			note_dst.x = note_x[(i | 0x4) ^ stage.note_swap] - FIXED_DEC(16,1);
			Stage_DrawStrum(i | 4, &note_src, &note_dst);
			if(stage.bluenotes)
					Stage_DrawTex(&stage.tex_blue, &note_src, &note_dst, stage.bump);
				else
					Stage_DrawTex(&stage.tex_hud0, &note_src, &note_dst, stage.bump);
		}

            //Draw score
			for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];

				if (this->refresh_pause)
				{
					if((stage.sickk !=0 || stage.goodd !=0 || stage.badd != 0 || stage.shitt !=0))
						sprintf(this->pause_text, "SICKS = %d\nGOODS = %d\nBADS = %d\nSHITS = %d", stage.sickk, stage.goodd, stage.badd, stage.shitt);
					else
						sprintf(this->pause_text, "SICKS = 0\nGOODS = 0\nBADS = 0\nSHITS = 0");
					//stage.font_cdr.draw(&stage.font_cdr, stage.pause_text, 4, 18, FontAlign_Left);
					this->refresh_pause = false;
				}
				if ((this->refresh_score || this->refresh_misses))
				{
					if(stage.prefs.botplay ==0)
					{
						if ((this->score != 0 || stage.misses != 0))
							sprintf(this->score_text, "Score: %d0  |  Misses: %d", this->score * stage.max_score / this->max_score, stage.misses);
						else
						{
							strcpy(this->score_text, "Score: 0  |  Misses: 0");
						}
				        }
				        else
				        {
				        	strcpy(this->score_text, "Score: ?  |  Misses: ?  |  BOTPLAY");
				        }
					this->refresh_score = false;
					this->refresh_misses = false;
				}
				if(!stage.nohud && !stage.prefs.onlyarrows)
				{
					if(stage.prefs.mode == StageMode_2P)
					{
						if (stage.prefs.downscroll)
						{
						stage.font_cdr.draw(&stage.font_cdr, this->score_text, (i ^ (stage.prefs.mode == StageMode_Swap)) ? 170 - 80 : 170 + 80, 35, FontAlign_Center);
						}
						else
						{
						stage.font_cdr.draw(&stage.font_cdr, this->score_text, (i ^ (stage.prefs.mode == StageMode_Swap)) ? 170 - 80 : 170 + 80, 220, FontAlign_Center);
						}
					}
					else
					{
						if (stage.prefs.downscroll)
						{
						stage.font_cdr.draw(&stage.font_cdr, this->score_text, 170, 35, FontAlign_Center);
						}
						else
						{
						stage.font_cdr.draw(&stage.font_cdr, this->score_text, 170, 220, FontAlign_Center);
						}
					}
				}

			}

            if (stage.prefs.mode < StageMode_2P)
			{
				//Perform health checks
				if (stage.player_state[0].health <= 0)
				{
					//Player has died
					stage.player_state[0].health = 0;
					stage.state = StageState_Dead;
				}
				if (stage.player_state[0].health > 20000)
					stage.player_state[0].health = 20000;

				//Draw health heads
				if(!stage.prefs.onlyarrows)
				{
					Stage_DrawHealth(stage.player_state[0].health, stage.player->health_i,    1);
					Stage_DrawHealth(stage.player_state[0].health, stage.opponent->health_i, -1);
				}

				//Draw health bar
				RECT health_fill = {0, 0, stage.hud1shit3 - (stage.hud1shit3 * stage.player_state[0].health / 20000), stage.hud1shit};
				RECT health_back = {0, stage.hud1shit, 256, stage.hud1shit};
				RECT_FIXED health_dst = {FIXED_DEC(-128,1), (SCREEN_HEIGHT2 - 32 - stage.hud1shit2) << FIXED_SHIFT, 0, FIXED_DEC(stage.hud1shit,1)};
				if (stage.prefs.downscroll)
						health_dst.y = -health_dst.y - health_dst.h;

				health_dst.w = health_fill.w << FIXED_SHIFT;
				if(!stage.prefs.onlyarrows)
					Stage_DrawTex(&stage.tex_hud1, &health_fill, &health_dst, stage.bump);
				health_dst.w = health_back.w << FIXED_SHIFT;
				if(!stage.prefs.onlyarrows)
					Stage_DrawTex(&stage.tex_hud1, &health_back, &health_dst, stage.bump);
			}

            //Draw stage foreground
            if (stage.back->draw_fg != NULL)
                stage.back->draw_fg(stage.back);

            //Tick foreground objects
            ObjectList_Tick(&stage.objlist_fg);

            //Tick characters
            stage.player->tick(stage.player);
            stage.opponent->tick(stage.opponent);

            //Draw stage middle
            if (stage.back->draw_md != NULL)
                stage.back->draw_md(stage.back);

            //Tick girlfriend
            if (stage.gf != NULL)
                stage.gf->tick(stage.gf);

            //Tick background objects
            ObjectList_Tick(&stage.objlist_bg);

            //Draw stage background
            if (stage.back->draw_bg != NULL)
                stage.back->draw_bg(stage.back);
            break;
        }
        case StageState_Dead: //Start BREAK animation and reading extra data from CD
        {
            //Stop music immediately
            Audio_StopXA();

            //Unload stage data
            Mem_Free(stage.chart_data);
            stage.chart_data = NULL;

            //Free background
            stage.back->free(stage.back);
            stage.back = NULL;

            //Free objects
            ObjectList_Free(&stage.objlist_fg);
            ObjectList_Free(&stage.objlist_bg);

            //Free opponent and girlfriend
            Stage_SwapChars();
            Character_Free(stage.opponent);
            stage.opponent = NULL;
            Character_Free(stage.gf);
            stage.gf = NULL;

            //Reset stage state
            stage.flag = 0;
            stage.bump = stage.sbump = FIXED_UNIT;

            //Change background colour to black
            Gfx_SetClear(0, 0, 0);

            //Run death animation, focus on player, and change state
            stage.player->set_anim(stage.player, PlayerAnim_Dead0);

            Stage_FocusCharacter(stage.player, 0);
            stage.song_time = 0;

            storeresults[0] += stage.sickk;
            storeresults[1] += stage.goodd;
            storeresults[2] += stage.badd;
            storeresults[3] += stage.shitt;
            storeresults[4] += stage.misses;
            for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
            {
                PlayerState *this = &stage.player_state[i];
                storeresults[5] += (this->score * stage.max_score / this->max_score);
            }
            storeresults[6] += highcombo;

            stage.state = StageState_DeadLoad;
        }
    //Fallthrough
        case StageState_DeadLoad:
        {
            //Scroll camera and tick player
            if (stage.song_time < FIXED_UNIT)
                stage.song_time += FIXED_UNIT / 60;
            stage.camera.td = FIXED_DEC(-2, 100) + FIXED_MUL(stage.song_time, FIXED_DEC(45, 1000));
            if (stage.camera.td > 0)
                Stage_ScrollCamera();
            stage.player->tick(stage.player);

            //Drop mic and change state if CD has finished reading and animation has ended
            if (IO_IsReading() || stage.player->animatable.anim != PlayerAnim_Dead1)
                break;

            stage.player->set_anim(stage.player, PlayerAnim_Dead2);
            stage.camera.td = FIXED_DEC(25, 1000);
            stage.state = StageState_DeadDrop;
            break;
        }
        case StageState_DeadDrop:
        {
            //Scroll camera and tick player
            Stage_ScrollCamera();
            stage.player->tick(stage.player);

            //Enter next state once mic has been dropped
            if (stage.player->animatable.anim == PlayerAnim_Dead3)
            {
                stage.state = StageState_DeadRetry;
                if (stage.stage_id >= StageId_6_1 && stage.stage_id <= StageId_6_3)
                	Audio_PlayXA_Track(XA_Gameoverpixel, 0x40, 0, true);
                else
                	Audio_PlayXA_Track(XA_GameOver, 0x40, 1, true);
            }
            break;
        }
        case StageState_DeadRetry:
        {
            for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];

				stage.font_cdr.draw(&stage.font_cdr, this->pause_text, 4, 18, FontAlign_Left);
			}

			sprintf(stage.Results_text1, "MAX COMBO %d", storeresults[6]);
			//Draw mind games port version identification
			    stage.font_cdr.draw(&stage.font_cdr,
				stage.Results_text1,
				80,
				230,
				FontAlign_Center
			    );

			sprintf(stage.Results_text2, "Notes Missed: %d", storeresults[4]);
			//Draw mind games port version identification
			    stage.font_cdr.draw(&stage.font_cdr,
				stage.Results_text2,
				240,
				230,
				FontAlign_Center
			    );

            if (pad_state.press & (PAD_START | PAD_CIRCLE))
		{
		    stage.trans = StageTrans_Reload;
		    Trans_Start();
		    for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
		}
	    if (pad_state.press & PAD_CROSS)
		{
		    stage.trans = StageTrans_Menu;
		    Trans_Start();
		}

            //Randomly twitch
            if (stage.player->animatable.anim == PlayerAnim_Dead3)
            {
                if (RandomRange(0, 29) == 0)
                    stage.player->set_anim(stage.player, PlayerAnim_Dead4);
                if (RandomRange(0, 29) == 0)
                    stage.player->set_anim(stage.player, PlayerAnim_Dead5);
            }

            //Scroll camera and tick player
            Stage_ScrollCamera();
            stage.player->tick(stage.player);
            break;
        }
        case StageState_Dialogue:
        {
            stage.cutdia=1;
            //oh boy
            RECT dia_src = {0, 54, 205, 58};
            RECT_FIXED dia_dst = {FIXED_DEC(-140,1), FIXED_DEC(20,1), FIXED_DEC(265,1), FIXED_DEC(88,1)};

			static Dialogue_Struct* dialoguep;

            static Dialogue_Struct senpaidia[] = {
                {"Ah, a new fair maiden has come\nin search of true love!", 1, Dialogue_Senpai_Normal, 0},
                {"A serenade between gentlemen\nshall decide where her\nbeautiful heart shall reside.", 1, Dialogue_Senpai_Normal, 0},
                {"Beep bo bop", 0, Dialogue_BF_Normal, 0},
            };

            static Dialogue_Struct rosesdia[] = {
                {"Not bad for an ugly worm.", 1, Dialogue_Angy_Senpai, 0},
                {"But this time I'll rip your nuts off\nright after your girlfriend\nfinishes gargling mine.", 1, Dialogue_Angy_Senpai, 0},
                {"Bop beep be be skdoo bep", 0, Dialogue_BF_Normal, 0},
            };

            static Dialogue_Struct thornsdia[] = {
                {"Direct contact with real humans,\nafter being trapped in here\nfor so long...", 1, Dialogue_Spirit, 1},
                {"and HER of all people.", 1, Dialogue_Spirit, 1},
                {"I'll make her father pay\nfor what he's done to me\nand all the others....", 1, Dialogue_Spirit, 1},
                {"I'll beat you and\nmake you take my place.", 1, Dialogue_Spirit, 1},
                {"You don't mind your bodies\nbeing borrowed right?\nIt's only fair...", 1, Dialogue_Spirit, 1},
            };

            //Clear per-frame flags
            stage.flag &= ~(STAGE_FLAG_JUST_STEP | STAGE_FLAG_SCORE_REFRESH);

            //play dialogue song
            if (Audio_PlayingXA() != 1)
            {
                Audio_PlayXA_Track(stage.stage_def->diasong, 0x40, stage.stage_def->dia_channel, true); //read stagedef and play song
            }

			u16 dialogue_final = 0;

			switch (stage.stage_id)
			{
				case StageId_6_2:
					dialoguep = rosesdia;
					dialogue_final = COUNT_OF(rosesdia);
				break;
				case StageId_6_3:
					dialoguep = thornsdia;
					dialogue_final = COUNT_OF(thornsdia);
				break;
				default:
					dialoguep = senpaidia;
					dialogue_final = COUNT_OF(senpaidia);
				break;
			}

			stage.portrait.next = dialoguep[stage.delect].portrait;

			if (stage.delect == dialogue_final)
			{
				Audio_StopXA();
				stage.state = StageState_Play;
				Stage_UnLoadDia();
			}
			else
			{
				//text drawing shit
				stage.font_arial.draw(&stage.font_arial,
					dialoguep[stage.delect].text,
					47,
					160,
					FontAlign_Left
				);

				if (stage.portrait.current != stage.portrait.next && stage.delect < dialogue_final)
				{
					//Update the current portrait
					stage.portrait.current = stage.portrait.next;

					//Change the texture when needed
					if (stage.portrait.tex_id !=  portraits[stage.portrait.current].tex)
					{
						stage.portrait.tex_id = portraits[stage.portrait.current].tex;
						Gfx_LoadTex(&stage.portrait.tex, Archive_Find(stage.portrait.data[dialoguep[stage.delect].camera], portrait_tims[stage.portrait.tex_id]), 0);
					}
				}

				diabox = (dialoguep[stage.delect].diaboxes != 0);

				//camera shit
				if (dialoguep[stage.delect].camera != 0)
					Stage_FocusCharacter(stage.opponent, FIXED_UNIT / 24);
				else
					Stage_FocusCharacter(stage.player, FIXED_UNIT / 24);

				if(!diabox)
				{
					Stage_DrawTex(&stage.tex_hud1, &dia_src, &dia_dst, stage.bump);
				}
				else
				{
					RECT coverup_bar = {20, 140, 265, 88};
            				Gfx_DrawRect(&coverup_bar, 0, 0, 0);
				}

				int pos_x = (dialoguep[stage.delect].camera) ? SCREEN_WIDTH2 - 60 : SCREEN_WIDTH - 60;
				int pos_y = SCREEN_HEIGHT2 + 5;

				//Draw Character portrait
				RECT dst = {
					pos_x - portraits[stage.portrait.current].pos.x,
					pos_y - portraits[stage.portrait.current].pos.y,
					portraits[stage.portrait.current].src.w,
					portraits[stage.portrait.current].src.h
				};

				Gfx_DrawTex(&stage.portrait.tex, &portraits[stage.portrait.current].src, &dst);
			}
            		Gfx_BlendRect(&thescreen, 255, 255, 255, 0);


            //Draw stage foreground
            if (stage.back->draw_fg != NULL)
                stage.back->draw_fg(stage.back);

            //Tick foreground objects
            ObjectList_Tick(&stage.objlist_fg);

            //Tick characters
            stage.player->tick(stage.player);
            stage.opponent->tick(stage.opponent);

            //Draw stage middle
            if (stage.back->draw_md != NULL)
                stage.back->draw_md(stage.back);

            //Tick girlfriend
            if (stage.gf != NULL)
                stage.gf->tick(stage.gf);

            //Tick background objects
            ObjectList_Tick(&stage.objlist_bg);

            //Draw stage background
            if (stage.back->draw_bg != NULL)
                stage.back->draw_bg(stage.back);

            //progress to next message
            if (pad_state.press & PAD_CROSS)
            {
                stage.delect++;
            }

			//skip dialogue
            if (pad_state.press & PAD_START)
            {
				stage.delect = dialogue_final;
            }

            Stage_ScrollCamera();

            break;
        }
        case StageState_Results:
        {
        	if (stage.prefs.botplay)
	    	{
    			Menu_DrawBot(126, 2);
	    	}
        	//Draw mind games port version identification
		    stage.font_bold.draw(&stage.font_bold,
			"RESULTS",
			16,
			8,
			FontAlign_Left
		    );
		//Draw stage name or story name
		if(!stage.story)
		{
		    stage.font_cdr.draw(&stage.font_cdr,
			stage.stage_def->pausestage,
			312,
			8,
			FontAlign_Right
		    );
		    stage.font_cdr.draw(&stage.font_cdr,
			"Freeplay",
			16,
			26,
			FontAlign_Left
		    );
		}
		else
		{
		    stage.font_cdr.draw(&stage.font_cdr,
			stage.storyname,
			312,
			8,
			FontAlign_Right
		    );
		    stage.font_cdr.draw(&stage.font_cdr,
			"Story Mode",
			16,
			26,
			FontAlign_Left
		    );
		 }//pausediff
		    stage.font_cdr.draw(&stage.font_cdr,
			pausediff,
			312,
			20,
			FontAlign_Right
		    );

		Stage_DrawSickForResults(16,sickmove);
		Stage_DrawGoodForResults(16,goodmove);//72
		Stage_DrawBadForResults(16,badmove);//104
		Stage_DrawShitForResults(16,shitmove);//136

        	if(sickmove !=40)
        	{
        		sickmove-=4;
        	}
        	if(goodmove !=72)
        	{
        		goodmove-=4;
        	}
        	if(badmove !=104)
        	{
        		badmove-=4;
        	}
        	if(shitmove !=136)
        	{
        		shitmove-=4;
        	}
        	else
        	{
			//Draw mind games port version identification
			    stage.font_bold.draw(&stage.font_bold,
				"MAX COMBO",
				80,
				168,
				FontAlign_Center
			    );

			if(stage.nohud==1)
			{
				//Draw mind games port version identification
				    stage.font_cdr.draw(&stage.font_cdr,
					"HUD DISABLED MODE",
					160,
					2,
					FontAlign_Center
				    );
			}
			int maxcombo = storeresults[0] + storeresults[1] + storeresults[2] + storeresults[3] + storeresults[4];

			sprintf(stage.Results_text1, "%d / %d", storeresults[6], maxcombo);
			//Draw mind games port version identification
			    stage.font_cdr.draw(&stage.font_cdr,
				stage.Results_text1,
				80,
				186,
				FontAlign_Center
			    );
			int hitnotes = storeresults[0] + storeresults[1] + storeresults[2] + storeresults[3];

			sprintf(stage.Results_text2, "%d / %d Notes Missed: %d", hitnotes, maxcombo, storeresults[4]);
			//Draw mind games port version identification
			    stage.font_cdr.draw(&stage.font_cdr,
				stage.Results_text2,
				240,
				186,
				FontAlign_Center
			    );
			//Draw mind games port version identification
			    stage.font_bold.draw(&stage.font_bold,
				"NOTES HIT",
				240,
				168,
				FontAlign_Center
			    );
			//Draw mind games port version identification
			    stage.font_bold.draw(&stage.font_bold,
				"SCORE",
				160,
				200,
				FontAlign_Center
			    );
			sprintf(stage.Results_text3, "%d0", storeresults[5]);

			//Draw mind games port version identification
			stage.font_cdr.draw(&stage.font_cdr,
				stage.Results_text3,
				160,
				218,
				FontAlign_Center
		        );

			//     achievement shit    //

			//No Hud achievement
			if(stage.nohud==1 && !stage.prefs.botplay && stage.stage_diff == StageDiff_Hard && !stage.prefs.unlockedAchievements[Beat_Any_Song_With_No_Hud])
			{
				Achievement_Unlock(Beat_Any_Song_With_No_Hud);
				stage.prefs.nohudsong = stage.stage_def->pausestage;
			}


			//Swap Mode achievement
			if(stage.prefs.mode == StageMode_Swap)
				Achievement_Unlock(Play_Swapped_Mode);

			//Full Combo Achievements
			if(stage.story && storeresults[6] == maxcombo && storeresults[6] !=0 && !stage.prefs.botplay && stage.stage_diff == StageDiff_Hard && stage.prefs.mode != StageMode_Swap && stage.prefs.mode != StageMode_2P)
	    		{
		    		switch(stage.stage_id)
		  		{
		  			case StageId_1_3:
		  				Achievement_Unlock(Full_Combo_Week_1);
		  				break;
		  			case StageId_2_3:
		  				Achievement_Unlock(Full_Combo_Week_2);
		  				break;
		  			case StageId_3_3:
		  				Achievement_Unlock(Full_Combo_Week_3);
		  				break;
		  			case StageId_4_3:
		  				Achievement_Unlock(Full_Combo_Week_4);
		  				break;
		  			case StageId_5_3:
		  				Achievement_Unlock(Full_Combo_Week_5);
		  				break;
		  			case StageId_6_3:
		  				Achievement_Unlock(Full_Combo_Week_6);
		  				break;
		  			case StageId_7_3:
		  				Achievement_Unlock(Full_Combo_Week_7);
		  				break;
		  			default:
		  				break;
		  		}
	  		}
	  		if(storeresults[6] == maxcombo && storeresults[6] !=0 && stage.stage_id == StageId_8_2 && !stage.prefs.botplay && stage.stage_diff == StageDiff_Hard && stage.prefs.mode != StageMode_Swap && stage.prefs.mode != StageMode_2P)
	  			Achievement_Unlock(Full_Combo_Senbonzakura);


			//inputs to make it not a softlock
			if (pad_state.press & PAD_CROSS)
		  	{
		  		WriteSaveDataStructToBinaryAndSaveItOnTheFuckingMemoryCard();
				stage.trans = StageTrans_Menu;
				Trans_Start();
				for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
			}
			if(!stage.story)
			{
				if (pad_state.press & PAD_SQUARE)
			  	{
					stage.trans = StageTrans_Reload;
					Trans_Start();
					for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
				}
			}
			else
			{
				if (pad_state.press & PAD_SQUARE)
			  	{
			  		switch(stage.stage_id)
			  		{
			  			case StageId_1_4:
			  				stage.stage_id = StageId_1_4;
			  				break;
			  			case StageId_1_3:
			  				stage.stage_id = StageId_1_3;
			  				break;
			  			case StageId_2_3:
			  				stage.stage_id = StageId_2_3;
			  				break;
			  			case StageId_3_3:
			  				stage.stage_id = StageId_3_1;
			  				break;
			  			case StageId_4_3:
			  				stage.stage_id = StageId_4_1;
			  				break;
			  			case StageId_5_3:
			  				stage.stage_id = StageId_5_1;
			  				break;
			  			case StageId_6_3:
			  				stage.stage_id = StageId_6_1;
			  				break;
			  			case StageId_7_3:
			  				stage.stage_id = StageId_7_1;
			  				break;
			  			default:
			  				break;
			  		}
			  		for (int i = 0; i < 7; i++)
		  			storeresults[i] =0;
			  		Stage_Load(stage.stage_id, stage.stage_diff, true);
				}
			}

		}//fall 25

        	Rating_DisplayComboHits();
        	if(bffall !=96 && sickmove <=240)
        	{
        		bffall +=12;
			RECT resbfF_src = {144, 112, 112, 144};
			RECT resbfF_dst = {
				185,
				bffall,
				112,
				144
			};
			Gfx_DrawTex(&stage.tex_sprites, &resbfF_src, &resbfF_dst);
		}
		if(bffall ==96 && shitmove !=136)
		{
			RECT resbfL_src = {0, 144, 128, 112};
			RECT resbfL_dst = {
				175,//14
				127,
				128,
				112
			};
			Gfx_DrawTex(&stage.tex_sprites, &resbfL_src, &resbfL_dst);
		}
        	if(shitmove ==136)
		{
			//return to menu button
			RECT resx_src = {160, 16, 96, 16};
			RECT resx_dst = {
				192,
				224,
				96,
				16
			};
			Gfx_DrawTex(&stage.tex_sprites, &resx_src, &resx_dst);

			if(!stage.story)
			{
				//replay song button
				RECT ress_src = {160, 0, 80, 16};
				RECT ress_dst = {
					40,
					224,
					80,
					16
				};
				Gfx_DrawTex(&stage.tex_sprites, &ress_src, &ress_dst);
			}
			else
			{
				//replay week button
				RECT resw_src = {160, 48, 80, 16};
				RECT resw_dst = {
					40,
					224,
					80,
					16
				};
				Gfx_DrawTex(&stage.tex_sprites, &resw_src, &resw_dst);
			}

			RECT resbfI_src = {0, 0, 159, 130};
			RECT resbfI_dst = {
				161,
				110,
				159,
				130
			};
			Gfx_DrawTex(&stage.tex_sprites, &resbfI_src, &resbfI_dst);
		}

		RECT sidel_src = {0, 0, 120, 240};
		RECT sidel_dst = {
			0,
			0,
			120,
			240
		};
		Gfx_DrawTex(&stage.tex_results, &sidel_src, &sidel_dst);

        	RECT sider_src = {135, 0, 120, 240};
		RECT sider_dst = {
			200,
			0,
			120,
			240
		};
		Gfx_DrawTex(&stage.tex_results, &sider_src, &sider_dst);

        	Gfx_SetClear(254, 200, 92);
        }
        default:
            break;
    }
}

#ifdef PSXF_NETWORK
void Stage_NetHit(Packet *packet)
{
    //Reject if not in stage
    if (gameloop != GameLoop_Stage)
        return;

    //Get packet info
    u16 i = ((*packet)[1] << 0) | ((*packet)[2] << 8);
    u32 hit_score = ((*packet)[3] << 0) | ((*packet)[4] << 8) | ((*packet)[5] << 16) | ((*packet)[6] << 24);
    u8 hit_type = (*packet)[7];
    u16 hit_combo = ((*packet)[8] << 0) | ((*packet)[9] << 8);

    //Get note pointer
    if (i >= stage.num_notes)
        return;

    Note *note = &stage.notes[i];
    u8 type = note->type & 0x3;

    u8 opp_flag = (stage.prefs.mode == StageMode_Net1) ? NOTE_FLAG_OPPONENT : 0;
    if ((note->type & NOTE_FLAG_OPPONENT) != opp_flag)
        return;

    //Update game state
    PlayerState *this = &stage.player_state[(stage.prefs.mode == StageMode_Net1) ? 1 : 0];
    stage.notes[i].type |= NOTE_FLAG_HIT;

    this->score = hit_score;
    this->refresh_score = true;
    this->combo = hit_combo;

    if (note->type & NOTE_FLAG_SUSTAIN)
    {
        //Hit a sustain
        Stage_StartVocal();
        this->arrow_hitan[type] = stage.step_time;
        this->character->set_anim(this->character, note_anims[type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);
    }
    else if (!(note->type & NOTE_FLAG_MINE)))
    {
        //Hit a note
        Stage_StartVocal();
        this->arrow_hitan[type] = stage.step_time;
        this->character->set_anim(this->character, note_anims[type & 0x3][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);

        //Create combo object
        Obj_Combo *combo = Obj_Combo_New(
            this->character->focus_x,
            this->character->focus_y,
            hit_type,
            this->combo >= 10 ? this->combo : 0xFFFF
        );
        if (combo != NULL)
            ObjectList_Add(&stage.objlist_fg, (Object*)combo);

        //Create note splashes if SICK
        if (hit_type == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                //Create splash object
                Obj_Splash *splash = Obj_Splash_New(
                    note_x[(note->type & 0x7) ^ stage.note_swap],
                    note_y * (stage.prefs.downscroll ? -1 : 1),
                    type
                );
                if (splash != NULL)
                    ObjectList_Add(&stage.objlist_splash, (Object*)splash);
            }
        }
    }
    else
    {
        //Hit a mine
        this->arrow_hitan[type & 0x3] = -1;
        if (this->character->spec & CHAR_SPEC_MISSANIM)
            this->character->set_anim(this->character, note_anims[type & 0x3][2]);
        else
            this->character->set_anim(this->character, note_anims[type & 0x3][0]);
    }
}

void Stage_NetMiss(Packet *packet)
{
    //Reject if not in stage
    if (gameloop != GameLoop_Stage)
        return;

    //Get packet info
    u8 type = (*packet)[1];
    u32 hit_score = ((*packet)[2] << 0) | ((*packet)[3] << 8) | ((*packet)[4] << 16) | ((*packet)[5] << 24);

    //Update game state
    PlayerState *this = &stage.player_state[(stage.prefs.mode == StageMode_Net1) ? 1 : 0];

    this->score = hit_score;
    this->refresh_score = true;

    //Missed
    if (!(type & ~0x3))
    {
        this->arrow_hitan[type] = -1;
        if (this->character->spec & CHAR_SPEC_MISSANIM)
            this->character->set_anim(this->character, note_anims[type][2]);
        else
            this->character->set_anim(this->character, note_anims[type][0]);
    }
    Stage_MissNote(this);
}
#endif
