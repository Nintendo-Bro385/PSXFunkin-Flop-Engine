#include "pause.h"
#include "audio.h"
#include "pad.h"
#include "trans.h"
#include "stage.h"

static struct
{
	u8 pause_menu;
	u8 select;
}pause;

void PausedState()
{
    Audio_PauseXA();
    static const char *stage_options[] = {
        "RESUME",
        "RESTART SONG",
        "EXIT TO MENU"
    };

	//Change option
        if (pad_state.press & PAD_UP)
        {
            if (pause.select > 0)
                pause.select--;
            else
                pause.select = COUNT_OF(stage_options) - 1;
        }
        if (pad_state.press & PAD_DOWN)
        {
            if (pause.select < COUNT_OF(stage_options) - 1)
                pause.select++;
            else
                pause.select = 0;
        }
    //Select option if cross or start is pressed
    if (pad_state.press & (PAD_CROSS | PAD_START))
    {
        switch (pause.select)
        {
            case 0: //Resume
                stage.paused = false;
                pause.select = 0;
                break;
            case 1: //Retry
                stage.trans = StageTrans_Reload;
                Trans_Start();
                pause.select = 0;
                break;
            case 2: //Quit
                stage.trans = StageTrans_Menu;
                Trans_Start();
                pause.select = 0;
                break;
        }
    }
}
