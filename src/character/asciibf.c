/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "asciibf.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Clucky character structure
enum
{
	AsciiBF_ArcMain_Idle0,
	AsciiBF_ArcMain_Idle1,
	AsciiBF_ArcMain_Idle2,
	AsciiBF_ArcMain_Idle3,
	AsciiBF_ArcMain_Idle4,
	AsciiBF_ArcMain_Left0,
	AsciiBF_ArcMain_Left1,
	AsciiBF_ArcMain_Down0,
	AsciiBF_ArcMain_Down1,
	AsciiBF_ArcMain_Up0,
	AsciiBF_ArcMain_Up1,
	AsciiBF_ArcMain_Right0,
	AsciiBF_ArcMain_Right1,
	
	AsciiBF_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[AsciiBF_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_AsciiBF;

//Clucky character definitions
static const CharFrame char_asciibf_frame[] = {
	{AsciiBF_ArcMain_Idle0, {  0,   0, 256, 256}, { 53,  92}}, //0 idle 1
	{AsciiBF_ArcMain_Idle1, {  0,   0, 256, 256}, { 53,  92}}, //1 idle 2
	{AsciiBF_ArcMain_Idle2, {  0,   0, 256, 256}, { 53,  92}}, //2 idle 3
	{AsciiBF_ArcMain_Idle3, {  0,   0, 256, 256}, { 53,  92}}, //3 idle 4
	{AsciiBF_ArcMain_Idle3, {  0,   0, 256, 256}, { 53,  92}}, //4 idle 5
	
	{AsciiBF_ArcMain_Left0, {  0,   0, 256, 256}, { 53,  92}}, //5 left 1
	{AsciiBF_ArcMain_Left1, {  0,   0, 256, 256}, { 53,  92}}, //6 left 2
	
	{AsciiBF_ArcMain_Down0, {  0,   0, 256, 256}, { 53,  92}}, //7 down 1
	{AsciiBF_ArcMain_Down1, {  0,   0, 256, 256}, { 53,  92}}, //8 down 2
	
	{AsciiBF_ArcMain_Up0, {  0,   0,  256, 256}, { 53,  92}}, //9 up 1
	{AsciiBF_ArcMain_Up1, {  0,   0,  256, 256}, { 53,  92}}, //10 up 2
	
	{AsciiBF_ArcMain_Right0, {  0,   0, 256, 256}, { 53,  92}}, //11 right 1
	{AsciiBF_ArcMain_Right1, {  0,   0, 256, 256}, { 53,  92}}, //12 right 2
};

static const Animation char_asciibf_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, 4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Clucky character functions
void Char_AsciiBF_SetFrame(void *user, u8 frame)
{
	Char_AsciiBF *this = (Char_AsciiBF*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_asciibf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_AsciiBF_Tick(Character *character)
{
	Char_AsciiBF *this = (Char_AsciiBF*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_AsciiBF_SetFrame);
	Character_Draw(character, &this->tex, &char_asciibf_frame[this->frame]);
}

void Char_AsciiBF_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_AsciiBF_Free(Character *character)
{
	Char_AsciiBF *this = (Char_AsciiBF*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_AsciiBF_New(fixed_t x, fixed_t y)
{
	//Allocate clucky object
	Char_AsciiBF *this = Mem_Alloc(sizeof(Char_AsciiBF));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_AsciiBF_New] Failed to allocate asciibf object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_AsciiBF_Tick;
	this->character.set_anim = Char_AsciiBF_SetAnim;
	this->character.free = Char_AsciiBF_Free;
	
	Animatable_Init(&this->character.animatable, char_asciibf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(-100,1);
	this->character.focus_y = FIXED_DEC(-64,1);
	this->character.focus_zoom = FIXED_DEC(1,2);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\ASCIIBF.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Clucky_ArcMain_Idle0
		"idle1.tim", //Clucky_ArcMain_Idle1
		"idle2.tim", //Clucky_ArcMain_Idle1
		"idle3.tim", //Clucky_ArcMain_Idle1
		"idle4.tim", //Clucky_ArcMain_Idle1
		"left0.tim",  //Clucky_ArcMain_Left
		"left1.tim",  //Clucky_ArcMain_Left
		"down0.tim",  //Clucky_ArcMain_Down
		"down1.tim",  //Clucky_ArcMain_Down
		"up0.tim",    //Clucky_ArcMain_Up
		"up1.tim",    //Clucky_ArcMain_Up
		"right0.tim", //Clucky_ArcMain_Right
		"right1.tim", //Clucky_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
