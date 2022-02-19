#ifndef __MONTION_H
#define __MONTION_H

#include <stdint.h>

float straight(float target_speed_x, float actual_speed_x);
float turn(float target_speed_w, float actual_speed_w);
int need_to_decelerate(float total_distance, float travelled_distance,
		float actual_speed, float end_speed);

#endif /* __ENCODER_H */
