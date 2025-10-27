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

	sillybilly_ArcMain_sing0,
	sillybilly_ArcMain_sing1,
	sillybilly_ArcMain_sing2,
	sillybilly_ArcMain_sing3,
	sillybilly_ArcMain_sing4,
	sillybilly_ArcMain_sing5,
	sillybilly_ArcMain_sing6,
	sillybilly_ArcMain_sing7,
	sillybilly_ArcMain_sing9,
	sillybilly_ArcMain_sing10,
	sillybilly_ArcMain_sing11,
	sillybilly_ArcMain_sing13,
	sillybilly_ArcMain_sing14,
	sillybilly_ArcMain_sing15,
	sillybilly_ArcMain_sing16,
	sillybilly_ArcMain_sing18,
	sillybilly_ArcMain_sing20,
	sillybilly_ArcMain_sing21,
	sillybilly_ArcMain_sing22,
	sillybilly_ArcMain_sing23,
	sillybilly_ArcMain_sing24,
	sillybilly_ArcMain_sing26,
	sillybilly_ArcMain_sing27,
	sillybilly_ArcMain_sing28,
	sillybilly_ArcMain_sing29,
	sillybilly_ArcMain_sing30,
	sillybilly_ArcMain_sing31,
	sillybilly_ArcMain_sing32,
	sillybilly_ArcMain_sing33,
	sillybilly_ArcMain_sing35,
	sillybilly_ArcMain_sing36,
	sillybilly_ArcMain_sing37,
	sillybilly_ArcMain_sing39,
	sillybilly_ArcMain_sing41,
	sillybilly_ArcMain_sing43,
	sillybilly_ArcMain_sing44,

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

//Big billy 53

