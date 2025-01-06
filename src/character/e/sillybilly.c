/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sillybilly.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

boolean smallphase;

//SillyBilly character structure
enum
{
	SillyBilly_ArcMain_Idle0,
	SillyBilly_ArcMain_Idle1,
	SillyBilly_ArcMain_Idle2,
	SillyBilly_ArcMain_Idle3,
	SillyBilly_ArcMain_Left0,
	SillyBilly_ArcMain_Left1,
	SillyBilly_ArcMain_Left2,
	SillyBilly_ArcMain_Left3,
	SillyBilly_ArcMain_Down0,
	SillyBilly_ArcMain_Down1,
	SillyBilly_ArcMain_Down2,
	SillyBilly_ArcMain_Down3,
	SillyBilly_ArcMain_Down4,
	SillyBilly_ArcMain_Right0,
	SillyBilly_ArcMain_Sidle0,
	SillyBilly_ArcMain_Sleft0,
	SillyBilly_ArcMain_Sup0,
	SillyBilly_ArcMain_Sright0,
	SillyBilly_ArcMain_Ungrow0,
	SillyBilly_ArcMain_Ungrow1,
	SillyBilly_ArcMain_Ungrow2,
	SillyBilly_ArcMain_Ungrow3,
	SillyBilly_ArcMain_Unsh0,
	SillyBilly_ArcMain_Unsh1,
	SillyBilly_ArcMain_Unsh2,
	SillyBilly_ArcMain_Unsh3,
	SillyBilly_ArcMain_Unsh4,
	SillyBilly_ArcMain_Unsh5,
	SillyBilly_ArcMain_Unsh6,
	SillyBilly_ArcMain_Unsh7,
	SillyBilly_ArcMain_Unsh8,
	
