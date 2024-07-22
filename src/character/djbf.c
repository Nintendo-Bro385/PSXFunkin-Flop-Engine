/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "djbf.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Djbf character structure
enum
{
	Djbf_ArcMain_Idle0,
	Djbf_ArcMain_Idle1,
	Djbf_ArcMain_Spin0,
	Djbf_ArcMain_Spin1,
	Djbf_ArcMain_Select0,
	
	Djbf_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Djbf_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Djbf;

//Djbf character definitions
static const CharFrame char_djbf_frame[] = {
	{Djbf_ArcMain_Idle0, {  0,   0,  98,  110}, { 53,  92}}, //idle0 0
	{Djbf_ArcMain_Idle0, { 98,   0,  99,  108}, { 54,  90}}, //idle1 1
	{Djbf_ArcMain_Idle0, {  0, 110,  98,  114}, { 53,  96}}, //idle2 2
	{Djbf_ArcMain_Idle0, { 98, 110,  99,  114}, { 54,  96}}, //idle3 3
	{Djbf_ArcMain_Idle1, {  0,   0,  98,  113}, { 53,  95}}, //idle4 4
	{Djbf_ArcMain_Idle1, { 98,   0,  99,  113}, { 54,  95}}, //idle4 5
	{Djbf_ArcMain_Idle1, {  0, 113,  98,  113}, { 53,  95}}, //idle4 6
	
	{Djbf_ArcMain_Spin0, {  0,   0, 108,  104}, { 63,  86}}, //spin0 7
	{Djbf_ArcMain_Spin0, {108,   0, 107,  104}, { 62,  86}}, //spin1 8
	{Djbf_ArcMain_Spin0, {  0, 104, 108,  105}, { 63,  87}}, //spin2 9
	{Djbf_ArcMain_Spin0, {108, 104, 108,  106}, { 63,  88}}, //spin3 10
	{Djbf_ArcMain_Spin1, {  0,   0, 109,  107}, { 64,  89}}, //spin4 11
	{Djbf_ArcMain_Spin1, {109,   0, 110,  105}, { 65,  87}}, //spin5 12
	{Djbf_ArcMain_Spin1, {  0, 105, 98,   105}, { 53,  87}}, //spin6 13
	
	{Djbf_ArcMain_Select0, {  0,   0,  99,  109}, { 54,  91}}, //select0 14
	{Djbf_ArcMain_Select0, { 99,   0,  98,  109}, { 53,  91}}, //select1 15
	{Djbf_ArcMain_Select0, {  0, 109,  99,  108}, { 54,  90}}, //select2 16
	{Djbf_ArcMain_Select0, { 99, 109,  98,  108}, { 53,  90}}, //select3 17
};

static const Animation char_djbf_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ 7, 8, 9, 10, 11, 12, 13,  ASCR_CHGANI, CharAnim_Idle}},          //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){ 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 17, 17, 17, 17,  ASCR_BACK, 1}},    //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Djbf character functions
void Char_Djbf_SetFrame(void *user, u8 frame)
{
	Char_Djbf *this = (Char_Djbf*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_djbf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Djbf_Tick(Character *character)
{
	Char_Djbf *this = (Char_Djbf*)character;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{	
		//Perform dance
		if (stage.note_scroll >= character->sing_end && (stage.song_step % stage.gf_speed) == 0)
			character->set_anim(character, CharAnim_Idle);
		
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Djbf_SetFrame);
	Character_Draw(character, &this->tex, &char_djbf_frame[this->frame]);
}

void Char_Djbf_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Djbf_Free(Character *character)
{
	Char_Djbf *this = (Char_Djbf*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Djbf_New(fixed_t x, fixed_t y)
{
	//Allocate djbf object
	Char_Djbf *this = Mem_Alloc(sizeof(Char_Djbf));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Djbf_New] Failed to allocate djbf object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Djbf_Tick;
	this->character.set_anim = Char_Djbf_SetAnim;
	this->character.free = Char_Djbf_Free;
	
	Animatable_Init(&this->character.animatable, char_djbf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 11;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\DJBF.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Djbf_ArcMain_Idle0
		"idle1.tim", //Djbf_ArcMain_Idle1
		"spin0.tim",
		"spin1.tim",
		"select0.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