//SillyBilly character definitions
static const CharFrame char_sillybilly_frame[] = {

	//Big Silly Billy

	{SillyBilly_ArcMain_Idle0, {  0,   0, 105, 174}, { 45, 184}}, //0 idle 1
	{SillyBilly_ArcMain_Idle0, {105,   2, 106, 172}, { 45+1, 182}}, //1 idle 2
	{SillyBilly_ArcMain_Idle1, {  0,   2, 105, 173}, { 45, 183}}, //2 idle 3
	{SillyBilly_ArcMain_Idle1, {105,   0, 103, 175}, { 45-1, 185}}, //3 idle 4
	{SillyBilly_ArcMain_Idle2, {  0,   0, 102, 177}, { 45-2, 187}}, //4 idle 5
	{SillyBilly_ArcMain_Idle2, {102,   0, 103, 177}, { 45-2, 187}}, //5 idle 6
	{SillyBilly_ArcMain_Idle3, {  0,   4, 104, 177}, { 45-2, 187}}, //6 idle 7

	{SillyBilly_ArcMain_Idle3, {104,   0, 123, 181}, { 45+20, 191}}, //7 left 1
	{SillyBilly_ArcMain_Left0, {  0,   1, 122, 181}, { 45+19, 191}}, //8 left 2
	{SillyBilly_ArcMain_Left1, {  0,   0, 117, 183}, { 45+13, 193}}, //9 left 3
	{SillyBilly_ArcMain_Left2, {  0,   0, 116, 182}, { 45+13, 192}}, //10 left 4
	{SillyBilly_ArcMain_Left3, {  0,   0, 116, 182}, { 45+13, 192}}, //11 left 5

	{SillyBilly_ArcMain_Down0, {  0,  88, 117, 147}, { 45+3, 157}}, //12 down 1
	{SillyBilly_ArcMain_Down1, {  0,  87, 116, 148}, { 45+3, 158}}, //13 down 2
	{SillyBilly_ArcMain_Down2, {  0,  86, 114, 151}, { 45+4, 161}}, //14 down 3
	{SillyBilly_ArcMain_Down3, {  0,  85, 115, 153}, { 45+2, 163}}, //15 down 4
	{SillyBilly_ArcMain_Down4, {  0,  86, 115, 152}, { 45+2, 162}}, //16 down 5

	{SillyBilly_ArcMain_Down0, {117,   5, 129, 230}, { 45+26, 240}}, //17 up 1
	{SillyBilly_ArcMain_Down1, {116,   7, 128, 228}, { 45+25, 238}}, //18 up 2
	{SillyBilly_ArcMain_Down2, {114,  14, 135, 223}, { 45+31, 233}}, //19 up 3
	{SillyBilly_ArcMain_Down3, {115,  18, 135, 220}, { 45+30, 230}}, //20 up 4
	{SillyBilly_ArcMain_Down4, {115,  17, 135, 221}, { 45+30, 231}}, //21 up 5

	{SillyBilly_ArcMain_Left0, {122,   0, 132, 182}, { 45-3, 192}}, //22 right 1
	{SillyBilly_ArcMain_Left1, {117,   6, 135, 177}, { 45-3, 187}}, //23 right 2
	{SillyBilly_ArcMain_Left2, {116,   5, 130, 177}, { 45-2, 187}}, //24 right 3
	{SillyBilly_ArcMain_Left3, {116,   3, 128, 179}, { 45-2, 189}}, //25 right 4
	{SillyBilly_ArcMain_Right0,{  0,   0, 129, 179}, { 45-2, 189}}, //26 right 5

	//Small Silly Billy

	{SillyBilly_ArcMain_Right0,{129,  12,  82,  83}, {45-9-5, 93+3}}, //27 small idle 1
	{SillyBilly_ArcMain_Right0,{129,  96,  83,  83}, {45-8-5, 93+3}}, //28 small idle 2
	{SillyBilly_ArcMain_Sidle0,{  0,   0,  83,  87}, {45-9-5, 97+3}}, //29 small idle 3
	{SillyBilly_ArcMain_Sidle0,{ 83,   1,  82,  86}, {45-9-5, 96+3}}, //30 small idle 4
	{SillyBilly_ArcMain_Sidle0,{165,   2,  81,  85}, {45-10-5, 95+3}}, //31 small idle 5
	{SillyBilly_ArcMain_Sidle0,{  0,  87,  81,  86}, {45-10-5, 96+3}}, //32 small idle 6

	{SillyBilly_ArcMain_Sidle0,{ 81,  87,  88,  91}, {45+2-5, 101+3}}, //33 small left 1
	{SillyBilly_ArcMain_Sleft0,{  0,   0,  89,  91}, {45+3-5, 101+3}}, //34 small left 2
	{SillyBilly_ArcMain_Sidle0,{169,  87,  86,  89}, {45-2-5, 99+3}}, //35 small left 3
	{SillyBilly_ArcMain_Sleft0,{ 89,   3,  85,  88}, {45-3-5, 98+3}}, //36 small left 4
	{SillyBilly_ArcMain_Sleft0,{  0,  91,  84,  89}, {45-3-5, 99+3}}, //37 small left 5

	{SillyBilly_ArcMain_Sidle0,{  0, 178,  87,  75}, {45-6-5, 85+3}}, //38 small down 1
	{SillyBilly_ArcMain_Sidle0,{ 87, 178,  86,  76}, {45-7-5, 86+3}}, //39 small down 2
	{SillyBilly_ArcMain_Sleft0,{ 84,  91,  84,  77}, {45-8-5, 87+3}}, //40 small down 3
	{SillyBilly_ArcMain_Sleft0,{ 84, 168,  84,  77}, {45-8-5, 87+3}}, //41 small down 4
	{SillyBilly_ArcMain_Sidle0,{173, 176,  83,  78}, {45-8-5, 88+3}}, //42 small down 5

	{sillybilly_ArcMain_sing4,  {  0, 151,  81,  97}, {45-14-5, 107+3}}, //43 small up 1
	{sillybilly_ArcMain_sing4,  { 81, 151,  81,  96}, {45-13-5, 106+3}}, //44 small up 2
	{sillybilly_ArcMain_sing4,  {162, 151,  82,  96}, {45-12-5, 106+3}}, //45 small up 3
	{sillybilly_ArcMain_sing4,  {163,   0,  82,  95}, {45-13-5, 105+3}}, //46 small up 4
	{sillybilly_ArcMain_sing36, {130,   0,  83,  95}, {45-12-5, 105+3}}, //47 small up 5

	{SillyBilly_ArcMain_Down0,{  0,   0,  95,  88}, {45-11-5, 98+3}}, //48 small right 1
	{SillyBilly_ArcMain_Down1,{  0,   0,  96,  87}, {45-11-5, 97+3}}, //49 small right 2
	{SillyBilly_ArcMain_Down2,{  0,   0,  92,  86}, {45-11-5, 96+3}}, //50 small right 3
	{SillyBilly_ArcMain_Down3,{  0,   0,  91,  85}, {45-11-5, 95+3}}, //51 small right 4
	{SillyBilly_ArcMain_Down4,{  0,   0,  91,  86}, {45-11-5, 96+3}}, //52 small right 5

	//Ungrow animtation

	{SillyBilly_ArcMain_Ungrow0,{  0,   0, 103, 176}, {45-1, 186}}, //53 ungrow 1
	{SillyBilly_ArcMain_Ungrow0,{103,   5, 106, 171}, {45-1, 181}}, //54 ungrow 2
	{SillyBilly_ArcMain_Ungrow1,{  0,   0, 105, 174}, {45-1, 184}}, //55 ungrow 3
	{SillyBilly_ArcMain_Ungrow1,{105,  15, 102, 159}, {45-1, 169}}, //56 ungrow 4
	{SillyBilly_ArcMain_Ungrow2,{  0,   0, 116, 160}, {45+12, 170}}, //57 ungrow 5
	{SillyBilly_ArcMain_Ungrow2,{116,  28, 117, 132}, {45+16-1, 142+2}}, //58 ungrow 6
	{SillyBilly_ArcMain_Ungrow3,{  0,   0, 110, 107}, {45+13-3, 117+3}}, //59 ungrow 7
	{SillyBilly_ArcMain_Ungrow3,{110,  14,  86,  93}, {45-6-4, 103+3}}, //60 ungrow 8
	{SillyBilly_ArcMain_Ungrow3,{  0, 107,  85,  82}, {45-6-5,  92+3}}, //61 ungrow 9

	//Unshrink animation

	{SillyBilly_ArcMain_Unsh0,{109,   0,  82,  79}, {45,  89}}, //62 unshrink 1
	{SillyBilly_ArcMain_Unsh0,{  0,   0, 109, 138}, {45+27, 148}}, //63 unshrink 2
	{SillyBilly_ArcMain_Unsh0,{130, 121, 116, 131}, {45+28, 141}}, //64 unshrink 3
	{SillyBilly_ArcMain_Unsh1,{  0,   0, 124, 136}, {45+27, 146}}, //65 unshrink 4
	{SillyBilly_ArcMain_Unsh1,{124,   0, 127, 134}, {45+25, 144}}, //66 unshrink 5
	{SillyBilly_ArcMain_Unsh0,{  0, 138, 130, 111}, {45+17, 121}}, //67 unshrink 6
	{SillyBilly_ArcMain_Unsh2,{  0,   0, 146, 107}, {45, 117}}, //68 unshrink 7
	{SillyBilly_ArcMain_Unsh2,{  0, 107, 146, 107}, {45, 117}}, //69 unshrink 8
	{SillyBilly_ArcMain_Unsh3,{  0,   0, 142, 107}, {45, 117}}, //70 unshrink 9
	{SillyBilly_ArcMain_Unsh1,{  0, 150, 138, 106}, {45, 116}}, //71 unshrink 10
	{SillyBilly_ArcMain_Unsh1,{138, 134, 117, 122}, {45, 132}}, //72 unshrink 11
	{SillyBilly_ArcMain_Unsh4,{  0,   0, 116, 146}, {45+4, 156}}, //73 unshrink 12
	{SillyBilly_ArcMain_Unsh4,{116,   0, 117, 204}, {45+18, 214}}, //74 unshrink 13
	{SillyBilly_ArcMain_Unsh3,{142,   0, 113, 203}, {45+13, 213}}, //75 unshrink 14
	{SillyBilly_ArcMain_Unsh5,{  0,   0, 118, 199}, {45+17, 209}}, //76 unshrink 15
	{SillyBilly_ArcMain_Unsh5,{118,   0, 125, 167}, {45+23, 177}}, //77 unshrink 16
	{SillyBilly_ArcMain_Unsh6,{  0,   0, 127, 147}, {45+22, 157}}, //78 unshrink 17
	{SillyBilly_ArcMain_Unsh6,{127,   0, 126, 152}, {45+9, 162}}, //79 unshrink 18
	{SillyBilly_ArcMain_Unsh7,{  0,   0, 123, 152}, {45+6, 162}}, //80 unshrink 19
	{SillyBilly_ArcMain_Unsh7,{123,   0, 119, 150}, {45+6, 160}}, //81 unshrink 20
	{sillybilly_ArcMain_sing37,{128,   0, 118, 151}, {45+6, 161}}, //82 unshrink 21

	{sillybilly_ArcMain_sing0,{0,0,107,173},{44,181}}, //68 Sing_new
	{sillybilly_ArcMain_sing0,{111,0,102,170},{43,178}}, //69 Sing_new
	{sillybilly_ArcMain_sing1,{0,0,123,170},{62,178}}, //70 Sing_new
	{sillybilly_ArcMain_sing1,{126,0,130,165},{69,173}}, //71 Sing_new
	{sillybilly_ArcMain_sing1,{126,0,130,165},{69,173}}, //72 Sing_new
	{sillybilly_ArcMain_sing2,{0,0,129,168},{68,175}}, //73 Sing_new
	{sillybilly_ArcMain_sing2,{133,0,121,168},{60,176}}, //74 Sing_new
	{sillybilly_ArcMain_sing3,{0,0,122,167},{60,176}}, //75 Sing_new
	{sillybilly_ArcMain_sing3,{126,0,124,157},{50,166}}, //76 Sing_new
	{sillybilly_ArcMain_sing4,{0,0,164,152},{67,160}}, //77 Sing_new
	{sillybilly_ArcMain_sing5,{0,0,192,145},{67,153}}, //78 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //79 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //80 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //81 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //82 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //83 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //84 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //85 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //86 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //87 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //88 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //89 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //90 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //91 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //92 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //93 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //94 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //95 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //96 Sing_new
	{sillybilly_ArcMain_sing6,{0,0,196,142},{67,151}}, //97 Sing_new
	{sillybilly_ArcMain_sing7,{0,0,195,142},{67,151}}, //98 Sing_new
	{sillybilly_ArcMain_sing9,{0,0,177,144},{67,153}}, //99 Sing_new
	{sillybilly_ArcMain_sing10,{0,0,167,145},{67,154}}, //100 Sing_new
	{sillybilly_ArcMain_sing11,{0,0,137,166},{69,176}}, //101 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //102 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //103 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //104 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //105 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //106 Sing_new
	{sillybilly_ArcMain_sing13,{0,0,142,166},{78,176}}, //107 Sing_new
	{sillybilly_ArcMain_sing14,{0,0,143,166},{78,176}}, //108 Sing_new
	{sillybilly_ArcMain_sing15,{0,0,141,166},{77,176}}, //109 Sing_new
	{sillybilly_ArcMain_sing16,{0,0,136,170},{71,180}}, //110 Sing_new
	{sillybilly_ArcMain_sing16,{0,0,136,170},{71,180}}, //111 Sing_new
	{sillybilly_ArcMain_sing16,{0,0,136,170},{71,180}}, //112 Sing_new
	{sillybilly_ArcMain_sing16,{0,0,136,170},{71,180}}, //113 Sing_new
	{sillybilly_ArcMain_sing16,{0,0,136,170},{71,180}}, //114 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //115 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //116 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //117 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //118 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //119 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //120 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //121 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //122 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //123 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //124 Sing_new
	{sillybilly_ArcMain_sing18,{0,0,138,170},{72,180}}, //125 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //126 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //127 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //128 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //129 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //130 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //131 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //132 Sing_new
	{sillybilly_ArcMain_sing20,{0,0,136,170},{70,180}}, //133 Sing_new
	{sillybilly_ArcMain_sing21,{0,0,133,156},{66,166}}, //134 Sing_new
	{sillybilly_ArcMain_sing21,{134,0,122,132},{42,142}}, //135 Sing_new
	{sillybilly_ArcMain_sing22,{0,0,127,131},{43,141}}, //136 Sing_new
	{sillybilly_ArcMain_sing22,{129,0,127,130},{43,140}}, //137 Sing_new
	{sillybilly_ArcMain_sing23,{0,0,127,132},{43,142}}, //138 Sing_new
	{sillybilly_ArcMain_sing23,{0,0,127,132},{43,142}}, //139 Sing_new
	{sillybilly_ArcMain_sing24,{0,0,102,175},{44,183}}, //140 Sing_new
	{sillybilly_ArcMain_sing24,{102,0,102,221},{53,228}}, //141 Sing_new
	{sillybilly_ArcMain_sing24,{102,0,102,221},{53,228}}, //142 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //143 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //144 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //145 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //146 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //147 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //148 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //149 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //150 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //151 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //152 Sing_new
	{sillybilly_ArcMain_sing26,{0,0,102,227},{54,234}}, //153 Sing_new
	{sillybilly_ArcMain_sing26,{104,0,103,227},{55,234}}, //154 Sing_new
	{sillybilly_ArcMain_sing27,{0,0,103,224},{55,232}}, //155 Sing_new
	{sillybilly_ArcMain_sing27,{0,0,103,224},{55,232}}, //156 Sing_new
	{sillybilly_ArcMain_sing27,{107,0,102,223},{54,231}}, //157 Sing_new
	{sillybilly_ArcMain_sing27,{107,0,102,223},{54,231}}, //158 Sing_new
	{sillybilly_ArcMain_sing28,{0,0,100,219},{51,226}}, //159 Sing_new
	{sillybilly_ArcMain_sing28,{0,0,100,219},{51,226}}, //160 Sing_new
	{sillybilly_ArcMain_sing28,{104,0,100,219},{51,226}}, //161 Sing_new
	{sillybilly_ArcMain_sing28,{104,0,100,219},{51,226}}, //162 Sing_new
	{sillybilly_ArcMain_sing29,{0,0,103,207},{54,216}}, //163 Sing_new
	{sillybilly_ArcMain_sing29,{0,0,103,207},{54,216}}, //164 Sing_new
	{sillybilly_ArcMain_sing29,{107,0,111,189},{64,200}}, //165 Sing_new
	{sillybilly_ArcMain_sing29,{107,0,111,189},{64,200}}, //166 Sing_new
	{sillybilly_ArcMain_sing29,{107,0,111,189},{64,200}}, //167 Sing_new
	{sillybilly_ArcMain_sing30,{0,0,103,181},{56,193}}, //168 Sing_new
	{sillybilly_ArcMain_sing30,{0,0,103,181},{56,193}}, //169 Sing_new
	{sillybilly_ArcMain_sing30,{106,0,100,183},{52,195}}, //170 Sing_new
	{sillybilly_ArcMain_sing30,{106,0,100,183},{52,195}}, //171 Sing_new
	{sillybilly_ArcMain_sing31,{0,0,100,185},{52,197}}, //172 Sing_new
	{sillybilly_ArcMain_sing31,{0,0,100,185},{52,197}}, //173 Sing_new
	{sillybilly_ArcMain_sing31,{104,0,116,173},{51,185}}, //174 Sing_new
	{sillybilly_ArcMain_sing31,{104,0,116,173},{51,185}}, //175 Sing_new
	{sillybilly_ArcMain_sing32,{0,0,167,161},{56,173}}, //176 Sing_new
	{sillybilly_ArcMain_sing32,{0,0,167,161},{56,173}}, //177 Sing_new
	{sillybilly_ArcMain_sing33,{0,0,168,161},{56,173}}, //178 Sing_new
	{sillybilly_ArcMain_sing33,{0,0,168,161},{56,173}}, //179 Sing_new
	{sillybilly_ArcMain_sing33,{0,0,168,161},{56,173}}, //180 Sing_new
	{sillybilly_ArcMain_sing33,{0,0,168,161},{56,173}}, //181 Sing_new
	{sillybilly_ArcMain_sing35,{0,0,155,166},{56,178}}, //182 Sing_new
	{sillybilly_ArcMain_sing35,{0,0,155,166},{56,178}}, //183 Sing_new
	{sillybilly_ArcMain_sing36,{0,0,130,150},{65,162}}, //184 Sing_new
	{sillybilly_ArcMain_sing36,{0,0,130,150},{65,162}}, //185 Sing_new
	{sillybilly_ArcMain_sing37,{0,0,128,152},{63,164}}, //186 Sing_new
	{sillybilly_ArcMain_sing37,{0,0,128,152},{63,164}}, //187 Sing_new
	{sillybilly_ArcMain_sing39,{0,0,131,163},{66,175}}, //188 Sing_new
	{sillybilly_ArcMain_sing39,{0,0,131,163},{66,175}}, //189 Sing_new
	{sillybilly_ArcMain_sing41,{0,0,133,178},{66,190}}, //190 Sing_new
	{sillybilly_ArcMain_sing41,{0,0,133,178},{66,190}}, //191 Sing_new
	{sillybilly_ArcMain_sing43,{0,0,136,169},{69,182}}, //192 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //193 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //194 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //195 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //196 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //197 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //198 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //199 Sing_new
	{sillybilly_ArcMain_sing44,{0,0,134,170},{68,183}}, //200 Sing_new
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
	{2, (const u8[]){ 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, ASCR_BACK, 3}},   //CharAnim_Sing
};

