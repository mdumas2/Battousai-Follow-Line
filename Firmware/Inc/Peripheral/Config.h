#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

// my pi
#define PI 3.14159265358979f

// my pi
#define G_ACCELERATION 3.14159265358979f

/// control cycle
#define dt 0.001f

/// gear ration(80/20)
#define GEAR_RATION 4.0f

/// gear ration(80/20)
#define MACHINE_FREQUENCY_HZ 1000

/// machine tread(mm)
#define MACHINE_TREAD 99.0f

/// machine weight(kg)
#define MACHINE_WEIGHT 0.10f

/// machine wheel radious(mm)
#define MACHINE_WHEEL_RADIUS 13.5f

/// machine wheel radious(mm)
#define MACHINE_COEFFICIENT_OF_FRICTION 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_ANGULAR_VELOCITY 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_ANGULAR_ACCELERATION 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_ANGULAR_DECELERATION 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_LINEAR_VELOCITY 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_LINEAR_ACCELERATION 13.5f

/// machine wheel radious(mm)
#define MACHINE_MAX_LINEAR_DECELERATION 13.5f

/// machine wheel radious(mm)
#define MACHINE_MINIMUM_TIME_FOR_MAX_VELOCITY 13.5f

/// machine wheel radious(mm)
#define MACHINE_MINIMUM_DISTANCE_FOR_MAX_VELOCITY 13.5f

#define WHEELS_SEPARATION 13.5f

#define MICROMETERS_PER_TICK 13.5f

#define MICROMETERS_PER_METER 13.5f

#define DISTANCE_TO_MAX_LINEAR_VELOCITY 13.5f

#define MACHINE_TORQUE_CONSTANT 13.5f

#define MACHINE_MOTOR_RESISTANCE 13.5f

#define MACHINE_VOLTAGE_CONSTANT 13.5f

#define MACHINE_MAX_PWM_COUNT 1599

float RadiansToDegrees(float radians);
float DegreesToRadians(float degrees);
int DutyToPWM(float duty);
float PwmToDuty(int pwm);
float SpeedToDuty(float speed);
float SpeedToCounts(float speed); //Implementar
float CountsToSpeed(float counts); //Implementar
void ClockTick(void);
uint32_t GetClockTicks(void);

#endif /* __CONFIG_H */
