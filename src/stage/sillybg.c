/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sillybg.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

int lyrics_select;
//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	//IO_Data arc_final, arc_final_ptr[6];

	//Textures
	Gfx_Tex tex_back0; //Normal mirror left
	Gfx_Tex tex_back1; //Normal mirror right

	Gfx_Tex tex_broke0; //Broken mirror left
	Gfx_Tex tex_broke1; //Broken mirror right

	Gfx_Tex tex_floor; //Floor

	Gfx_Tex tex_frame0; //mirror pillars left

	/*//Cutscene
	Gfx_Tex tex_final;
	u8 final_frame, final_tex_id;

	Animatable final_animatable;*/

} Back_Sillybg;

/*static const CharFrame final_frame[82] = {
  {0, {  0,  0, 64, 64}, {0,0}}, //0 idle 1
  {0, { 64,  0, 64, 64}, {0,0}}, //1 idle 2
  {0, {128,  0, 64, 64}, {0,0}}, //2 idle 3
  {0, {192,  0, 64, 64}, {0,0}}, //3 idle 4
  {0, {  0, 64, 64, 64}, {0,0}}, //4 idle 5
  {0, { 64, 64, 64, 64}, {0,0}}, //5 idle 6
  {0, {128, 64, 64, 64}, {0,0}}, //6 idle 7
  {0, {192, 64, 64, 64}, {0,0}}, //7 idle 8
  {0, {  0,128, 64, 64}, {0,0}}, //8 idle 9
  {0, { 64,128, 64, 64}, {0,0}}, //9 idle 10
  {0, {128,128, 64, 64}, {0,0}}, //10 idle 11
  {0, {192,128, 64, 64}, {0,0}}, //11 idle 12
  {0, {  0,192, 64, 64}, {0,0}}, //12 idle 13
  {0, { 64,192, 64, 64}, {0,0}}, //13 idle 14
  {0, {128,192, 64, 64}, {0,0}}, //14 idle 15
  {0, {192,192, 64, 64}, {0,0}}, //15 idle 16
  {1, {  0,  0, 64, 64}, {0,0}}, //16 idle 17
  {1, { 64,  0, 64, 64}, {0,0}}, //17 idle 18
  {1, {128,  0, 64, 64}, {0,0}}, //18 idle 19
  {1, {192,  0, 64, 64}, {0,0}}, //19 idle 20
  {1, {  0, 64, 64, 64}, {0,0}}, //20 idle 21
  {1, { 64, 64, 64, 64}, {0,0}}, //21 idle 22
  {1, {128, 64, 64, 64}, {0,0}}, //22 idle 23
  {1, {192, 64, 64, 64}, {0,0}}, //23 idle 24
  {1, {  0,128, 64, 64}, {0,0}}, //24 idle 25
  {1, { 64,128, 64, 64}, {0,0}}, //25 idle 26
  {1, {128,128, 64, 64}, {0,0}}, //26 idle 27
  {1, {192,128, 64, 64}, {0,0}}, //27 idle 28
  {1, {  0,192, 64, 64}, {0,0}}, //28 idle 29
  {1, { 64,192, 64, 64}, {0,0}}, //29 idle 30
  {1, {128,192, 64, 64}, {0,0}}, //30 idle 31
  {1, {192,192, 64, 64}, {0,0}}, //31 idle 32
  {2, {  0,  0, 64, 64}, {0,0}}, //32 idle 33
  {2, { 64,  0, 64, 64}, {0,0}}, //33 idle 34
  {2, {128,  0, 64, 64}, {0,0}}, //34 idle 35
  {2, {192,  0, 64, 64}, {0,0}}, //35 idle 36
  {2, {  0, 64, 64, 64}, {0,0}}, //36 idle 37
  {2, { 64, 64, 64, 64}, {0,0}}, //37 idle 38
  {2, {128, 64, 64, 64}, {0,0}}, //38 idle 39
  {2, {192, 64, 64, 64}, {0,0}}, //39 idle 40
  {2, {  0,128, 64, 64}, {0,0}}, //40 idle 41
  {2, { 64,128, 64, 64}, {0,0}}, //41 idle 42
  {2, {128,128, 64, 64}, {0,0}}, //42 idle 43
  {2, {192,128, 64, 64}, {0,0}}, //43 idle 44
  {2, {  0,192, 64, 64}, {0,0}}, //44 idle 45
  {2, { 64,192, 64, 64}, {0,0}}, //45 idle 46
  {2, {128,192, 64, 64}, {0,0}}, //46 idle 47
  {2, {192,192, 64, 64}, {0,0}}, //47 idle 48
  {3, {  0,  0, 64, 64}, {0,0}}, //48 idle 49
  {3, { 64,  0, 64, 64}, {0,0}}, //49 idle 50
  {3, {128,  0, 64, 64}, {0,0}}, //50 idle 51
  {3, {192,  0, 64, 64}, {0,0}}, //51 idle 52
  {3, {  0, 64, 64, 64}, {0,0}}, //52 idle 53
  {3, { 64, 64, 64, 64}, {0,0}}, //53 idle 54
  {3, {128, 64, 64, 64}, {0,0}}, //54 idle 55
  {3, {192, 64, 64, 64}, {0,0}}, //55 idle 56
  {3, {  0,128, 64, 64}, {0,0}}, //56 idle 57
  {3, { 64,128, 64, 64}, {0,0}}, //57 idle 58
  {3, {128,128, 64, 64}, {0,0}}, //58 idle 59
  {3, {192,128, 64, 64}, {0,0}}, //59 idle 60
  {3, {  0,192, 64, 64}, {0,0}}, //60 idle 61
  {3, { 64,192, 64, 64}, {0,0}}, //61 idle 62
  {3, {128,192, 64, 64}, {0,0}}, //62 idle 63
  {3, {192,192, 64, 64}, {0,0}}, //63 idle 64
  {4, {  0,  0, 64, 64}, {0,0}}, //64 idle 65
  {4, { 64,  0, 64, 64}, {0,0}}, //65 idle 66
  {4, {128,  0, 64, 64}, {0,0}}, //66 idle 67
  {4, {192,  0, 64, 64}, {0,0}}, //67 idle 68
  {4, {  0, 64, 64, 64}, {0,0}}, //68 idle 69
  {4, { 64, 64, 64, 64}, {0,0}}, //69 idle 70
  {4, {128, 64, 64, 64}, {0,0}}, //70 idle 71
  {4, {192, 64, 64, 64}, {0,0}}, //71 idle 72
  {4, {  0,128, 64, 64}, {0,0}}, //72 idle 73
  {4, { 64,128, 64, 64}, {0,0}}, //73 idle 74
  {4, {128,128, 64, 64}, {0,0}}, //74 idle 75
  {4, {192,128, 64, 64}, {0,0}}, //75 idle 76
  {4, {  0,192, 64, 64}, {0,0}}, //76 idle 77
  {4, { 64,192, 64, 64}, {0,0}}, //77 idle 78
  {4, {128,192, 64, 64}, {0,0}}, //78 idle 79
  {4, {192,192, 64, 64}, {0,0}}, //79 idle 80
  {5, {  0,  0, 64, 64}, {0,0}}, //80 idle 81
  {5, { 64,  0, 64, 64}, {0,0}}, //81 idle 82
};

static const Animation final_anim[1] = {
	{5, (const u8[]) { 0, 0, 1, 1, 2, 3, 4, 5, 3, 4, 5, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 11, 12, 11, 12, 13, 14, 14, 14, 15, 16, 17, 18, 19, 20, 18, 19, 20, 18, 19, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 53, 54, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 81, 82, 82, 82, 82, 82, 82, ASCR_BACK, 0}},
};

void Sillybg_Final_SetFrame(void* user, u8 frame)
{
	Back_Sillybg* this = (Back_Sillybg*)user;

	//Check if this is a new frame
	if (frame != this->final_frame)
	{
		//Check if new art shall be loaded
		const CharFrame* cframe = &final_frame[this->final_frame = frame];
		if (cframe->tex != this->final_tex_id)
			Gfx_LoadTex(&this->tex_final, this->arc_final_ptr[this->final_tex_id = cframe->tex], 0);
	}
}

void Sillybg_Final_Draw(Back_Sillybg* this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame* cframe = &final_frame[this->final_frame];

	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);


	RECT src = { cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3] };
	RECT_FIXED dst = { ox, oy, (src.w + 280) << FIXED_SHIFT, (src.h + 200) << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_final, &src, &dst, stage.camera.bzoom);
}*/