static const Animation char_smallsillybilly_anim[CharAnim_Max] = {
	{1, (const u8[]){ 27, 28, 28, 29, 29, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, ASCR_BACK, 1}}, //CharAnim_Idle
	{1, (const u8[]){ 33, 34, 34, 35, 35, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37, ASCR_BACK, 3}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{1, (const u8[]){ 38, 39, 39, 40, 40, 41, 41, 41, 42, 42, 42, 42, 42, 42, 42, ASCR_BACK, 3}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){ 43, 44, 44, 45, 45, 46, 46, 46, 47, 47, 47, 47, 47, 47, 47, ASCR_BACK, 3}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{1, (const u8[]){ 48, 49, 49, 50, 50, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, ASCR_BACK, 3}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{1, (const u8[]){ 0, 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 53, 53, 53, 54, 54, 55, 56, 57, 57, 58, 58, 59, 59, 60, 60, 60, 61, 61, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_Sing
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
				case StageId_8_2: //Animations
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
						case 128:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(9,8);
							break;
						case 1408:
							character->set_anim(character, CharAnim_UnGrow);
							this->character.focus_x = FIXED_DEC(35,1);
							this->character.focus_y = FIXED_DEC(-80,1);
							this->character.focus_zoom = FIXED_DEC(7,6);
							smallphase =1;
							break;
						case 2044:
							character->set_anim(character, CharAnim_UnShrink);
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(9,8);
							smallphase =0;
							break;
						case 2336:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(5,4);
							break;
						case 2352:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(9,8);
							break;
						case 3360:
							this->character.focus_x = FIXED_DEC(30,1);
							this->character.focus_y = FIXED_DEC(-118,1);
							this->character.focus_zoom = FIXED_DEC(11,12);
							break;
						case 3366:
							character->set_anim(character, CharAnim_Sing);
							break;
						case 3376:
							this->character.focus_x = FIXED_DEC(30,1);
							this->character.focus_y = FIXED_DEC(-118,1);
							this->character.focus_zoom = FIXED_DEC(11,10);
							break;
						case 3408:
							this->character.focus_x = FIXED_DEC(30,1);
							this->character.focus_y = FIXED_DEC(-118,1);
							this->character.focus_zoom = FIXED_DEC(10,9);
							break;
						case 3440:
							this->character.focus_x = FIXED_DEC(30,1);
							this->character.focus_y = FIXED_DEC(-118,1);
							this->character.focus_zoom = FIXED_DEC(9,8);
							break;
						case 3464:
							this->character.focus_x = FIXED_DEC(28,1);
							this->character.focus_y = FIXED_DEC(-120,1);
							this->character.focus_zoom = FIXED_DEC(8,7);
							break;
						case 3472:
							this->character.focus_x = FIXED_DEC(26,1);
							this->character.focus_y = FIXED_DEC(-122,1);
							this->character.focus_zoom = FIXED_DEC(7,6);
							break;
						case 3478:
							this->character.focus_x = FIXED_DEC(26,1);
							this->character.focus_y = FIXED_DEC(-122,1);
							this->character.focus_zoom = FIXED_DEC(6,5);
							break;
						case 3484:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(5,4);
							break;
						case 3486:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(3,2);
							break;
						case 3493:
							this->character.focus_x = FIXED_DEC(-60,1);
							this->character.focus_y = FIXED_DEC(-84,1);
							this->character.focus_zoom = FIXED_DEC(1,1);
							break;
						case 3888:
							this->character.focus_x = FIXED_DEC(25,1);
							this->character.focus_y = FIXED_DEC(-125,1);
							this->character.focus_zoom = FIXED_DEC(9,8);
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
	}
	else
	{
	    Animatable_Animate(&character->animatable, (void*)this, Char_SillyBilly_SetFrame);
        if (stage.song_step>=3888)
            Character_DrawCol(character, &this->tex, &char_sillybilly_frame[this->frame], 100, 113, 155);
        else
            Character_Draw(character, &this->tex, &char_sillybilly_frame[this->frame]);
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

	this->character.focus_x = FIXED_DEC(25,1);
	this->character.focus_y = FIXED_DEC(-125,1);
	this->character.focus_zoom = FIXED_DEC(2,1);

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

		"sing0.tim",
		"sing1.tim",
		"sing2.tim",
		"sing3.tim",
		"sing4.tim",
		"sing5.tim",
		"sing6.tim",
		"sing7.tim",
		"sing9.tim",
		"sing10.tim",
		"sing11.tim",
		"sing13.tim",
		"sing14.tim",
		"sing15.tim",
		"sing16.tim",
		"sing18.tim",
		"sing20.tim",
		"sing21.tim",
		"sing22.tim",
		"sing23.tim",
		"sing24.tim",
		"sing26.tim",
		"sing27.tim",
		"sing28.tim",
		"sing29.tim",
		"sing30.tim",
		"sing31.tim",
		"sing32.tim",
		"sing33.tim",
		"sing35.tim",
		"sing36.tim",
		"sing37.tim",
		"sing39.tim",
		"sing41.tim",
		"sing43.tim",
		"sing44.tim",

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
