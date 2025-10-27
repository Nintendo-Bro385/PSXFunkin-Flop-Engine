/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bfy.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../random.h"
#include "../main.h"

int r2;
int g2;
int b2;
char Color2[33];

//Boyfriend skull fragments
static SkullFragment char_bfy_skull[15] = {
	{ 1 * 8, -87 * 8, -13, -13},
	{ 9 * 8, -88 * 8,   5, -22},
	{18 * 8, -87 * 8,   9, -22},
	{26 * 8, -85 * 8,  13, -13},

	{-3 * 8, -82 * 8, -13, -11},
	{ 8 * 8, -85 * 8,  -9, -15},
	{20 * 8, -82 * 8,   9, -15},
	{30 * 8, -79 * 8,  13, -11},

	{-1 * 8, -74 * 8, -13, -5},
	{ 8 * 8, -77 * 8,  -9, -9},
	{19 * 8, -75 * 8,   9, -9},
	{26 * 8, -74 * 8,  13, -5},

	{ 5 * 8, -73 * 8, -5, -3},
	{14 * 8, -76 * 8,  9, -6},
	{26 * 8, -67 * 8, 15, -3},
};

//Boyfriend player types
enum
{
	BFY_ArcMain_Idle0,
	BFY_ArcMain_Idle1,
	BFY_ArcMain_Left0,
	BFY_ArcMain_Down0,
	BFY_ArcMain_Up0,
	BFY_ArcMain_Right0,
	BFY_ArcMain_Right1,
	BFY_ArcMain_Dead0, //BREAK

	BFY_ArcMain_Max,
};

enum
{
	BFY_ArcDead_Dead1, //Mic Drop
	BFY_ArcDead_Dead2, //Twitch
	BFY_ArcDead_Retry, //Retry prompt

	BFY_ArcDead_Max,
};

#define BFY_Arc_Max BFY_ArcMain_Max

typedef struct
{
	//Character base structure
	Character character;

	//Render data and state
	IO_Data arc_main, arc_dead;
	CdlFILE file_dead_arc; //dead.arc file position
	IO_Data arc_ptr[BFY_Arc_Max];

	Gfx_Tex tex, tex_retry;
	u8 frame, tex_id;

	u8 retry_bump;

	SkullFragment skull[COUNT_OF(char_bfy_skull)];
	u8 skull_scale;
} Char_BFY;

