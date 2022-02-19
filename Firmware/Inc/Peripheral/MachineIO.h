#ifndef __MACHINE_IO_H
#define __MACHINE_IO_H

#include "gpio.h"

//void setFullColorLed(uint8_t light);
void SetUiLed(uint8_t light);
void SetSideSensorUiLed(uint8_t light);
uint8_t GetPushSw1(void);
uint8_t GetPushSw2(void);

#endif /* __MACHINE_IO_H */
