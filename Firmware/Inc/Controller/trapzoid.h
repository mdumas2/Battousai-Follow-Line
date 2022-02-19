#ifndef __TRAPZOID_H
#define __TRAPZOID_H

#include <Peripheral/config.h>

void make(float speed_x, float first_radius, float first_distance,
		float second_radius, float second_distance);

float update_max_speed_x(void);
float update_max_speed_w(void);
float update_final_speed_x(void);
float update_final_speed_w(void);

//int32_t max_likelihood_counter_diff(uint16_t now, uint16_t before);

#endif /* __ENCODER_H */
