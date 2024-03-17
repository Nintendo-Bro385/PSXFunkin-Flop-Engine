/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

//thanks to spicyjpeg for helping me understand this code

#include "save.h"

#include <libmcrd.h>
#include "stage.h"
				  
	        //HAS to be BASCUS-scusid,somename
#define savetitle "bu00:BASCUS-02385SKY"
#define savename  "VS SKY"

static const u8 saveIconPalette[32] = 
{
  	0x25, 0xFF, 0x00, 0x80, 0xB3, 0xE5, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const u8 saveIconImage[128] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
    0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x22, 0x22, 0x01, 0x00,
    0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x12, 0x00, 0x10, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x01, 0x10, 0x22, 0x12, 0x22, 0x12, 0x21, 0x22, 0x01,
    0x21, 0x22, 0x21, 0x11, 0x21, 0x12, 0x22, 0x01, 0x11, 0x12, 0x22, 0x22,
    0x22, 0x22, 0x21, 0x01, 0x10, 0x22, 0x12, 0x11, 0x12, 0x11, 0x22, 0x01,
    0x10, 0x22, 0x13, 0x12, 0x12, 0x12, 0x22, 0x01, 0x10, 0x32, 0x22, 0x22,
    0x22, 0x22, 0x23, 0x01, 0x21, 0x22, 0x11, 0x22, 0x22, 0x22, 0x21, 0x12,
    0x10, 0x21, 0x22, 0x11, 0x11, 0x11, 0x12, 0x11, 0x00, 0x21, 0x22, 0x22,
    0x22, 0x22, 0x01, 0x00, 0x00, 0x10, 0x21, 0x22, 0x22, 0x11, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x11, 0x11, 0x00, 0x00, 0x00
    
    
};

static void toShiftJIS(u8 *buffer, const char *text)
{
    int pos = 0;
    for (u32 i = 0; i < strlen(text); i++) 
    {
        u8 c = text[i];
        if (c >= '0' && c <= '9') { buffer[pos++] = 0x82; buffer[pos++] = 0x4F + c - '0'; }
        else if (c >= 'A' && c <= 'Z') { buffer[pos++] = 0x82; buffer[pos++] = 0x60 + c - 'A'; }
        else if (c >= 'a' && c <= 'z') { buffer[pos++] = 0x82; buffer[pos++] = 0x81 + c - 'a'; }
        else if (c == '(') { buffer[pos++] = 0x81; buffer[pos++] = 0x69; }
        else if (c == ')') { buffer[pos++] = 0x81; buffer[pos++] = 0x6A; }
        else /* space */ { buffer[pos++] = 0x81; buffer[pos++] = 0x40; }
    }
}

static void initSaveFile(SaveFile *file, const char *name) 
{
	file->id = 0x4353;
 	file->iconDisplayFlag = 0x11;
 	file->iconBlockNum = 1;
  	toShiftJIS(file->title, name);
 	memcpy(file->iconPalette, saveIconPalette, 32);
 	memcpy(file->iconImage, saveIconImage, 128);
}

void defaultSettings()
{
	//stage.prefs.botplay = 1;	

	/*for (int i = 0; i < StageId_Max; i++)
	{
		stage.prefs.savescore[i][0] = 0;
		stage.prefs.savescore[i][1] = 0;
		stage.prefs.savescore[i][2] = 0;
	}*/
}

boolean readSaveFile()
{
	int fd = open(savetitle, 0x0001);
	if (fd < 0) // file doesnt exist 
		return false;

	SaveFile file;
	if (read(fd, (void *) &file, sizeof(SaveFile)) == sizeof(SaveFile)) 
		printf("ok\n");
	else {
		printf("read error\n");
		return false;
	}
	memcpy((void *) &stage.prefs, (const void *) file.saveData, sizeof(stage.prefs));
	close(fd);
	return true;
}

boolean MemoryCardExists(void)
{
	// If memory card status dont return 0x11 this means the memory card probably exists
	return _card_status(0) != 0x11;
}

void WriteSaveDataStructToBinaryAndSaveItOnTheFuckingMemoryCard()
{	
	int fd = open(savetitle, 0x0002);

	if (fd < 0) // if save doesnt exist make one
		fd =  open(savetitle, 0x0202 | (1 << 16));

	SaveFile file;
	initSaveFile(&file, savename);
  	memcpy((void *) file.saveData, (const void *) &stage.prefs, sizeof(stage.prefs));
	
	if (fd >= 0) {
	  	if (write(fd, (void *) &file, sizeof(SaveFile)) == sizeof(SaveFile)) 
	  		printf("ok\n");
	 	else 
	 		printf("write error\n");  // if save doesnt exist do a error
		close(fd);
	} 
	else 
		printf("open error %d\n", fd);  // failed to save
}
