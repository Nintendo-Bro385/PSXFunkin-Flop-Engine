/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "mgf.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//GF Weeb character structure
enum
{
	Mgf_ArcMain_Menugf,
	
	Mgf_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Mgf_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Mgf;

//GF character definitions
static const CharFrame char_mgf_frame[] = {
	{Mgf_ArcMain_Menugf, {  0,   0, 86,  79}, { 52,  92}}, //1 left 0
	{Mgf_ArcMain_Menugf, {  0,   79, 86,  79}, { 52,  92}}, //2 left 0
	{Mgf_ArcMain_Menugf, {  0,   158, 86,  79}, { 52,  92}}, //3 left 0
	{Mgf_ArcMain_Menugf, {  86,   0, 86,  79}, { 52,  92}}, //4 left 0
	{Mgf_ArcMain_Menugf, {  86,   79, 86,  79}, { 52,  92}}, //5 left 0
	{Mgf_ArcMain_Menugf, {  86,   158, 86,  79}, { 52,  92}}, //6 left 0
	{Mgf_ArcMain_Menugf, {  172,   0, 86,  79}, { 52,  92}}, //7 left 0
	{Mgf_ArcMain_Menugf, {  172,   79, 86,  79}, { 52,  92}}, //8 left 0
	{Mgf_ArcMain_Menugf, {  172,   158, 88,  79}, { 52,  92}}, //9 left 0
};

static const Animation char_mgf_anim[CharAnim_Max] = {
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_Idle
	{1, (const u8[]){ 2,  2,  3,  3,  4,  4,  5,  5,  6,  6, ASCR_BACK, 1}}, //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_UpAlt
	{1, (const u8[]){ 7,  7,  8,  8,  0,  0,  1,  1,  ASCR_BACK, 1}}, //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_RightAlt
};

//GF Weeb character functions
void Char_Mgf_SetFrame(void *user, u8 frame)
{
	Char_Mgf *this = (Char_Mgf*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_mgf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Mgf_Tick(Character *character)
{
	Char_Mgf *this = (Char_Mgf*)character;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		//Perform dance
		if ((stage.song_step % stage.gf_speed) == 0)
		{
			//Switch animation
			if (character->animatable.anim == CharAnim_Left)
				character->set_anim(character, CharAnim_Right);
			else
				character->set_anim(character, CharAnim_Left);
		}
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Mgf_SetFrame);
	Character_Draw(character, &this->tex, &char_mgf_frame[this->frame]);
}

void Char_Mgf_SetAnim(Character *character, u8 anim)
{
	//Set animation
	if (anim != CharAnim_Idle && anim != CharAnim_Left && anim != CharAnim_Right)
		return;
	Animatable_SetAnim(&character->animatable, anim);
}

void Char_Mgf_Free(Character *character)
{
	Char_Mgf *this = (Char_Mgf*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Mgf_New(fixed_t x, fixed_t y)
{
	//Allocate gf weeb object
	Char_Mgf *this = Mem_Alloc(sizeof(Char_Mgf));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Mgf_New] Failed to allocate mgf object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Mgf_Tick;
	this->character.set_anim = Char_Mgf_SetAnim;
	this->character.free = Char_Mgf_Free;
	
	Animatable_Init(&this->character.animatable, char_mgf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(16,1);
	this->character.focus_y = FIXED_DEC(-50,1);
	this->character.focus_zoom = FIXED_DEC(13,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MGF.ARC;1");
	
	const char **pathp = (const char *[]){
		"menugf.tim",  //Mgf_ArcMain_Menugf
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
