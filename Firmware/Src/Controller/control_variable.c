 /**
 * @file control_variable.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Controller/control_variable.h>
#include <Peripheral/Config.h>

float sensors_prev_erro = 0.0f;

/**
 * Resumo: Devolve o feedback dos encoders que será utilizado no PID
 */
float update_feedback_encoderk_x(float delta_enc_r, float delta_enc_l)
{
	float feedbac_Encoderk_x;
	feedbac_Encoderk_x = (delta_enc_r + delta_enc_l) / 2;
	return feedbac_Encoderk_x;
}

/**
 * Resumo: Devolve o feedback do Giroscópio que será utilizado no PID
 */
float update_feedback_gyro(float gyro)
{
	float feedback_gyro;
	feedback_gyro = gyro;
	return feedback_gyro;
}

/**
 * Resumo: Devolve o feedback dos sensores infravermelhos que será utilizado no PID
 */
float update_feedback_sensors(float error_sensors)
{
	float feedback_sensors;
	feedback_sensors = (error_sensors - sensors_prev_erro) / dt;
	sensors_prev_erro = error_sensors;
	return feedback_sensors;
}
