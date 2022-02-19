 /**
 * @file trapzoid.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Controller/trapzoid.h>
#include <math.h>
#include <stdlib.h>

float target_max_speed_x;
float target_max_speed_w;

float target_final_speed_x;
float target_final_speed_w;

/**
 * Resumo: Atualiza a velocidade linear maxima alvo do trecho
 * Retorna o valor da velocidade linear maxima alvo.
 */
void make(float speed_x, float first_radius, float first_distance,
		float second_radius, float second_distance)
{

	if (isnan(first_radius) || isinf(first_radius) || abs(first_radius) > 100)
	{
		first_radius = 0;
	}

	if (first_radius == 0)
	{
		target_max_speed_x = sqrt(
				(speed_x * speed_x)
						- (first_distance * MACHINE_MAX_LINEAR_ACCELERATION));
		target_max_speed_w = 0;
	}

	if (first_radius != 0)
	{
		target_max_speed_x =
				sqrt(
						first_radius * G_ACCELERATION
								* MACHINE_COEFFICIENT_OF_FRICTION);
		//targetSpeedW = (targetSpeedX/radius)*(180/PI);
		//targetSpeedW = sqrt(turn_acceleration_mss * abs(radius));
	}

	if (isnan(second_radius) || isinf(second_radius)
			|| abs(second_radius) > 100)
	{
		first_radius = 0;
	}

	if (second_radius == 0)
	{
		target_final_speed_x = sqrt(
				(target_max_speed_x * target_max_speed_x)
						- (second_distance * MACHINE_MAX_LINEAR_ACCELERATION));
		target_final_speed_w = 0;
	}

	if (first_radius != 0)
	{
		target_final_speed_x =
				sqrt(
						second_radius * G_ACCELERATION
								* MACHINE_COEFFICIENT_OF_FRICTION);
		//targetSpeedW = (targetSpeedX/radius)*(180/PI);
		//targetSpeedW = sqrt(turn_acceleration_mss * abs(radius));
	}

}

/**
 * Resumo: Atualiza a velocidade linear maxima alvo do trecho
 * Retorna o valor da velocidade linear maxima alvo.
 */
float update_max_speed_x(void)
{
	return target_max_speed_x;
}

/**
 * Resumo: Atualiza a velocidade angular maxima alvo do trecho
 * Retorna o valor da velocidade angular maxima alvo.
 */
float update_max_speed_w(void)
{
	return target_max_speed_w;
}

/**
 * Resumo: Atualiza a velocidade linear final alvo do trecho
 * Retorna o valor da velocidade linear final alvo.
 */
float update_final_speed_x(void)
{
	return target_final_speed_x;
}

/**
 * Resumo: Atualiza a velocidade angular final alvo do trecho
 * Retorna o valor da velocidade angular final alvo.
 */
float update_final_speed_w(void)
{
	return target_final_speed_w;
}

