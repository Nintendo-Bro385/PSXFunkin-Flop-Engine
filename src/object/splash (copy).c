/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "splash.h"

#include "../mem.h"
#include "../timer.h"
#include "../random.h"
#include "../mutil.h"

//Splash object functions
boolean Obj_Splash_Tick(Object *obj)
{
	Obj_Splash *this = (Obj_Splash*)obj;
	
	//Move
	fixed_t lx = this->x - this->xsp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	fixed_t ly = this->y - this->ysp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	this->x += this->xsp;
	this->y += this->ysp;
	this->xsp = this->xsp * 5 / 6;
	this->ysp = this->ysp * 5 / 6;
	
	//Scale down
	fixed_t scale = FIXED_MUL(FIXED_UNIT - FIXED_MUL(this->size, this->size), FIXED_DEC(8,10));
	this->size += FIXED_UNIT / 25;
	
	//Draw plubbie
	RECT plub_src = {120 + (this->colour << 2), 224, 4, 4};
	RECT_FIXED plub_dst = {
		this->x - (scale << 2),
		this->y - (scale << 2),
		scale << 3,
		scale << 3
	};
	
	Stage_DrawTex(&stage.tex_hud0, &plub_src, &plub_dst, stage.bump);
	
	//Draw tail
	fixed_t tx =  this->sin * scale >> 6;
	fixed_t ty = -this->cos * scale >> 6;
	
	RECT tail_src = {120 + (this->colour << 2), 228, 4, 4};
	POINT_FIXED tl = {this->x - tx, this->y - ty};
	POINT_FIXED tr = {this->x + tx, this->y + ty};
	POINT_FIXED bl = {lx - tx, ly - ty};
	POINT_FIXED br = {lx + tx, ly + ty};
	
	Stage_DrawTexArb(&stage.tex_hud0, &tail_src, &tl, &tr, &bl, &br, stage.bump);
	
	return this->size >= FIXED_UNIT;
}

void Obj_Splash_Free(Object *obj)
{
	(void)obj;
}

Obj_Splash *Obj_Splash_New(fixed_t x, fixed_t y, u8 colour)
{
	//Allocate new object
	Obj_Splash *this = (Obj_Splash*)Mem_Alloc(sizeof(Obj_Splash));
	if (this == NULL)
		return NULL;
	
	//Set object functions
	this->obj.tick = Obj_Splash_Tick;
	this->obj.free = Obj_Splash_Free;
	
	//Initialize position
	u8 angle = Random8();
	fixed_t speed = RandomRange(FIXED_DEC(35,10), FIXED_DEC(45,10));
	this->xsp = ((this->cos = MUtil_Cos(angle)) * speed) >> 8;
	this->ysp = ((this->sin = MUtil_Sin(angle)) * speed) >> 8;
	this->size = 0;
	
	this->x = x + this->xsp;
	this->y = y + this->ysp;
	
	this->colour = colour;
	
	return this;
}
//Splash object functions
boolean Obj_Good_Tick(Object *obj)
{
	Obj_Good *this = (Obj_Good*)obj;
	
	//Move
	fixed_t lx = this->x - this->xsp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	fixed_t ly = this->y - this->ysp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	this->x += this->xsp;
	this->y += this->ysp;
	this->xsp = this->xsp * 5 / 6;
	this->ysp = this->ysp * 5 / 6;
	
	//Scale down
	fixed_t scale = FIXED_MUL(FIXED_UNIT - FIXED_MUL(this->size, this->size), FIXED_DEC(8,10));
	this->size += FIXED_UNIT / 25;
	
	//Draw plubbie
	RECT plub_src = {136 + (this->colour << 2), 224, 4, 4};
	RECT_FIXED plub_dst = {
		this->x - (scale << 2),
		this->y - (scale << 2),
		scale << 3,
		scale << 3
	};
	
	Stage_DrawTex(&stage.tex_hud0, &plub_src, &plub_dst, stage.bump);
	
	//Draw tail
	fixed_t tx =  this->sin * scale >> 6;
	fixed_t ty = -this->cos * scale >> 6;
	
	RECT tail_src = {136 + (this->colour << 2), 228, 4, 4};
	POINT_FIXED tl = {this->x - tx, this->y - ty};
	POINT_FIXED tr = {this->x + tx, this->y + ty};
	POINT_FIXED bl = {lx - tx, ly - ty};
	POINT_FIXED br = {lx + tx, ly + ty};
	
	Stage_DrawTexArb(&stage.tex_hud0, &tail_src, &tl, &tr, &bl, &br, stage.bump);
	
	return this->size >= FIXED_UNIT;
}

void Obj_Good_Free(Object *obj)
{
	(void)obj;
}