void Back_Sillybg_DrawFG(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;


	fixed_t fx, fy;

	fx = stage.camera.x >> 3;
	fy = stage.camera.y >> 3;
	struct
	{
              const char *Lyrics;
	} song_lyrics[] = {
	      {" "},
    	      {"ILL MAKE"},
	      {"YOU SAY"},
	      {"HOW PROUD"},
	      {"YOU"},
	      {"YOU ARE"},
	      {"YOU ARE OF"},
	      {"YOU ARE OF ME"},
	      {"SO"},
	      {"SO STAY"},
	      {"AWAKE"},
	      {"JUST"},
	      {"LONG"},
	      {"ENOUGH TO SEE"},
	      {"MY"},
	      {"MY WAY"},
	};
	if(stage.stage_id == StageId_8_2)
	{
		switch (stage.song_step)
		{
			case 0:
				lyrics_select=0;
				break;
			case 1424:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1428:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1440:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1444:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1456:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1460:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1472:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1476:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1488:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1492:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1504:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1508:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1520:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1524:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1536:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1540:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1552:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1568:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1572:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1584:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1588:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1600:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1604:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1616:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1620:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1632:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1636:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1648:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1652:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1664:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1668:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1680:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1684:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1688:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1692:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1696:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1700:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1704:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1708:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1712:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1716:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1720:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1724:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1728:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1744:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1748:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1752:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1756:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1760:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1764:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1768:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1772:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1776:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1780:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1784:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1788:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1792:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1824:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1830:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1836:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1856:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1862:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1868:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2464:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2466:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2467:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2469:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2470:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2496:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2498:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2499:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2501:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2502:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2506:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2508:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2510:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2512:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 3356:
				stage.nohud=1;
				break;
			case 3360:
				lyrics_select=1;
				break;
			case 3400:
				lyrics_select=2;
				break;
			case 3432:
				lyrics_select=3;
				break;
			case 3464:
				lyrics_select=4;
				break;
			case 3472:
				lyrics_select=5;
				break;
			case 3478:
				lyrics_select=6;
				break;
			case 3484:
				lyrics_select=7;
				break;
			case 3495:
				lyrics_select=8;
				break;
			case 3504:
				lyrics_select=9;
				break;
			case 3528:
				lyrics_select=10;
				break;
			case 3561:
				lyrics_select=11;
				break;
			case 3568:
				lyrics_select=12;
				break;
			case 3592:
				lyrics_select=13;
				break;
			case 3621:
				stage.nohud=0;
				break;
			case 3623:
				lyrics_select=14;
				break;
			case 3632:
				lyrics_select=15;
				break;
			case 3696:
				lyrics_select=0;
				break;
			case 3752:
				lyrics_select=15;
				break;
			case 3807:
				lyrics_select=0;
				break;
			case 3888:
				stage.bluenotes=true;
				break;
		}
	}
	if(stage.song_step>=4176)
       	{
       		RECT black = {0, 0, 320, 240};
		Gfx_DrawRect(&black, 0, 0, 0);
    }
	stage.font_bold.draw_col(&stage.font_bold, song_lyrics[lyrics_select].Lyrics, 160, 223, FontAlign_Center, 202 >> 1, 172 >> 1, 61 >> 1);

	RECT tline = {0, 16, 320, 1};
	Gfx_DrawRect(&tline, 255, 255, 255);

	RECT Top_bar = {0, 0, 320, 16};
       	Gfx_BlendRect(&Top_bar, 0, 0, 0, 0);

       	RECT bline = {0, 221, 320, 1};
	Gfx_DrawRect(&bline, 255, 255, 255);

       	RECT Bottom_bar = {0, 222, 320, 18};
       	Gfx_BlendRect(&Bottom_bar, 0, 0, 0, 0);
    if(stage.song_step >= 3872 && stage.song_step <= 3888)
	{
		RECT blackscreen = {0, 0, 320, 240};
		Gfx_DrawRect(&blackscreen, 0, 0, 0);
	}
	/*if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step)
		{
			case 3493:
				Animatable_SetAnim(&this->final_animatable, 0);
				break;
		}
	}
	Animatable_Animate(&this->final_animatable, (void*)this, Sillybg_Final_SetFrame);
	if (stage.song_step >= 3493 && stage.song_step <= 3888)
	{
		Sillybg_Final_Draw(this, FIXED_DEC(-160, 1), FIXED_DEC(-125, 1));
	}*/
}

