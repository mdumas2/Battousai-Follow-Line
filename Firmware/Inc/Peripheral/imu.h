#ifndef __IMU_H
#define __IMU_H

#include "spi.h"

void spi2_start(void);
uint8_t mpu6500_init(void);
void mpu6500_start_calibration(void);
void mpu6500_calibrate_sensor(void);
uint8_t mpu6500_get_status(void);
float mpu6500_get_gyro_x(void);
float mpu6500_get_gyro_y(void);
float mpu6500_get_gyro_z(void);
float mpu6500_get_accel_x(void);
float mpu6500_get_accel_y(void);
float mpu6500_get_accel_z(void);

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile float beta;				// algorithm gain
extern volatile float q0, q1, q2, q3;// quaternion of sensor frame relative to auxiliary frame
extern volatile float roll, pitch, yaw; // euler representation

//---------------------------------------------------------------------------------------------------
// Function declarations

void update_imu(float gx, float gy, float gz, float ax, float ay, float az);

#endif /* __MPU6500_H */
