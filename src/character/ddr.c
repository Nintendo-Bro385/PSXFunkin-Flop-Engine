/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "ddr.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//DDR character structure
enum
{
	DDR_ArcMain_Hud4,
	DDR_ArcMain_Hud41,
	DDR_ArcMain_Hud42,
	DDR_ArcMain_Hud43,
	
	DDR_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[DDR_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_DDR;

//DDR character definitions
static const CharFrame char_ddr_frame[] = {
	{DDR_ArcMain_Hud4, {  0,   0, 109, 165}, { 42, 163}}, //0 idle 1
	{DDR_ArcMain_Hud41, {110,   0, 106, 166}, { 41, 164}}, //1 idle 2
	{DDR_ArcMain_Hud42, {  0,   0, 106, 169}, { 41, 167}}, //2 idle 3
	{DDR_ArcMain_Hud43, {107,   0, 106, 167}, { 41, 165}}, //3 idle 4
};

static const Animation char_ddr_anim[CharAnim_Max] = {
	{3, (const u8[]){ 0, 0, 1, 1, 2, 2, 3, 3, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//DDR character functions
void Char_DDR_SetFrame(void *user, u8 frame)
{
	Char_DDR *this = (Char_DDR*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_ddr_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_DDR_Tick(Character *character)
{
	Char_DDR *this = (Char_DDR*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_DDR_SetFrame);
	Character_Draw(character, &this->tex, &char_ddr_frame[this->frame]);
}

void Char_DDR_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_DDR_Free(Character *character)
{
	Char_DDR *this = (Char_DDR*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_DDR_New(fixed_t x, fixed_t y)
{
	//Allocate ddr object
	Char_DDR *this = Mem_Alloc(sizeof(Char_DDR));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_DDR_New] Failed to allocate ddr object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_DDR_Tick;
	this->character.set_anim = Char_DDR_SetAnim;
	this->character.free = Char_DDR_Free;
	
	Animatable_Init(&this->character.animatable, char_ddr_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 12;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\DDR.ARC;1");
	
	const char **pathp = (const char *[]){
		"hud4.tim", //DDR_ArcMain_Idle0
		"hud41.tim", //DDR_ArcMain_Idle1
		"hud42.tim",  //DDR_ArcMain_Left
		"hud43.tim",  //DDR_ArcMain_Down
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
