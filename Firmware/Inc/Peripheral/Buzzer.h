#ifndef __BUZZER_H
#define __BUZZER_H

#include "tim.h"
#include <stdint.h>

#define NORMAL 799
#define C_SCALE 3058
#define D_SCALE 2886
#define E_SCALE 2724
#define F_SCALE 2571
#define G_SCALE 1926
#define A_SCALE 1818U
#define B_SCALE 1620
#define C_H_SCALE 1529

void Tim11Start(void);
void ControlBuzzer(uint16_t scale, uint16_t timeBeep);

#endif /* __BUZZER_H */
