#ifndef __ENCODER_H
#define __ENCODER_H

#include "spi.h"

void Spi3Start(void);
void Spi1Start(void);
void UpdateEncoder(float *left, float *right, float current_angle,
		float *position_x, float *position_y);

#endif /* __ENCODER_H */