Obj_Good *Obj_Good_New(fixed_t x, fixed_t y, u8 colour)
{
	//Allocate new object
	Obj_Good *this = (Obj_Good*)Mem_Alloc(sizeof(Obj_Good));
	if (this == NULL)
		return NULL;
	
	//Set object functions
	this->obj.tick = Obj_Good_Tick;
	this->obj.free = Obj_Good_Free;
	
	//Initialize position
	u8 angle = Random8();
	fixed_t speed = RandomRange(FIXED_DEC(35,10), FIXED_DEC(45,10));
	this->xsp = ((this->cos = MUtil_Cos(angle)) * speed) >> 8;
	this->ysp = ((this->sin = MUtil_Sin(angle)) * speed) >> 8;
	this->size = 0;
	
	this->x = x + this->xsp;
	this->y = y + this->ysp;
	
	this->colour = colour;
	
	return this;
}
//Splash object functions
boolean Obj_Bad_Tick(Object *obj)
{
	Obj_Bad *this = (Obj_Bad*)obj;
	
	//Move
	fixed_t lx = this->x - this->xsp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	fixed_t ly = this->y - this->ysp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	this->x += this->xsp;
	this->y += this->ysp;
	this->xsp = this->xsp * 5 / 6;
	this->ysp = this->ysp * 5 / 6;
	
	//Scale down
	fixed_t scale = FIXED_MUL(FIXED_UNIT - FIXED_MUL(this->size, this->size), FIXED_DEC(8,10));
	this->size += FIXED_UNIT / 25;
	
	//Draw plubbie
	RECT plub_src = {152 + (this->colour << 2), 224, 4, 4};
	RECT_FIXED plub_dst = {
		this->x - (scale << 2),
		this->y - (scale << 2),
		scale << 3,
		scale << 3
	};
	
	Stage_DrawTex(&stage.tex_hud0, &plub_src, &plub_dst, stage.bump);
	
	//Draw tail
	fixed_t tx =  this->sin * scale >> 6;
	fixed_t ty = -this->cos * scale >> 6;
	
	RECT tail_src = {152 + (this->colour << 2), 228, 4, 4};
	POINT_FIXED tl = {this->x - tx, this->y - ty};
	POINT_FIXED tr = {this->x + tx, this->y + ty};
	POINT_FIXED bl = {lx - tx, ly - ty};
	POINT_FIXED br = {lx + tx, ly + ty};
	
	Stage_DrawTexArb(&stage.tex_hud0, &tail_src, &tl, &tr, &bl, &br, stage.bump);
	
	return this->size >= FIXED_UNIT;
}

void Obj_Bad_Free(Object *obj)
{
	(void)obj;
}

Obj_Bad *Obj_Bad_New(fixed_t x, fixed_t y, u8 colour)
{
	//Allocate new object
	Obj_Bad *this = (Obj_Bad*)Mem_Alloc(sizeof(Obj_Bad));
	if (this == NULL)
		return NULL;
	
	//Set object functions
	this->obj.tick = Obj_Bad_Tick;
	this->obj.free = Obj_Bad_Free;
	
	//Initialize position
	u8 angle = Random8();
	fixed_t speed = RandomRange(FIXED_DEC(35,10), FIXED_DEC(45,10));
	this->xsp = ((this->cos = MUtil_Cos(angle)) * speed) >> 8;
	this->ysp = ((this->sin = MUtil_Sin(angle)) * speed) >> 8;
	this->size = 0;
	
	this->x = x + this->xsp;
	this->y = y + this->ysp;
	
	this->colour = colour;
	
	return this;
}
//Splash object functions
boolean Obj_Shit_Tick(Object *obj)
{
	Obj_Shit *this = (Obj_Shit*)obj;
	
	//Move
	fixed_t lx = this->x - this->xsp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	fixed_t ly = this->y - this->ysp * (2 + ((this->size * 4) >> FIXED_SHIFT));
	this->x += this->xsp;
	this->y += this->ysp;
	this->xsp = this->xsp * 5 / 6;
	this->ysp = this->ysp * 5 / 6;
	
	//Scale down
	fixed_t scale = FIXED_MUL(FIXED_UNIT - FIXED_MUL(this->size, this->size), FIXED_DEC(8,10));
	this->size += FIXED_UNIT / 25;
	
	//Draw plubbie
	RECT plub_src = {168 + (this->colour << 2), 224, 4, 4};
	RECT_FIXED plub_dst = {
		this->x - (scale << 2),
		this->y - (scale << 2),
		scale << 3,
		scale << 3
	};
	
	Stage_DrawTex(&stage.tex_hud0, &plub_src, &plub_dst, stage.bump);
	
	//Draw tail
	fixed_t tx =  this->sin * scale >> 6;
	fixed_t ty = -this->cos * scale >> 6;
	
	RECT tail_src = {168 + (this->colour << 2), 228, 4, 4};
	POINT_FIXED tl = {this->x - tx, this->y - ty};
	POINT_FIXED tr = {this->x + tx, this->y + ty};
	POINT_FIXED bl = {lx - tx, ly - ty};
	POINT_FIXED br = {lx + tx, ly + ty};
	
	Stage_DrawTexArb(&stage.tex_hud0, &tail_src, &tl, &tr, &bl, &br, stage.bump);
	
	return this->size >= FIXED_UNIT;
}

void Obj_Shit_Free(Object *obj)
{
	(void)obj;
}

Obj_Shit *Obj_Shit_New(fixed_t x, fixed_t y, u8 colour)
{
	//Allocate new object
	Obj_Shit *this = (Obj_Shit*)Mem_Alloc(sizeof(Obj_Shit));
	if (this == NULL)
		return NULL;
	
	//Set object functions
	this->obj.tick = Obj_Shit_Tick;
	this->obj.free = Obj_Shit_Free;
	
	//Initialize position
	u8 angle = Random8();
	fixed_t speed = RandomRange(FIXED_DEC(35,10), FIXED_DEC(45,10));
	this->xsp = ((this->cos = MUtil_Cos(angle)) * speed) >> 8;
	this->ysp = ((this->sin = MUtil_Sin(angle)) * speed) >> 8;
	this->size = 0;
	
	this->x = x + this->xsp;
	this->y = y + this->ysp;
	
	this->colour = colour;
	
	return this;
}
