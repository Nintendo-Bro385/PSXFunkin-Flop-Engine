/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "speakerxmas.h"

#include "../io.h"
#include "../stage.h"
#include "../timer.h"

//Speaker functions
void Speaker_Init(Speaker *this)
{
	//Initialize speaker state
	this->bump = 0;
	
	//Load speaker graphics
	Gfx_LoadTex(&this->tex, IO_Read("\\CHAR\\SPEAKER.TIM;1"), GFX_LOADTEX_FREE);
}
static const CharFrame light_frame[] = {
	{0, {  0,   0, 200, 114}, { 71,  18}}, //0 light 1
	{0, {  0, 115, 200, 114}, { 71,  17}}, //1 light 2
	{1, {  0,   0, 200, 114}, { 71,  18}}, //2 light 3
	{1, {  0, 115, 200, 114}, { 71,  17}}, //3 light 4
};

static const Animation light_anim[] = {
	{2, (const u8[]){0, 1, 2, 3, ASCR_BACK, 1}}, //light
};

//Light state
static IO_Data week5_arc_light_ptr[2];
static Gfx_Tex week5_tex_light;
static u8 week5_light_frame, week5_light_tex_id;
void Speaker_Bump(Speaker *this)
{
	//Set bump
	this->bump = FIXED_DEC(4,1) / 24 - 1;
}

void Speaker_Tick(Speaker *this, fixed_t x, fixed_t y, fixed_t parallax)
{
	//Get frame to use according to bump
	u8 frame;
	if (this->bump > 0)
	{
		frame = (this->bump * 24) >> FIXED_SHIFT;
		this->bump -= timer_dt;
	}
	else
	{
		frame = 0;
	}
	
	//Draw speakers
	static const struct SpeakerPiece
	{
		u8 rect[4];
		u8 ox, oy;
	} speaker_draw[4][2] = {
		{ //bump 0
			{{ 97,  88, 158, 88},   0,  0},
			{{  0, 176,  18, 56}, 158, 32},
		},
		{ //bump 1
			{{176,   0,  79, 88},   0,  0},
			{{  0,  88,  97, 88},  79,  0},
		},
		{ //bump 2
			{{  0,   0,  88, 88},   0,  0},
			{{ 88,   0,  88, 88},  88,  0},
		},
		{ //bump 3
			{{  0,   0,  88, 88},   0,  0},
			{{ 88,   0,  88, 88},  88,  0},
		}
	};
	
	const struct SpeakerPiece *piece = speaker_draw[frame];
	for (int i = 0; i < 2; i++, piece++)
	{
		//Draw piece
		RECT piece_src = {piece->rect[0], piece->rect[1], piece->rect[2], piece->rect[3]};
		RECT_FIXED piece_dst = {
			x - FIXED_DEC(88,1) + ((fixed_t)piece->ox << FIXED_SHIFT) - FIXED_MUL(stage.camera.x, parallax),
			y + ((fixed_t)piece->oy << FIXED_SHIFT) - FIXED_MUL(stage.camera.y, parallax),
			(fixed_t)piece->rect[2] << FIXED_SHIFT,
			(fixed_t)piece->rect[3] << FIXED_SHIFT,
		};
		
		Stage_DrawTex(&this->tex, &piece_src, &piece_dst, stage.camera.bzoom);
	}
}
