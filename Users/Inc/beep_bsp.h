#ifndef __BEEP_BSP_H
#define __BEEP_BSP_H

#include "stm32f1xx_hal.h"
#include "main.h"


typedef enum 
{
    BEEP_NOTE_C4 = 262,
    BEEP_NOTE_D4 = 294,
    BEEP_NOTE_E4 = 330,
    BEEP_NOTE_F4 = 349,
    BEEP_NOTE_G4 = 392,
    BEEP_NOTE_A4 = 440,
    BEEP_NOTE_B4 = 494,
}BEEP_NOTE;

void set_beep_note(BEEP_NOTE note);
#endif /*__BEEP_BSP_H*/
