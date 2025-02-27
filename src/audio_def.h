#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11295)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3840)},  //XA_GameOver
	{XA_Menu, XA_LENGTH(11295)}, //XA_Freeky
	{XA_Menu, XA_LENGTH(3700)}, //XA_Ludum
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(7685)}, //XA_Bopeebo7685
	{XA_Week1A, XA_LENGTH(8000)}, //XA_Fresh
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(8667)}, //XA_Dadbattle
	{XA_Week1B, XA_LENGTH(6800)}, //XA_Tutorial
	//WEEK2A.XA
	{XA_Week2A, XA_LENGTH(9923)}, //XA_Spookeez
	{XA_Week2A, XA_LENGTH(8880)}, //XA_South
	//WEEK2B.XA
	{XA_Week2B, XA_LENGTH(17778)}, //XA_Monster
	{XA_Week2B, XA_LENGTH(11000)}, //XA_Clucked
	//WEEK3A.XA
	{XA_Week3A, XA_LENGTH(8400)},  //XA_Pico
	{XA_Week3A, XA_LENGTH(10000)}, //XA_Philly
	//WEEK3B.XA
	{XA_Week3B, XA_LENGTH(10700)}, //XA_Blammed
	{XA_Week3B, XA_LENGTH(4400)}, //XA_Results
	//WEEK4A.XA
	{XA_Week4A, XA_LENGTH(9300)},  //XA_SatinPanties
	{XA_Week4A, XA_LENGTH(10300)}, //XA_High
	//WEEK4B.XA
	{XA_Week4B, XA_LENGTH(12300)}, //XA_MILF
	{XA_Week4B, XA_LENGTH(10300)}, //XA_Test
	//WEEK5A.XA
	{XA_Week5A, XA_LENGTH(11520)}, //XA_Cocoa
	{XA_Week5A, XA_LENGTH(9401)},  //XA_Eggnog
	//WEEK5B.XA
	{XA_Week5B, XA_LENGTH(13223)}, //XA_WinterHorrorland
	//WEEK6A.XA
	{XA_Week6A, XA_LENGTH(9800)}, //XA_Senpai
	{XA_Week6A, XA_LENGTH(8928)}, //XA_Roses
	//WEEK6B.XA
	{XA_Week6B, XA_LENGTH(10298)}, //XA_Thorns
	//WEEK6C.XA
	{XA_Week6C, XA_LENGTH(7600)}, //XA_Gameoverpixel
	{XA_Week6C, XA_LENGTH(2900)}, //XA_Lunchbox1
	{XA_Week6C, XA_LENGTH(1100)}, //XA_Lunchbox2
	//WEEK7A.XA
	{XA_Week7A, XA_LENGTH(8493)},  //XA_Ugh
	{XA_Week7A, XA_LENGTH(13866)}, //XA_Guns
	//WEEK7B.XA
	{XA_Week7B, XA_LENGTH(12200)}, //XA_Stress
	//KAPIA.XA
	{XA_PortalA, XA_LENGTH(17600)}, //XA_Still alive
	{XA_PortalA, XA_LENGTH(24400)}, //XA_Senbonzakura
	//Billy.xa
	{XA_Billy, XA_LENGTH(37500)}, //XA_Sillybilly

};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	"\\MUSIC\\WEEK2A.XA;1", //XA_Week2A
	"\\MUSIC\\WEEK2B.XA;1", //XA_Week2B
	"\\MUSIC\\WEEK3A.XA;1", //XA_Week3A
	"\\MUSIC\\WEEK3B.XA;1", //XA_Week3B
	"\\MUSIC\\WEEK4A.XA;1", //XA_Week4A
	"\\MUSIC\\WEEK4B.XA;1", //XA_Week4B
	"\\MUSIC\\WEEK5A.XA;1", //XA_Week5A
	"\\MUSIC\\WEEK5B.XA;1", //XA_Week5B
	"\\MUSIC\\WEEK6A.XA;1", //XA_Week6A
	"\\MUSIC\\WEEK6B.XA;1", //XA_Week6B
	"\\MUSIC\\WEEK6C.XA;1", //XA_Week6C
	"\\MUSIC\\WEEK7A.XA;1", //XA_Week7A
	"\\MUSIC\\WEEK7B.XA;1", //XA_Week7B
	"\\MUSIC\\PORTALA.XA;1",  //XA_PortalA
	"\\MUSIC\\BILLY.XA;1",  //XA_Billy
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	{"freeky", false}, //XA_Freeky
	{"ludum", false}, //XA_Ludum
	//WEEK1A.XA
	{"bopeebo", true}, //XA_Bopeebo
	{"fresh", true},   //XA_Fresh
	//WEEK1B.XA
	{"dadbattle", true}, //XA_Dadbattle
	{"tutorial", false}, //XA_Tutorial
	//WEEK2A.XA
	{"spookeez", true}, //XA_Spookeez
	{"south", true},    //XA_South
	//WEEK2B.XA
	{"monster", true}, //XA_Monster
	{"clucked", true}, //XA_Clucked
	//WEEK3A.XA
	{"pico", true},   //XA_Pico
	{"philly", true}, //XA_Philly
	//WEEK3B.XA
	{"blammed", true}, //XA_Blammed
	{"results", false}, //XA_Blammed
	//WEEK4A.XA
	{"satinpanties", true}, //XA_SatinPanties
	{"high", true},         //XA_High
	//WEEK4B.XA
	{"milf", true}, //XA_MILF
	{"test", true}, //XA_Test
	//WEEK5A.XA
	{"cocoa", true},  //XA_Cocoa
	{"eggnog", true}, //XA_Eggnog
	//WEEK5B.XA
	{"winterhorrorland", true}, //XA_WinterHorrorland
	//WEEK6A.XA
	{"senpai", true}, //XA_Senpai
	{"roses", true},  //XA_Roses
	//WEEK6B.XA
	{"thorns", true}, //XA_Thorns
	//WEEK6C.XA
	{"gameoverpixel", false}, //XA_Gameoverpixel
	{"lunchbox1", false}, //XA_Lunchbox1
	{"lunchbox2", false}, //XA_Lunchbox2
	//WEEK7A.XA
	{"ugh", true},  //XA_Ugh
	{"guns", true}, //XA_Guns
	//WEEK7B.XA
	{"stress", true}, //XA_Stress
	//PORTALA.XA
	{"stillalive", true},     //XA_Stillalive
	{"senbonzakura", false}, //XA_Beathoven
	//Billy.xa
	{"sillybilly", true}, //XA_Beathoven
	
	{NULL, false}
};