	SillyBilly_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[SillyBilly_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_SillyBilly;

//for X offsets - for right and + for left

//for Y offsets - for down and + for up
//SillyBilly character definitions
static const CharFrame char_sillybilly_frame[] = {

	//Big Silly Billy	
	
	{SillyBilly_ArcMain_Idle0, {  0,   0, 105, 174}, { 45, 184}}, //0 idle 1
	{SillyBilly_ArcMain_Idle0, {105,   2, 106, 172}, { 45, 182}}, //1 idle 2
	{SillyBilly_ArcMain_Idle1, {  0,   2, 105, 173}, { 45, 183}}, //2 idle 3
	{SillyBilly_ArcMain_Idle1, {105,   0, 103, 175}, { 45, 185}}, //3 idle 4
	{SillyBilly_ArcMain_Idle2, {  0,   0, 102, 177}, { 45, 187}}, //4 idle 5
	{SillyBilly_ArcMain_Idle2, {102,   0, 103, 177}, { 45, 187}}, //5 idle 6
	{SillyBilly_ArcMain_Idle3, {  0,   4, 104, 177}, { 45, 187}}, //6 idle 7
	
	{SillyBilly_ArcMain_Idle3, {104,   0, 123, 181}, { 45+20, 191}}, //7 left 1
	{SillyBilly_ArcMain_Left0, {  0,   1, 122, 181}, { 45+19, 191}}, //8 left 2
	{SillyBilly_ArcMain_Left1, {  0,   0, 117, 183}, { 45+13, 193}}, //9 left 3
	{SillyBilly_ArcMain_Left2, {  0,   0, 116, 182}, { 45+13, 192}}, //10 left 4
	{SillyBilly_ArcMain_Left3, {  0,   0, 116, 182}, { 45+13, 192}}, //11 left 5
	
	{SillyBilly_ArcMain_Down0, {  0,  83, 117, 147}, { 45+1, 157}}, //12 down 1
	{SillyBilly_ArcMain_Down1, {  0,  80, 116, 148}, { 45+2, 158}}, //13 down 2
	{SillyBilly_ArcMain_Down2, {  0,  72, 114, 151}, { 45+3, 161}}, //14 down 3
	{SillyBilly_ArcMain_Down3, {  0,  67, 115, 153}, { 45+2, 163}}, //15 down 4
	{SillyBilly_ArcMain_Down4, {  0,  69, 115, 152}, { 45+2, 162}}, //16 down 5
	
	{SillyBilly_ArcMain_Down0, {117,   0, 129, 230}, { 45+30, 240}}, //17 up 1
	{SillyBilly_ArcMain_Down1, {116,   0, 128, 228}, { 45+29, 238}}, //18 up 2
	{SillyBilly_ArcMain_Down2, {114,   0, 135, 223}, { 45+34, 233}}, //19 up 3
	{SillyBilly_ArcMain_Down3, {115,   0, 135, 220}, { 45+32, 230}}, //20 up 4
	{SillyBilly_ArcMain_Down4, {115,   0, 135, 221}, { 45+32, 231}}, //21 up 5
	
	{SillyBilly_ArcMain_Left0, {122,   0, 132, 182}, { 45, 192}}, //22 right 1
	{SillyBilly_ArcMain_Left1, {117,   6, 135, 177}, { 45, 187}}, //23 right 2
	{SillyBilly_ArcMain_Left2, {116,   5, 130, 177}, { 45, 187}}, //24 right 3
	{SillyBilly_ArcMain_Left3, {116,   3, 128, 179}, { 45, 189}}, //25 right 4
	{SillyBilly_ArcMain_Right0,{  0,   0, 129, 179}, { 45+1, 189}}, //26 right 5
	
	//Small Silly Billy
	
	{SillyBilly_ArcMain_Right0,{129,  12,  82,  83}, {45, 93}}, //27 small idle 1
	{SillyBilly_ArcMain_Right0,{129,  96,  83,  83}, {45, 93}}, //28 small idle 2
	{SillyBilly_ArcMain_Sidle0,{  0,   0,  83,  87}, {45, 97}}, //29 small idle 3
	{SillyBilly_ArcMain_Sidle0,{ 83,   1,  82,  86}, {45, 96}}, //30 small idle 4
	{SillyBilly_ArcMain_Sidle0,{165,   2,  81,  85}, {45, 95}}, //31 small idle 5
	{SillyBilly_ArcMain_Sidle0,{  0,  87,  81,  86}, {45, 96}}, //32 small idle 6
	
	{SillyBilly_ArcMain_Sidle0,{ 81,  87,  88,  91}, {45+11, 101}}, //33 small left 1
	{SillyBilly_ArcMain_Sleft0,{  0,   0,  89,  91}, {45+12, 101}}, //34 small left 2
	{SillyBilly_ArcMain_Sidle0,{169,  87,  86,  89}, {45+8, 99}}, //35 small left 3
	{SillyBilly_ArcMain_Sleft0,{ 89,   0,  85,  88}, {45+7, 98}}, //36 small left 4
	{SillyBilly_ArcMain_Sleft0,{  0,  91,  84,  89}, {45+6, 99}}, //37 small left 5
	
	{SillyBilly_ArcMain_Sidle0,{  0, 178,  87,  75}, {45, 85}}, //38 small down 1
	{SillyBilly_ArcMain_Sidle0,{ 87, 178,  86,  76}, {45, 86}}, //39 small down 2
	{SillyBilly_ArcMain_Sleft0,{ 84,  91,  84,  77}, {45, 87}}, //40 small down 3
	{SillyBilly_ArcMain_Sleft0,{ 84, 168,  84,  77}, {45, 87}}, //41 small down 4
	{SillyBilly_ArcMain_Sidle0,{173, 176,  83,  78}, {45, 88}}, //42 small down 5
	
	{SillyBilly_ArcMain_Sup0,  {  0,   0,  81,  97}, {45, 107}}, //43 small up 1
	{SillyBilly_ArcMain_Sup0,  { 81,   0,  81,  96}, {45, 106}}, //44 small up 2
	{SillyBilly_ArcMain_Sup0,  {162,   0,  82,  96}, {45, 106}}, //45 small up 3
	{SillyBilly_ArcMain_Sup0,  {  0,  97,  82,  95}, {45, 105}}, //46 small up 4
	{SillyBilly_ArcMain_Sup0,  { 82,  97,  83,  95}, {45, 105}}, //47 small up 5
	
	{SillyBilly_ArcMain_Sright0,{  0,   0,  95,  88}, {45, 98}}, //48 small right 1
	{SillyBilly_ArcMain_Sright0,{ 95,   0,  96,  87}, {45, 97}}, //49 small right 2
	{SillyBilly_ArcMain_Sright0,{  0,  88,  92,  86}, {45, 96}}, //50 small right 3
	{SillyBilly_ArcMain_Sright0,{ 92,  88,  91,  85}, {45, 95}}, //51 small right 4
	{SillyBilly_ArcMain_Sup0,   {165,  97,  91,  86}, {45, 96}}, //52 small right 5
	
	//Ungrow animtation
	
	{SillyBilly_ArcMain_Ungrow0,{  0,   0, 103, 176}, {45, 186}}, //53 ungrow 1
	{SillyBilly_ArcMain_Ungrow0,{103,   5, 106, 171}, {45, 181}}, //54 ungrow 2
	{SillyBilly_ArcMain_Ungrow1,{  0,   0, 105, 174}, {45, 184}}, //55 ungrow 3
	{SillyBilly_ArcMain_Ungrow1,{105,  15, 102, 159}, {45, 169}}, //56 ungrow 4
	{SillyBilly_ArcMain_Ungrow2,{  0,   0, 116, 160}, {45+13, 170}}, //57 ungrow 5
	{SillyBilly_ArcMain_Ungrow2,{116,  28, 117, 132}, {45+20, 142}}, //58 ungrow 6
	{SillyBilly_ArcMain_Ungrow3,{  0,   0, 110, 107}, {45+19, 117}}, //59 ungrow 7
	{SillyBilly_ArcMain_Ungrow3,{110,  14,  86,  93}, {45+1, 103}}, //60 ungrow 8
	{SillyBilly_ArcMain_Ungrow3,{  0, 107,  85,  82}, {45,  92}}, //61 ungrow 9
	
	//Unshrink animation
	
	{SillyBilly_ArcMain_Unsh0,{109,   0,  82,  79}, {45,  89}}, //62 unshrink 1
	{SillyBilly_ArcMain_Unsh0,{  0,   0, 109, 138}, {45+27, 148}}, //63 unshrink 2
	{SillyBilly_ArcMain_Unsh0,{130, 121, 116, 131}, {45, 141}}, //64 unshrink 3
	{SillyBilly_ArcMain_Unsh1,{  0,   0, 124, 136}, {45+27, 146}}, //65 unshrink 4
	{SillyBilly_ArcMain_Unsh1,{124,   0, 127, 134}, {45+25, 144}}, //66 unshrink 5
	{SillyBilly_ArcMain_Unsh0,{  0, 138, 130, 111}, {45, 121}}, //67 unshrink 6
	{SillyBilly_ArcMain_Unsh2,{  0,   0, 146, 107}, {45, 117}}, //68 unshrink 7
	{SillyBilly_ArcMain_Unsh2,{  0, 107, 146, 107}, {45, 117}}, //69 unshrink 8
	{SillyBilly_ArcMain_Unsh3,{  0,   0, 142, 107}, {45, 117}}, //70 unshrink 9
	{SillyBilly_ArcMain_Unsh1,{  0, 150, 138, 106}, {45, 116}}, //71 unshrink 10
	{SillyBilly_ArcMain_Unsh1,{138, 134, 117, 122}, {45, 132}}, //72 unshrink 11
	{SillyBilly_ArcMain_Unsh4,{  0,   0, 116, 146}, {45+4, 156}}, //73 unshrink 12
	{SillyBilly_ArcMain_Unsh4,{116,   0, 117, 204}, {45+18, 214}}, //74 unshrink 13
	{SillyBilly_ArcMain_Unsh3,{142,   0, 113, 203}, {45+13, 213}}, //75 unshrink 14
	{SillyBilly_ArcMain_Unsh5,{  0,   0, 118, 199}, {45+17, 209}}, //76 unshrink 15
	{SillyBilly_ArcMain_Unsh5,{118,   0, 125, 167}, {45+23, 177}}, //78 unshrink 16
	{SillyBilly_ArcMain_Unsh6,{  0,   0, 127, 147}, {45+22, 157}}, //79 unshrink 17
	{SillyBilly_ArcMain_Unsh6,{127,   0, 126, 152}, {45+9, 162}}, //80 unshrink 18
	{SillyBilly_ArcMain_Unsh7,{  0,   0, 123, 152}, {45+6, 162}}, //81 unshrink 19
	{SillyBilly_ArcMain_Unsh7,{123,   0, 119, 150}, {45+6, 160}}, //82 unshrink 20
	{SillyBilly_ArcMain_Unsh8,{  0,   0, 118, 151}, {45+6, 161}}, //83 unshrink 21
};

static const Animation char_sillybilly_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, ASCR_BACK, 1}}, //CharAnim_Idle
	{1, (const u8[]){ 7, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, ASCR_BACK, 3}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{1, (const u8[]){ 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, ASCR_BACK, 3}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){ 17, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, ASCR_BACK, 3}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{1, (const u8[]){ 22, 23, 23, 24, 24, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, ASCR_BACK, 3}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{1, (const u8[]){ 32, 27, 27, 28, 28, 29, 29, 62, 63, 64, 65, 65, 66, 66, 67, 67, 68, 69, 70, 70, 71, 71, 72, 73, 74, 75, 76, 76, 77, 77, 78, 79, 79, 80, 80, 81, 81, 82, 82, 82, ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
};

static const Animation char_smallsillybilly_anim[CharAnim_Max] = {
	{2, (const u8[]){ 27, 28, 29, 30, 31, 32, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 33, 34, 35, 36, 37, ASCR_BACK, 3}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 38,  39, 40, 41, 42, ASCR_BACK, 3}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 43,  44, 45, 46, 47, ASCR_BACK, 3}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 48, 49, 50, 51, 52, ASCR_BACK, 3}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{2, (const u8[]){ 53, 54, 55, 56, 57, 58, 59, 60, 61, ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
};

//SillyBilly character functions
void Char_SillyBilly_SetFrame(void *user, u8 frame)
{
	Char_SillyBilly *this = (Char_SillyBilly*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_sillybilly_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_SillyBilly_Tick(Character *character)
{
	Char_SillyBilly *this = (Char_SillyBilly*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
		
	if (stage.flag & STAGE_FLAG_JUST_STEP)
    	{   //Stage specific animations
		if (stage.note_scroll >= 0)
		{
			switch (stage.stage_id)
			{
				case StageId_8_3: //Animations
				{
					switch (stage.song_step)
					{
						case 112:
							character->set_anim(character, CharAnim_Up);
							break;
						case 116:
							character->set_anim(character, CharAnim_Up);
							break;
						case 120:
							character->set_anim(character, CharAnim_Up);
							break;
						case 124:
							character->set_anim(character, CharAnim_Up);
							break;
						case 1408:
							character->set_anim(character, CharAnim_UnGrow);
							smallphase =1;
							break;
						case 2044:
							character->set_anim(character, CharAnim_UnShrink);
							smallphase =0;
							break;
					}
					break;
				}
				default:
					break;
			}
		}
	}
	if(smallphase==1)
	{
		Animatable_Animate(&character->animatable2, (void*)this, Char_SillyBilly_SetFrame);
		Character_Draw(character, &this->tex, &char_sillybilly_frame[this->frame]);
		this->character.focus_x = FIXED_DEC(35,1);
		this->character.focus_y = FIXED_DEC(-80,1);
		this->character.focus_zoom = FIXED_DEC(7,6);
	}
	else
	{
		Animatable_Animate(&character->animatable, (void*)this, Char_SillyBilly_SetFrame);
		Character_Draw(character, &this->tex, &char_sillybilly_frame[this->frame]);
		this->character.focus_x = FIXED_DEC(25,1);
		this->character.focus_y = FIXED_DEC(-125,1);
		this->character.focus_zoom = FIXED_DEC(9,8);
	}
}

void Char_SillyBilly_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Animatable_SetAnim(&character->animatable2, anim);
	Character_CheckStartSing(character);
}

void Char_SillyBilly_Free(Character *character)
{
	Char_SillyBilly *this = (Char_SillyBilly*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_SillyBilly_New(fixed_t x, fixed_t y)
{
	//Allocate sillybilly object
	Char_SillyBilly *this = Mem_Alloc(sizeof(Char_SillyBilly));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_SillyBilly_New] Failed to allocate sillybilly object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_SillyBilly_Tick;
	this->character.set_anim = Char_SillyBilly_SetAnim;
	this->character.free = Char_SillyBilly_Free;
	
	Animatable_Init(&this->character.animatable, char_sillybilly_anim);
	Animatable_Init(&this->character.animatable2, char_smallsillybilly_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(35,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(7,6);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BILLY.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //SillyBilly_ArcMain_Idle0
		"idle1.tim", //SillyBilly_ArcMain_Idle1
		"idle2.tim",
		"idle3.tim",
		"left0.tim",  //SillyBilly_ArcMain_Left
		"left1.tim",  //SillyBilly_ArcMain_Left
		"left2.tim",  //SillyBilly_ArcMain_Left
		"left3.tim",  //SillyBilly_ArcMain_Left
		"down0.tim",  //SillyBilly_ArcMain_Down
		"down1.tim",  //SillyBilly_ArcMain_Down
		"down2.tim",  //SillyBilly_ArcMain_Down
		"down3.tim",  //SillyBilly_ArcMain_Down
		"down4.tim",  //SillyBilly_ArcMain_Down
		"right0.tim", //SillyBilly_ArcMain_Right
		"sidle0.tim",
		"sleft0.tim",
		"sup0.tim",
		"sright0.tim",
		"ungrow0.tim",
		"ungrow1.tim",
		"ungrow2.tim",
		"ungrow3.tim",
		"unsh0.tim",
		"unsh1.tim",
		"unsh2.tim",
		"unsh3.tim",
		"unsh4.tim",
		"unsh5.tim",
		"unsh6.tim",
		"unsh7.tim",
		"unsh8.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	smallphase=false;
	
	return (Character*)this;
}
