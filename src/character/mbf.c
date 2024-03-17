/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "mbf.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Mbf character structure
enum
{
	Mbf_ArcMain_Menubf,
	
	Mbf_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Mbf_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Mbf;

//Mbf character definitions
static const CharFrame char_mbf_frame[] = {
	{Mbf_ArcMain_Menubf, {  0,   0,  83,  80}, { 53,  92}}, //idle0 0
	{Mbf_ArcMain_Menubf, { 83,   0,  83,  79}, { 53,  91}}, //idle1 1
	{Mbf_ArcMain_Menubf, {166,   0,  82,  81}, { 52,  93}}, //idle2 2
	{Mbf_ArcMain_Menubf, {  0,  81,  84,  84}, { 52,  96}}, //idle3 3
	{Mbf_ArcMain_Menubf, { 84,  81,  83,  84}, { 52,  96}}, //idle4 4

	{Mbf_ArcMain_Menubf, {167,  81,  80,  84}, { 52,  96}}, //peace0 5
	{Mbf_ArcMain_Menubf, {  0, 165,  84,  84}, { 53,  95}}, //peace1 6
	{Mbf_ArcMain_Menubf, { 84, 165,  85,  84}, { 53,  95}}, //peace2 7
};

static const Animation char_mbf_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, ASCR_BACK, 1}}, //CharAnim_Idle
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},          //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 5, 6, 6, 7, 7, 7,  ASCR_BACK, 1}},        //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Mbf character functions
void Char_Mbf_SetFrame(void *user, u8 frame)
{
	Char_Mbf *this = (Char_Mbf*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_mbf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Mbf_Tick(Character *character)
{
	Char_Mbf *this = (Char_Mbf*)character;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{	
		//Perform dance
		if (stage.note_scroll >= character->sing_end && (stage.song_step % stage.gf_speed) == 0)
			character->set_anim(character, CharAnim_Idle);
		
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Mbf_SetFrame);
	Character_Draw(character, &this->tex, &char_mbf_frame[this->frame]);
}

void Char_Mbf_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Mbf_Free(Character *character)
{
	Char_Mbf *this = (Char_Mbf*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Mbf_New(fixed_t x, fixed_t y)
{
	//Allocate mbf object
	Char_Mbf *this = Mem_Alloc(sizeof(Char_Mbf));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Mbf_New] Failed to allocate mbf object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Mbf_Tick;
	this->character.set_anim = Char_Mbf_SetAnim;
	this->character.free = Char_Mbf_Free;
	
	Animatable_Init(&this->character.animatable, char_mbf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 11;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MBF.ARC;1");
	
	const char **pathp = (const char *[]){
		"menubf.tim", //Mbf_ArcMain_Idle0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
