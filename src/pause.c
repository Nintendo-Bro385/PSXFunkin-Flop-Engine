#include "pause.h"
#include "audio.h"
#include "pad.h"
#include "trans.h"

static uint8_t pause_menu = 0;

void PausedState()
{
    static const char *stage_options[] = {
        "RESUME",
        "RESTART SONG",
        "EXIT TO MENU"
    };
    
    //Select option if cross or start is pressed
    if (pad_state.press & (PAD_CROSS | PAD_START))
    {
        switch (pause_select)
        {
            case 0: //Resume
                stage.paused = false;
                pause_select = 0;
                break;
            case 1: //Retry
                stage.trans = StageTrans_Reload;
                Trans_Start();
                pause_select = 0;
                break;
            case 2: //Quit
                stage.trans = StageTrans_Menu;
                Trans_Start();
                pause_select = 0;
                break;
        }
    }
