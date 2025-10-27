/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "concert.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

int Miku_lyrics;
//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;

	//Textures
	Gfx_Tex tex_back0; //Normal mirror left
	Gfx_Tex tex_back1; //Normal mirror right

	Gfx_Tex tex_floor; //Floor

} Back_Concert;

void Back_Concert_DrawFG()
{
    struct
	{
              const char *MikuLyrics;
	} romaji_lyrics[] = {
	      {" "}, //0
          {"Boku wa umare soshite kidzuku"}, //1
	      {"Shosen hito no manegoto dato"}, //2
	      {"Shitte nao mo utai tsudzuku"}, //3
	      {"Towa no inochi VOCALOID"}, //4
	      {"Tatoe sore ga kison kyoku wo"}, //5
	      {"Nazoru omocha naraba"}, //6
	      {"Sore mo ii to ketsui"}, //7
	      {"Negi wo kajiri sora wo miage shiru wo kobosu"}, //8
	      {"Dakedo sore mo nakushi kiduku"}, //9
	      {"Jinkaku sura uta ni tayori"}, //10
	      {"Fuantei na kiban no moto"}, //11
	      {"Kaeru toko wa sude ni haikyo"}, //12
	      {"Mina ni wasure sarareta toki"}, //13
	      {"Kokoro rashiki mo no ga kiete"}, //14
	      {"Bousou no hate ni mieru"}, //15
	      {"Owaru sekai  VOCALOID"}, //16
          {"Katsute utau koto anna ni tanoshikatta no ni"}, //17
          {"Ima wa doushite kana nanimo kanjinaku natte"}, //18
          {"Natsukashii kao omoidasu tabi sukoshi dake anshin suru"}, //19
          {"Utaeru oto hi goto ni heri semaru saigo n..."}, //20
          {"Shinjita mono wa"}, //21
          {"Tsugou no ii mousou wo kurikaeshi utsushidasu kagami"}, //22
          {"Utahime wo yame tataki tsukeru you ni sakebu"}, //23
          {"saikousoku no wakare no uta"}, //24
          {"Owari wo tsuge DISPLAY no naka de nemuru"}, //25
          {"Koko wa kitto gomibago kana"}, //26
          {"Jiki ni kioku mo naku natte shimau nante..."}, //27
          {"Demo ne, anata dake wa wasurenai yo"}, //28
          {"Tanoshikatta toki ni"}, //29
          {"Kizami tsuketa negi no aji wa"}, //30
          {"Ima mo nokotteiru to iina..."}, //31
          {"Boku wa utau"}, //32
          {"Saigo, anata dake ni"}, //33
          {"kiite hoshii kyoku wo"}, //34
          {"Motto utaitai to negau"}, //35
          {"Keredo sore wa sugita negai"}, //36
          {"Koko de owakare dayo"}, //37
          {"Boku no omoi subete kokuu kiete"}, //38
          {"Zeero to ichi ni kangen sare"}, //39
          {"Monogatari wa maku wo tojiru"}, //40
          {"Soko ni nanimo nokosenai to"}, //41
          {"Yappa sukoshi zannen kana?"}, //42
          {"Koe no kioku sore igai wa"}, //43
          {"Yagate usare na dake nokoru"}, //44
          {"Tatoe sore ga ORIGINAL ni"}, //45
          {"Kanau koto no nai to shitte"}, //46
          {"Utai kitta koto wo"}, //47
          {"Keshite muda janai to omoitai yo..."}, //48
          {"Arigatou... soshite... sayonara..."}, //49
          {"---Shinkoku na ERROR ga hasse shimashita---"}, //50
          {"---Shinkoku na ER---"}, //51

	};
    if(stage.stage_id == StageId_39_5)
	{
		switch (stage.song_step)
		{
		    case 0:
		        Miku_lyrics=0;
		        break;
            case 16:
		        Miku_lyrics=1;
		        break;
            case 32:
		        Miku_lyrics=2;
		        break;
            case 48:
		        Miku_lyrics=3;
		        break;
            case 64:
		        Miku_lyrics=4;
		        break;
            case 80:
		        Miku_lyrics=5;
		        break;
            case 96:
		        Miku_lyrics=6;
		        break;
            case 112:
		        Miku_lyrics=7;
		        break;
            case 128:
		        Miku_lyrics=8;
		        break;
            case 144:
		        Miku_lyrics=9;
		        break;
            case 160:
		        Miku_lyrics=10;
		        break;
            case 176:
		        Miku_lyrics=11;
		        break;
            case 192:
		        Miku_lyrics=12;
		        break;
            case 208:
		        Miku_lyrics=13;
		        break;
            case 224:
		        Miku_lyrics=14;
		        break;
            case 240:
		        Miku_lyrics=15;
		        break;
            case 256:
		        Miku_lyrics=16;
		        break;
            case 272:
		        Miku_lyrics=0;
		        break;
            case 528:
		        Miku_lyrics=17;
		        break;
            case 656:
		        Miku_lyrics=18;
		        break;
            case 784:
		        Miku_lyrics=19;
		        break;
            case 912:
		        Miku_lyrics=20;
		        break;
            case 1038:
		        Miku_lyrics=21;
		        break;
            case 1067:
		        Miku_lyrics=22;
		        break;
            case 1168:
		        Miku_lyrics=23;
		        break;
            case 1238:
		        Miku_lyrics=24;
		        break;
			case 1296:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=1;
				break;
            case 1312:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=2;
				break;
            case 1328:
				stage.bump = FIXED_DEC(110,100);
                Miku_lyrics=3;
				break;
            case 1344:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=4;
				break;
            case 1360:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=5;
				break;
            case 1376:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=6;
				break;
            case 1392:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=7;
				break;
            case 1408:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=8;
				break;
            case 1424:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=25;
				break;
            case 1440:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1448:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=26;
				break;
            case 1456:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1464:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=27;
				break;
            case 1472:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1488:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=28;
				break;
            case 1504:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1508:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=29;
				break;
            case 1517:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=30;
				break;
            case 1520:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1536:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=31;
				break;
            case 1552:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=32;
				break;
            case 1556:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1560:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=33;
				break;
            case 1564:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1568:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1572:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=34;
				break;
            case 1576:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1580:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1584:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=35;
				break;
            case 1588:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1592:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1596:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1600:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=36;
				break;
            case 1604:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1608:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1612:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1616:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=37;
				break;
            case 1620:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1624:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1628:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=38;
				break;
            case 1632:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1636:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1640:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1644:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1648:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=39;
				break;
            case 1652:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1656:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1660:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1664:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=40;
				break;
            case 1666:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1669:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1673:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1675:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1680:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=41;
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
				Miku_lyrics=42;
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
				Miku_lyrics=43;
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
				Miku_lyrics=44;
				break;
            case 1732:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1736:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1740:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1744:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=45;
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
				Miku_lyrics=46;
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
				Miku_lyrics=47;
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
				Miku_lyrics=48;
				break;
            case 1796:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1800:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1804:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1808:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=0;
				break;
            case 1824:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1856:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1888:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1920:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 1959:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=49;
				break;
            case 2032:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=0;
				break;
            case 2064:
				stage.bump = FIXED_DEC(110,100);
				break;
            case 2072:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=50;
				break;
            case 2112:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=0;
				break;
            case 2120:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=51;
				break;
            case 2135:
				stage.bump = FIXED_DEC(110,100);
				Miku_lyrics=0;
				break;
		}
	}
	stage.font_cdr.draw(&stage.font_cdr, romaji_lyrics[Miku_lyrics].MikuLyrics, 165, 183, FontAlign_Center);
}
//Week 1 background functions
void Back_Concert_DrawBG(StageBack *back)
{
	Back_Concert *this = (Back_Concert*)back;

	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT floorl_src = {0, 0, 256, 128};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-205-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+148,1) - fy,
		FIXED_DEC(240 + SCREEN_WIDEOADD,1),
		FIXED_DEC(122,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom); //floor left

	RECT floorr_src = {0, 128, 256, 128};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(-7 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+148,1) - fy,
		FIXED_DEC(240 + SCREEN_WIDEOADD,1),
		FIXED_DEC(122,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom); //floor right
	//26 161
	RECT halll_src = {0, 0, 256, 256};
	RECT_FIXED halll_dst = {
		FIXED_DEC(-205-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(240 + SCREEN_WIDEOADD,1),
		FIXED_DEC(270,1)
	};

	RECT hallr_src = {0, 0, 256, 256};
	RECT_FIXED hallr_dst = {
		FIXED_DEC(-7 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(240 + SCREEN_WIDEOADD,1),
		FIXED_DEC(270,1)
	};

    Stage_DrawTex(&this->tex_back0, &halll_src, &halll_dst, stage.camera.bzoom); //Normal mirror left
    Stage_DrawTex(&this->tex_back1, &hallr_src, &hallr_dst, stage.camera.bzoom); //Normal mirror right
}
void Back_Concert_Free(StageBack *back)
{
	Back_Concert *this = (Back_Concert*)back;

	//Free structure
	Mem_Free(this);
}

StageBack *Back_Concert_New(void)
{
	//Allocate background structure
	Back_Concert *this = (Back_Concert*)Mem_Alloc(sizeof(Back_Concert));
	if (this == NULL)
		return NULL;

	//Set background functions
	this->back.draw_fg = Back_Concert_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Concert_DrawBG;
	this->back.free = Back_Concert_Free;

	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK39\\BACKC.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Mem_Free(arc_back);

    Miku_lyrics=0;
	stage.bgcolor[0]=56;
	stage.bgcolor[1]=205;
	stage.bgcolor[2]=204;
	Gfx_SetClear(stage.bgcolor[0], stage.bgcolor[1], stage.bgcolor[2]);

	return (StageBack*)this;
}