//Week 1 background functions
void Back_Sillybg_DrawBG(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;

	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT framel_src = {0, 0, 252, 187};
	RECT_FIXED framel_dst = {
		FIXED_DEC(-250+83 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-137-92,1) - fy,
		FIXED_DEC(297 + SCREEN_WIDEOADD,1),//252
		FIXED_DEC(282,1)//187
	};
	Stage_DrawTex(&this->tex_frame0, &framel_src, &framel_dst, stage.camera.bzoom); //frame left

	RECT floorl_src = {0, 0, 256, 128};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-205+26 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+161,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom); //floor left

	RECT floorr_src = {0, 128, 256, 128};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(49+26 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+161,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom); //floor right
	//26 161
	RECT halll_src = {0, 0, 256, 256};
	RECT_FIXED halll_dst = {
		FIXED_DEC(-205-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140-91,1) - fy,
		FIXED_DEC(302 + SCREEN_WIDEOADD,1),
		FIXED_DEC(387,1)
	};

	RECT hallr_src = {0, 0, 256, 256};
	RECT_FIXED hallr_dst = {
		FIXED_DEC(49+45-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140-91,1) - fy,
		FIXED_DEC(301 + SCREEN_WIDEOADD,1),
		FIXED_DEC(387,1)
	};
	if(stage.song_step ==3432)
	{
		RECT flash = {0, 0, 320, 240};
       		Gfx_BlendRect(&flash, 52, 52, 52, 0);
       	}

	if(stage.song_step >=3432)
	{
		Stage_DrawTex(&this->tex_broke0, &halll_src, &halll_dst, stage.camera.bzoom); //Broken mirror left
		Stage_DrawTex(&this->tex_broke1, &hallr_src, &hallr_dst, stage.camera.bzoom); //Broken mirror right
	}
	else
	{
		Stage_DrawTex(&this->tex_back0, &halll_src, &halll_dst, stage.camera.bzoom); //Normal mirror left
		Stage_DrawTex(&this->tex_back1, &hallr_src, &hallr_dst, stage.camera.bzoom); //Normal mirror right
	}
}
void Back_Sillybg_Free(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;

	//Free structure
	//Mem_Free(this->arc_final);
	Mem_Free(this);
}

