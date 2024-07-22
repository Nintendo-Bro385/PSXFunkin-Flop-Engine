/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_SPLASH_H
#define PSXF_GUARD_SPLASH_H

#include "../object.h"
#include "../stage.h"

//Splash object structure
typedef struct
{
	//Object base structure
	Object obj;
	
	//Splash state
	u8 colour;
	
	fixed_t x, y, xsp, ysp, size, sin, cos;
} Obj_Splash;
typedef struct
{
	//Object base structure
	Object obj;
	
	//Splash state
	u8 colour;
	
	fixed_t x, y, xsp, ysp, size, sin, cos;
} Obj_Good;
typedef struct
{
	//Object base structure
	Object obj;
	
	//Splash state
	u8 colour;
	
	fixed_t x, y, xsp, ysp, size, sin, cos;
} Obj_Bad;
typedef struct
{
	//Object base structure
	Object obj;
	
	//Splash state
	u8 colour;
	
	fixed_t x, y, xsp, ysp, size, sin, cos;
} Obj_Shit;

//Splash object functions
Obj_Splash *Obj_Splash_New(fixed_t x, fixed_t y, u8 colour);
Obj_Good *Obj_Good_New(fixed_t x, fixed_t y, u8 colour);
Obj_Bad *Obj_Bad_New(fixed_t x, fixed_t y, u8 colour);
Obj_Shit *Obj_Shit_New(fixed_t x, fixed_t y, u8 colour);

#endif
