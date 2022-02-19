#ifndef __CONTROL_VARIABLE_H
#define __CONTROL_VARIABLE_H

float update_feedback_encoderk_x(float delta_enc_r, float delta_enc_l);
float update_feedback_gyro(float gyro);
float update_feedback_sensors(float error_sensors);

#endif /* __ENCODER_H */