StageBack *Back_Sillybg_New(void)
{
	//Allocate background structure
	Back_Sillybg *this = (Back_Sillybg*)Mem_Alloc(sizeof(Back_Sillybg));
	if (this == NULL)
		return NULL;

	//Set background functions
	this->back.draw_fg = Back_Sillybg_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Sillybg_DrawBG;
	this->back.free = Back_Sillybg_Free;

	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK8\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_broke0, Archive_Find(arc_back, "broke0.tim"), 0);
	Gfx_LoadTex(&this->tex_broke1, Archive_Find(arc_back, "broke1.tim"), 0);
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Gfx_LoadTex(&this->tex_frame0, Archive_Find(arc_back, "frame0.tim"), 0);
	Mem_Free(arc_back);

	/*//Load final cutscene textures
	this->arc_final = IO_Read("\\WEEK8\\FINAL.ARC;1");
	this->arc_final_ptr[0] = Archive_Find(this->arc_final, "final0.tim");
	this->arc_final_ptr[1] = Archive_Find(this->arc_final, "final1.tim");
	this->arc_final_ptr[2] = Archive_Find(this->arc_final, "final2.tim");
	this->arc_final_ptr[3] = Archive_Find(this->arc_final, "final3.tim");
	this->arc_final_ptr[4] = Archive_Find(this->arc_final, "final4.tim");
	this->arc_final_ptr[5] = Archive_Find(this->arc_final, "final5.tim");

	//Initialize final cutscene state
	Animatable_Init(&this->final_animatable, final_anim);

	Animatable_SetAnim(&this->final_animatable, 0);

	this->final_frame = this->final_tex_id = 0xFF; //Force art load*/

	stage.bluenotes=false;

	Gfx_SetClear(139, 138, 196);

	return (StageBack*)this;
}
