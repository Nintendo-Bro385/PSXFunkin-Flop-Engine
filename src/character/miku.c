/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "miku.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Miku character structure
enum
{
	Miku_ArcMain_Idle0,
	Miku_ArcMain_Idle1,
	Miku_ArcMain_Idle2,
	Miku_ArcMain_Idle3,
	Miku_ArcMain_Idle4,
	Miku_ArcMain_Left,
	Miku_ArcMain_Down,
	Miku_ArcMain_Up,
	Miku_ArcMain_Right,

	Miku_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;

	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Miku_Arc_Max];

	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Miku;

//for X offsets - for right and + for left

//for Y offsets - for down and + for up

//Miku character definitions
static const CharFrame char_miku_frame[] = {
	{Miku_ArcMain_Idle0, {  0,   2, 116, 164}, { 46, 164}}, //0 idle 1
	{Miku_ArcMain_Idle0, {116,   0, 114, 166}, { 46, 166}}, //1 idle 2
	{Miku_ArcMain_Idle1, {  0,   0, 114, 166}, { 47, 166}}, //2 idle 3
	{Miku_ArcMain_Idle1, {114,   0, 116, 166}, { 47, 166}}, //3 idle 4
	{Miku_ArcMain_Idle2, {  0,   0, 116, 166}, { 48, 166}}, //4 idle 4
	{Miku_ArcMain_Idle2, {116,   2, 116, 164}, { 45, 164}}, //5 idle 4
	{Miku_ArcMain_Idle3, {  0,   0, 114, 166}, { 46, 166}}, //6 idle 4
	{Miku_ArcMain_Idle3, {114,   0, 115, 166}, { 47, 166}}, //7 idle 4
	{Miku_ArcMain_Idle4, {  0,   0, 116, 166}, { 48, 166}}, //8 idle 4

	{Miku_ArcMain_Left, {  0,   0, 114, 167}, { 56, 167}}, //9 left 1
	{Miku_ArcMain_Left, {114,   1, 112, 166}, { 56, 166}}, //10 left 2

	{Miku_ArcMain_Down, {  0,   3, 120, 158}, { 48, 158}}, //11 down 1
	{Miku_ArcMain_Down, {120,   0, 120, 161}, { 48, 161}}, //12 down 2

	{Miku_ArcMain_Up, {  0,   0, 108, 174}, { 51, 174}}, //13 up 1
	{Miku_ArcMain_Up, {108,   2, 114, 172}, { 53, 172}}, //14 up 2

	{Miku_ArcMain_Right, {  0,   1, 113, 166}, { 29, 166}}, //15 right 1
	{Miku_ArcMain_Right, {113,   0, 112, 167}, { 28, 167}}, //16 right 2
};

static const Animation char_miku_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, 4, 5, 6, 7, 8, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){13, 14, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){15, 16, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_Miku_SetFrame(void *user, u8 frame)
{
	Char_Miku *this = (Char_Miku*)user;

	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_miku_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Miku_Tick(Character *character)
{
	Char_Miku *this = (Char_Miku*)character;

	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);

	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Miku_SetFrame);
	Character_Draw(character, &this->tex, &char_miku_frame[this->frame]);
}

void Char_Miku_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Miku_Free(Character *character)
{
	Char_Miku *this = (Char_Miku*)character;

	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Miku_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_Miku *this = Mem_Alloc(sizeof(Char_Miku));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Miku_New] Failed to allocate miku object");
		ErrorLock();
		return NULL;
	}

	//Initialize character
	this->character.tick = Char_Miku_Tick;
	this->character.set_anim = Char_Miku_SetAnim;
	this->character.free = Char_Miku_Free;

	Animatable_Init(&this->character.animatable, char_miku_anim);
	Character_Init((Character*)this, x, y);

	//Set character information
	this->character.spec = 0;

	if(stage.stage_id == StageId_39_5)
    {
        this->character.health_i = 2;
    }
    else
    {
        this->character.health_i = 0;
    }

	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);

	//Load art
	this->arc_main = IO_Read("\\CHAR\\MIKU.ARC;1");

	const char **pathp = (const char *[]){
		"idle0.tim", //Clucky_ArcMain_Idle0
		"idle1.tim", //Clucky_ArcMain_Idle1
		"idle2.tim", //Clucky_ArcMain_Idle1
		"idle3.tim", //Clucky_ArcMain_Idle1
		"idle4.tim", //Clucky_ArcMain_Idle1
		"left.tim",  //Clucky_ArcMain_Left
		"down.tim",  //Clucky_ArcMain_Down
		"up.tim",    //Clucky_ArcMain_Up
		"right.tim", //Clucky_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);

	//Initialize render state
	this->tex_id = this->frame = 0xFF;

	return (Character*)this;
}
