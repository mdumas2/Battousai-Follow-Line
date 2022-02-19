#ifndef __PID_H
#define __PID_H

float KP_X, KD_X, KP_W, KD_W, KP_FF, erro_pos_x, erro_pos_w, old_error_pos_x, old_error_pos_w;
void set_pid_parameter(float pv_x, float pv_w, float speed_x, float speed_w);
void feedfoward (int n_r, int n_l, float baterry_voltage, float acceleration);
float update_pwm_l(void);
float update_pwm_r(void);
void set_final_pwm (void);

#endif /* __ENCODER_H */
