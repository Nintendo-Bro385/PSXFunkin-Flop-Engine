		{ //StageId_1_1 (Bopeebo)
		"BOPEEBO",
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(1,1),FIXED_DEC(13,10)},
		1, 1,
		XA_Bopeebo, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_1_2, STAGE_LOAD_FLAG
	},
	{ //StageId_1_2 (Fresh)
		"FRESH",
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(13,10),FIXED_DEC(18,10)},
		1, 2,
		XA_Fresh, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_1_3, STAGE_LOAD_FLAG
	},
	{ //StageId_1_3 (Dadbattle)
		"DADBATLLE",
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(15,10),FIXED_DEC(23,10)},
		1, 3,
		XA_Dadbattle, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_1_3, 0
	},
	{ //StageId_1_4 (Tutorial)
		"TUTORIAL",
		//Characters
		{Char_BF_New, FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_GF_New,  FIXED_DEC(0,1),  FIXED_DEC(-15,1)}, //TODO
		{NULL,           FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(1,1),FIXED_DEC(1,1)},
		1, 4,
		XA_Tutorial, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_1_4, 0
	},
	
	{ //StageId_2_1 (Spookeez)
		"SPOOKEEZ",
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Spook_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(17,10),FIXED_DEC(24,10)},
		2, 1,
		XA_Spookeez, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_2_2, STAGE_LOAD_FLAG
	},
	{ //StageId_2_2 (South)
		"SOUTH",
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Spook_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(11,10),FIXED_DEC(15,10),FIXED_DEC(22,10)},
		2, 2,
		XA_South, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_2_3, STAGE_LOAD_FLAG | STAGE_LOAD_OPPONENT
	},
	{ //StageId_2_3 (Monster)
		"MONSTER",
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Spook_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(13,10),FIXED_DEC(16,10)},
		2, 3,
		XA_Monster, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_2_3, 0
	},
	
	{ //StageId_3_1 (Pico)
		"PICO",
		//Characters
		{Char_BF_New,     FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Pico_New, FIXED_DEC(-105,1),   FIXED_DEC(85,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week3_New,
		
		//Song info
		{FIXED_DEC(12,10),FIXED_DEC(14,10),FIXED_DEC(16,10)},
		3, 1,
		XA_Pico, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_3_2, STAGE_LOAD_FLAG
	},
	{ //StageId_3_2 (Philly)
		"PHILLY",
		//Characters
		{Char_BF_New,     FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Pico_New, FIXED_DEC(-105,1),   FIXED_DEC(85,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week3_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(13,10),FIXED_DEC(2,1)},
		3, 2,
		XA_Philly, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_3_3, STAGE_LOAD_FLAG
	},
	{ //StageId_3_3 (Blammed)
		"BLAMMED",
		//Characters
		{Char_BF_New,     FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Pico_New, FIXED_DEC(-105,1),   FIXED_DEC(85,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week3_New,
		
		//Song info
		{FIXED_DEC(12,10),FIXED_DEC(15,10),FIXED_DEC(23,10)},
		3, 3,
		XA_Blammed, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_3_3, 0
	},
	
	{ //StageId_4_1 (Satin Panties)
		"SATIN PANTIES",
		//Characters
		{Char_BFCar_New,   FIXED_DEC(120,1),   FIXED_DEC(40,1)},
		{Char_Mom_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,    FIXED_DEC(-6,1),   FIXED_DEC(-5,1)},
		
		//Stage background
		Back_Week4_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(16,10),FIXED_DEC(18,10)},
		4, 1,
		XA_SatinPanties, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_4_2, STAGE_LOAD_FLAG
	},
	{ //StageId_4_2 (High)
		"HIGH",
		//Characters
		{Char_BFCar_New,   FIXED_DEC(120,1),   FIXED_DEC(40,1)},
		{Char_Mom_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,    FIXED_DEC(-6,1),   FIXED_DEC(-5,1)},
		
		//Stage background
		Back_Week4_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(18,10),FIXED_DEC(2,1)},
		4, 2,
		XA_High, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_4_3, STAGE_LOAD_FLAG
	},
	{ //StageId_4_3 (MILF)
		"MILF",
		//Characters
		{Char_BFCar_New,   FIXED_DEC(120,1),   FIXED_DEC(40,1)},
		{Char_Mom_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,    FIXED_DEC(-6,1),   FIXED_DEC(-5,1)},
		
		//Stage background
		Back_Week4_New,
		
		//Song info
		{FIXED_DEC(14,10),FIXED_DEC(17,10),FIXED_DEC(26,10)},
		4, 3,
		XA_MILF, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_4_3, 0
	},
	{ //StageId_4_4 (Test)
		"TEST",
		//Characters
		{Char_Dad_New,     FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_BFWeeb_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Song info
		{FIXED_DEC(16,10),FIXED_DEC(16,10),FIXED_DEC(16,10)},
		4, 4,
		XA_Test, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_4_4, 0
	},
	
	{ //StageId_5_1 (Cocoa)
		"COCOA",
		//Characters
		{Char_XmasBF_New,  FIXED_DEC(90,1),   FIXED_DEC(85,1)},
		{Char_XmasP_New, FIXED_DEC(-190,1),   FIXED_DEC(90,1)},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week5_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(13,10),FIXED_DEC(13,10)},
		5, 1,
		XA_Cocoa, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_5_2, STAGE_LOAD_FLAG
	},
	{ //StageId_5_2 (Eggnog)
		"EGGNOG",
		//Characters
		{Char_XmasBF_New,  FIXED_DEC(90,1),   FIXED_DEC(85,1)},
		{Char_XmasP_New, FIXED_DEC(-190,1),   FIXED_DEC(90,1)},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week5_New,
		
		//Song info
		{FIXED_DEC(14,10),FIXED_DEC(16,10),FIXED_DEC(19,10)},
		5, 2,
		XA_Eggnog, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_5_3, STAGE_LOAD_FLAG
	},
	{ //StageId_5_3 (Winter Horrorland)
		"WINTER HORRORLAND",
		//Characters
		{Char_XmasBF_New, FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_Dad_New,   FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(11,10),FIXED_DEC(13,10)},
		5, 3,
		XA_WinterHorrorland, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_5_3, 0
	},
	
	{ //StageId_6_1 (Senpai)
		"SENPAI",
		//Characters
		{Char_BFWeeb_New,  FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_Senpai_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{Char_GFWeeb_New,   FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_Week6_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(12,10),FIXED_DEC(13,10)},
		6, 1,
		XA_Senpai, 0,
		1, //dialogue check
		XA_Lunchbox1, 1, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SENPAI.ARC;1"},
		
		StageId_6_2, STAGE_LOAD_FLAG | STAGE_LOAD_OPPONENT
	},
	{ //StageId_6_2 (Roses)
		"ROSES",
		//Characters
		{Char_BFWeeb_New,   FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_SenpaiM_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{Char_GFWeeb_New,    FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_Week6_New,
		
		//Song info
		{FIXED_DEC(12,10),FIXED_DEC(13,10),FIXED_DEC(15,10)},
		6, 2,
		XA_Roses, 2,
		1, //dialogue check
		XA_Clucked, 2, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SENPAI.ARC;1"},
		
		StageId_6_3, 0
	},
	{ //StageId_6_3 (Thorns)
		"THORNS",
		//Characters
		{Char_BFWeeb_New,  FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_Spirit_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{Char_GFWeeb_New,   FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_Week6_New,
		
		//Song info
		{FIXED_DEC(11,10),FIXED_DEC(13,10),FIXED_DEC(15,10)},
		6, 3,
		XA_Thorns, 0,
		1, //dialogue check
		XA_Lunchbox2, 2, //dialogue song and it's channel
		{"\\PORTRAIT\\BF.ARC;1", "\\PORTRAIT\\SENPAI.ARC;1"},
		
		StageId_6_3, 0
	},
	
	{ //StageId_7_1 (Ugh)
		"UGH",
		//Characters
		{Char_BF_New,    FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_Tank_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week7_New,
		
		//Song info
		{FIXED_DEC(125,100),FIXED_DEC(18,10),FIXED_DEC(23,10)},
		7, 1,
		XA_Ugh, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_7_2, STAGE_LOAD_FLAG
	},
	{ //StageId_7_2 (Guns)
		"GUNS",
		//Characters
		{Char_BF_New,    FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_Tank_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week7_New,
		
		//Song info
		{FIXED_DEC(14,10),FIXED_DEC(2,1),FIXED_DEC(25,10)},
		7, 2,
		XA_Guns, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_7_3, STAGE_LOAD_FLAG | STAGE_LOAD_PLAYER | STAGE_LOAD_OPPONENT | STAGE_LOAD_GIRLFRIEND
	},
	{ //StageId_7_3 (Stress)
		"STRESS",
		//Characters
		{Char_BF_New,    FIXED_DEC(105,1),  FIXED_DEC(100,1)}, //TODO: carry gf
		{Char_Tank_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,      FIXED_DEC(0,1),  FIXED_DEC(-15,1)}, //TODO: pico funny
		
		//Stage background
		Back_Week7_New,
		
		//Song info
		{FIXED_DEC(175,100),FIXED_DEC(22,10),FIXED_DEC(26,10)},
		7, 3,
		XA_Stress, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_7_3, 0
	},
	{ //StageId_8_1 (Still Alive)
		"STILL ALIVE",
		//Characters
		{Char_BF_New,   FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(17,10),FIXED_DEC(17,10)},
		8, 1,
		XA_Stillalive, 0,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_8_1, 0
	},
	{ //StageId_8_2 (Senbonzakura)
		"SENBONZAKURA",
		//Characters
		{Char_BF_New,       FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(13,10),FIXED_DEC(17,10),FIXED_DEC(3,1)},
		8, 2,
		XA_Senbonzakura, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_8_2, 0
	},
	{ //StageId_2_4 (Clucked)
		"CLUCKED",
		//Characters
		{Char_BF_New,       FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Clucky_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,        FIXED_DEC(0,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(25,10),FIXED_DEC(25,10),FIXED_DEC(25,10)},
		2, 4,
		XA_Clucked, 2,
		0, //dialogue check
		XA_Clucked, 1, //dialogue song and it's channel
		{NULL, NULL},
		
		StageId_2_4, 0
	},
