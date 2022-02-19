#ifndef __MOTOR_H
#define __MOTOR_H

#include "tim.h"

void Tim1Start(void);
void Tim9Start(void);
void ControlMotor(int32_t left, int32_t right);

#endif /* __MOTOR_H */
