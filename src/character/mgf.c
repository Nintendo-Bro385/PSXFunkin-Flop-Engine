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
	Mgf_ArcMain_Menugf0,
	Mgf_ArcMain_Menugf1,
	
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
	{Mgf_ArcMain_Menugf0, {  1,   0,  86,  78}, { 37,  72}}, //0 bop left 1
	{Mgf_ArcMain_Menugf0, { 87,   0,  86,  78}, { 37,  72}}, //1 bop left 2
	{Mgf_ArcMain_Menugf0, {  1,  82,  86,  78}, { 37,  72}}, //2 bop left 3
	{Mgf_ArcMain_Menugf0, { 87,  82,  86,  78}, { 37,  72}}, //3 bop left 4
	{Mgf_ArcMain_Menugf0, {  0, 164,  86,  79}, { 37,  73}}, //4 bop left 5
	{Mgf_ArcMain_Menugf0, { 86, 164,  86,  79}, { 37,  73}}, //5 bop left 6
	
	{Mgf_ArcMain_Menugf1, {  3,   1,  85,  77}, { 36,  71}}, //6 bop right 1
	{Mgf_ArcMain_Menugf1, { 88,   1,  86,  77}, { 37,  71}}, //7 bop right 2
	{Mgf_ArcMain_Menugf1, {  2,  80,  86,  77}, { 37,  71}}, //8 bop right 3
	{Mgf_ArcMain_Menugf1, { 88,  80,  86,  77}, { 37,  71}}, //9 bop right 4
	{Mgf_ArcMain_Menugf1, {  2, 159,  86,  79}, { 37,  73}}, //10 bop right 5
	{Mgf_ArcMain_Menugf1, { 88, 160,  86,  78}, { 37,  72}}, //11 bop right 6
	
};

static const Animation char_mgf_anim[CharAnim_Max] = {
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                        //CharAnim_Idle
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                                   //CharAnim_Left
	{1, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5, ASCR_BACK, 1}}, //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                                    //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                          //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                                     //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                                     //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}},                                     //CharAnim_Right
	{1, (const u8[]){6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11,  ASCR_BACK, 1}}, //CharAnim_RightAlt
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
	
	if ((stage.flag & STAGE_FLAG_JUST_STEP) && (stage.song_step % stage.gf_speed) == 0)
			{
				//Switch animation
				if (character->animatable.anim == CharAnim_LeftAlt)
					character->set_anim(character, CharAnim_RightAlt);
				else
					character->set_anim(character, CharAnim_LeftAlt);
				
			}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Mgf_SetFrame);
	Character_Draw(character, &this->tex, &char_mgf_frame[this->frame]);
}

void Char_Mgf_SetAnim(Character *character, u8 anim)
{
	//Set animation
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
		"menugf0.tim",  //Mgf_ArcMain_Menugf
		"menugf1.tim",  //Mgf_ArcMain_Menugf
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