//Boyfriend player definitions
static const CharFrame char_bfy_frame[] = {
	{BFY_ArcMain_Idle0, {  0,   0,  97, 109}, { 53+5,   99}}, //0  idle 1
	{BFY_ArcMain_Idle0, { 97,   0,  97, 110}, { 53+6, 100}}, //1  idle 2
	{BFY_ArcMain_Idle0, {  0, 109,  98, 111}, { 53+7, 101}}, //2  idle 3
	{BFY_ArcMain_Idle0, { 98, 110,  97, 113}, { 53+7, 103}}, //3  idle 4
	{BFY_ArcMain_Idle1, {  0,   0,  96, 113}, { 53+6, 103}}, //4  idle 5
	{BFY_ArcMain_Idle1, { 96,   0,  96, 113}, { 53+6, 103}}, //5  idle 6

	{BFY_ArcMain_Idle1, {  0, 113, 106, 113}, { 53+15, 103}}, //6  left 1
	{BFY_ArcMain_Idle1, {106, 113, 107, 111}, { 53+16, 101}}, //7  left 2
	{BFY_ArcMain_Left0, {  0,   0, 105, 110}, { 53+14, 100}}, //8  left 3
	{BFY_ArcMain_Left0, {105,   0, 105, 112}, { 53+13, 102}}, //9  left 4
	{BFY_ArcMain_Left0, {  0, 112, 103, 112}, { 53+12, 102}}, //10 left 5

	{BFY_ArcMain_Left0, {103, 112,  98, 101}, { 53,  91}}, //0 idle 1
	{BFY_ArcMain_Down0, {  0,   0,  99, 102}, { 53,  92}}, //0 idle 1
	{BFY_ArcMain_Down0, { 99,   0,  96, 103}, { 53,  93}}, //0 idle 1
	{BFY_ArcMain_Down0, {  0, 103,  95, 104}, { 53,  94}}, //0 idle 1
	{BFY_ArcMain_Down0, { 95,   0,  94, 104}, { 53,  94}}, //0 idle 1

	{BFY_ArcMain_Up0, {  0,   0,  90, 122}, { 53,  112}}, //0 idle 1
	{BFY_ArcMain_Up0, { 90,   0,  91, 123}, { 53,  113}}, //0 idle 1
	{BFY_ArcMain_Up0, {  0, 122,  94, 122}, { 53,  112}}, //0 idle 1
	{BFY_ArcMain_Up0, { 94, 123,  94, 121}, { 53,  111}}, //0 idle 1
	{BFY_ArcMain_Right0, {  0,  0, 94, 119}, { 53, 109}}, //0 idle 1

	{BFY_ArcMain_Right0, { 94,   0, 105, 113}, { 53, 103}}, //0 idle 1
	{BFY_ArcMain_Right0, {  0, 119, 104, 114}, { 53, 104}}, //0 idle 1
	{BFY_ArcMain_Right0, {104, 113, 101, 114}, { 53, 104}}, //0 idle 1
	{BFY_ArcMain_Right1, {  0,   0, 100, 113}, { 53, 103}}, //0 idle 1
	{BFY_ArcMain_Right1, {100,   0, 100, 112}, { 53, 102}}, //0 idle 1

	{BFY_ArcMain_Dead0, {  0,   0, 128, 128}, { 53,  98}}, //23 dead0 0
	{BFY_ArcMain_Dead0, {128,   0, 128, 128}, { 53,  98}}, //24 dead0 1
	{BFY_ArcMain_Dead0, {  0, 128, 128, 128}, { 53,  98}}, //25 dead0 2
	{BFY_ArcMain_Dead0, {128, 128, 128, 128}, { 53,  98}}, //26 dead0 3

	{BFY_ArcDead_Dead1, {  0,   0, 128, 128}, { 53,  98}}, //27 dead1 0
	{BFY_ArcDead_Dead1, {128,   0, 128, 128}, { 53,  98}}, //28 dead1 1
	{BFY_ArcDead_Dead1, {  0, 128, 128, 128}, { 53,  98}}, //29 dead1 2
	{BFY_ArcDead_Dead1, {128, 128, 128, 128}, { 53,  98}}, //30 dead1 3

	{BFY_ArcDead_Dead2, {  0,   0, 128, 128}, { 53,  98}}, //31 dead2 body twitch 0
	{BFY_ArcDead_Dead2, {128,   0, 128, 128}, { 53,  98}}, //32 dead2 body twitch 1
	{BFY_ArcDead_Dead2, {  0, 128, 128, 128}, { 53,  98}}, //33 dead2 balls twitch 0
	{BFY_ArcDead_Dead2, {128, 128, 128, 128}, { 53,  98}}, //34 dead2 balls twitch 1
};

static const Animation char_bfy_anim[PlayerAnim_Max] = {
	{1, (const u8[]){ 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, ASCR_BACK, 1}}, //CharAnim_Idle
	{1, (const u8[]){ 6, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, ASCR_BACK, 1}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{1, (const u8[]){ 11, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15, 15, 15, 15, ASCR_BACK, 1}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{1, (const u8[]){ 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 20, 20, ASCR_BACK, 1}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{1, (const u8[]){21, 22, 22, 23, 23, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, ASCR_BACK, 1}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing

	{1, (const u8[]){ 6, ASCR_BACK, 1}},     //PlayerAnim_LeftMiss
	{1, (const u8[]){11, ASCR_BACK, 1}},     //PlayerAnim_DownMiss
	{1, (const u8[]){16, ASCR_BACK, 1}},     //PlayerAnim_UpMiss
	{1, (const u8[]){21, ASCR_BACK, 1}},     //PlayerAnim_RightMiss

	{2, (const u8[]){13, 14, 15, ASCR_BACK, 1}},         //PlayerAnim_Peace
	{2, (const u8[]){16, 17, 18, 19, ASCR_REPEAT}},      //PlayerAnim_Sweat

	{5, (const u8[]){26, 27, 28, 29, 29, 29, 29, 29, 29, 29, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{5, (const u8[]){29, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){30, 31, 32, 33, 33, 33, 33, 33, 33, 33, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	{3, (const u8[]){33, ASCR_REPEAT}},                                                       //PlayerAnim_Dead3
	{3, (const u8[]){34, 35, 33, 33, 33, 33, 33, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead4
	{3, (const u8[]){36, 37, 33, 33, 33, 33, 33, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead5

	{10, (const u8[]){33, 33, 33, ASCR_BACK, 1}}, //PlayerAnim_Dead4
	{ 3, (const u8[]){36, 37, 33, ASCR_REPEAT}},  //PlayerAnim_Dead5
};

//Boyfriend player functions
void Char_BFY_SetFrame(void *user, u8 frame)
{
	Char_BFY *this = (Char_BFY*)user;

	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bfy_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_BFY_Tick(Character *character)
{
	Char_BFY *this = (Char_BFY*)character;

	//Handle animation updates
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0 ||
	    (character->animatable.anim != CharAnim_Left &&
	     character->animatable.anim != CharAnim_LeftAlt &&
	     character->animatable.anim != CharAnim_Down &&
	     character->animatable.anim != CharAnim_DownAlt &&
	     character->animatable.anim != CharAnim_Up &&
	     character->animatable.anim != CharAnim_UpAlt &&
	     character->animatable.anim != CharAnim_Right &&
	     character->animatable.anim != CharAnim_RightAlt))
		Character_CheckEndSing(character);

	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		//Perform idle dance
		if (Animatable_Ended(&character->animatable) &&
			(character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_LeftAlt &&
		     character->animatable.anim != PlayerAnim_LeftMiss &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_DownAlt &&
		     character->animatable.anim != PlayerAnim_DownMiss &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_UpAlt &&
		     character->animatable.anim != PlayerAnim_UpMiss &&
		     character->animatable.anim != CharAnim_Right &&
		     character->animatable.anim != CharAnim_RightAlt &&
		     character->animatable.anim != PlayerAnim_RightMiss) &&
			(stage.song_step & 0x7) == 0)
			character->set_anim(character, CharAnim_Idle);
	}

	if (stage.flag & STAGE_FLAG_JUST_STEP)
    	{   //Stage specific animations
		if (stage.note_scroll >= 0)
		{
			switch (stage.stage_id)
			{
				case StageId_8_2: //Animations
				{
					switch (stage.song_step)
					{
						case 2400:
							this->character.focus_x = FIXED_DEC(-60,1);
							this->character.focus_y = FIXED_DEC(-84,1);
							this->character.focus_zoom = FIXED_DEC(8,7);
							break;
						case 2416:
							this->character.focus_x = FIXED_DEC(-60,1);
							this->character.focus_y = FIXED_DEC(-84,1);
							this->character.focus_zoom = FIXED_DEC(1,1);
							break;
					}
					break;
				}
				default:
					break;
			}
		}
	}

	//Retry screen
	if (character->animatable.anim >= PlayerAnim_Dead3)
	{
		//Tick skull fragments
		if (this->skull_scale)
		{
			SkullFragment *frag = this->skull;
			for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFY, skull); i++, frag++)
			{
				//Draw fragment
				RECT frag_src = {
					(i & 1) ? 112 : 96,
					(i >> 1) << 4,
					16,
					16
				};
				fixed_t skull_dim = (FIXED_DEC(16,1) * this->skull_scale) >> 6;
				fixed_t skull_rad = skull_dim >> 1;
				RECT_FIXED frag_dst = {
					character->x + (((fixed_t)frag->x << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.x,
					character->y + (((fixed_t)frag->y << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.y,
					skull_dim,
					skull_dim,
				};
				Stage_DrawTex(&this->tex_retry, &frag_src, &frag_dst, FIXED_MUL(stage.camera.zoom, stage.bump));

				//Move fragment
				frag->x += frag->xsp;
				frag->y += ++frag->ysp;
			}

			//Decrease scale
			this->skull_scale--;
		}

		//Draw input options
		u8 input_scale = 16 - this->skull_scale;
		if (input_scale > 16)
			input_scale = 0;

		RECT button_src = {
			 0, 96,
			16, 16
		};
		RECT_FIXED button_dst = {
			character->x - FIXED_DEC(32,1) - stage.camera.x,
			character->y - FIXED_DEC(88,1) - stage.camera.y,
			(FIXED_DEC(16,1) * input_scale) >> 4,
			FIXED_DEC(16,1),
		};

		//Cross - Retry
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));

		//Circle - Blueball
		button_src.x = 16;
		button_dst.y += FIXED_DEC(56,1);
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));

		//Draw 'RETRY'
		u8 retry_frame;

		if (character->animatable.anim == PlayerAnim_Dead6)
		{
			//Selected retry
			retry_frame = 2 - (this->retry_bump >> 3);
			if (retry_frame >= 3)
				retry_frame = 0;
			if (this->retry_bump & 2)
				retry_frame += 3;

			if (++this->retry_bump == 0xFF)
				this->retry_bump = 0xFD;
		}
		else
		{
			//Idle
			retry_frame = 1 +  (this->retry_bump >> 2);
			if (retry_frame >= 3)
				retry_frame = 0;

			if (++this->retry_bump >= 55)
				this->retry_bump = 0;
		}

		RECT retry_src = {
			(retry_frame & 1) ? 48 : 0,
			(retry_frame >> 1) << 5,
			48,
			32
		};
		RECT_FIXED retry_dst = {
			character->x -  FIXED_DEC(7,1) - stage.camera.x,
			character->y - FIXED_DEC(92,1) - stage.camera.y,
			FIXED_DEC(48,1),
			FIXED_DEC(32,1),
		};
		Stage_DrawTex(&this->tex_retry, &retry_src, &retry_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
	}

	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_BFY_SetFrame);

    if (stage.song_step>=3888)
        Character_DrawCol(character, &this->tex, &char_bfy_frame[this->frame], 100, 113, 155);
    else
        Character_Draw(character, &this->tex, &char_bfy_frame[this->frame]);
}

void Char_BFY_SetAnim(Character *character, u8 anim)
{
	Char_BFY *this = (Char_BFY*)character;

	//Perform animation checks
	switch (anim)
	{
		case PlayerAnim_Dead0:
			//Begin reading dead.arc and adjust focus
			this->arc_dead = IO_AsyncReadFile(&this->file_dead_arc);
			character->focus_x = FIXED_DEC(0,1);
			character->focus_y = FIXED_DEC(-40,1);
			character->focus_zoom = FIXED_DEC(125,100);
			break;
		case PlayerAnim_Dead2:
			//Unload main.arc
			Mem_Free(this->arc_main);
			this->arc_main = this->arc_dead;
			this->arc_dead = NULL;

			//Find dead.arc files
			const char **pathp = (const char *[]){
				"dead1.tim", //BFY_ArcDead_Dead1
				"dead2.tim", //BFY_ArcDead_Dead2
				"retry.tim", //BFY_ArcDead_Retry
				NULL
			};
			IO_Data *arc_ptr = this->arc_ptr;
			for (; *pathp != NULL; pathp++)
				*arc_ptr++ = Archive_Find(this->arc_main, *pathp);

			//Load retry art
			Gfx_LoadTex(&this->tex_retry, this->arc_ptr[BFY_ArcDead_Retry], 0);
			break;
	}

	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_BFY_Free(Character *character)
{
	Char_BFY *this = (Char_BFY*)character;

	//Free art
	Mem_Free(this->arc_main);
	Mem_Free(this->arc_dead);
}

Character *Char_BFY_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_BFY *this = Mem_Alloc(sizeof(Char_BFY));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_BFY_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}

	//Initialize character
	this->character.tick = Char_BFY_Tick;
	this->character.set_anim = Char_BFY_SetAnim;
	this->character.free = Char_BFY_Free;

	Animatable_Init(&this->character.animatable, char_bfy_anim);
	Character_Init((Character*)this, x, y);

	//Set character information
	this->character.spec = CHAR_SPEC_MISSANIM;

	this->character.health_i = 1;

	this->character.focus_x = FIXED_DEC(-60,1);
	this->character.focus_y = FIXED_DEC(-84,1);
	this->character.focus_zoom = FIXED_DEC(1,1);

	//Load art
	this->arc_main = IO_Read("\\CHAR\\BFY.ARC;1");
	this->arc_dead = NULL;
	IO_FindFile(&this->file_dead_arc, "\\CHAR\\BFDEAD.ARC;1");

	const char **pathp = (const char *[]){
		"idle0.tim",   //BFY_ArcMain_BFY0
		"idle1.tim",   //BFY_ArcMain_BFY1
		"left0.tim",   //BFY_ArcMain_BFY2
		"down0.tim",   //BFY_ArcMain_BFY3
		"up0.tim",   //BFY_ArcMain_BFY4
		"right0.tim",   //BFY_ArcMain_BFY5
		"right1.tim",   //BFY_ArcMain_BFY6
		"dead0.tim", //BFY_ArcMain_Dead0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);

	//Initialize render state
	this->tex_id = this->frame = 0xFF;

	//Initialize player state
	this->retry_bump = 0;

	//Copy skull fragments
	memcpy(this->skull, char_bfy_skull, sizeof(char_bfy_skull));
	this->skull_scale = 64;

	SkullFragment *frag = this->skull;
	for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFY, skull); i++, frag++)
	{
		//Randomize trajectory
		frag->xsp += RandomRange(-4, 4);
		frag->ysp += RandomRange(-2, 2);
	}

	return (Character*)this;
}
