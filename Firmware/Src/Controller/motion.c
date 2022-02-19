 /**
 * @file motion.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Controller/motion.h>
#include <Peripheral/Config.h>
#include <stdlib.h>

/**
 * Resumo: Atualiza a velocidade linear alvo que o robô deve ter
 * 1° parâmetro velocidade linear alvo em m/s.
 * 2° parâmetro velocidade linear atual em m/s.
 * Retorna a velocidade linear atual
 */
float straight(float target_speed_x, float actual_speed_x)
{
	if (actual_speed_x < target_speed_x)
	{
		actual_speed_x += (MACHINE_MAX_LINEAR_ACCELERATION * 2);
		if (actual_speed_x > target_speed_x)
			actual_speed_x = target_speed_x;
	}
	else if (actual_speed_x > target_speed_x)
	{
		actual_speed_x -= (MACHINE_MAX_LINEAR_DECELERATION * 2);
		if (actual_speed_x < target_speed_x)
			actual_speed_x = target_speed_x;
	}
	return actual_speed_x;

}

/**
 * Resumo: Atualiza a velocidade angular alvo que o robô deve ter
 * 1° parâmetro velocidade angular alvo em deg/s.
 * 2° parâmetro velocidade angular atual em deg/s.
 * Retorna a velocidade angular atual
 */
float turn(float target_speed_w, float actual_speed_w)
{
	if (actual_speed_w < target_speed_w)
	{
		actual_speed_w += MACHINE_MAX_ANGULAR_ACCELERATION;
		if (actual_speed_w > target_speed_w)
			actual_speed_w = target_speed_w;
	}
	else if (actual_speed_w > target_speed_w)
	{
		actual_speed_w -= MACHINE_MAX_ANGULAR_DECELERATION;
		if (actual_speed_w < target_speed_w)
			actual_speed_w = target_speed_w;
	}
	return actual_speed_w;
}

/**
 * Resumo: Analisa se é necessário frear o robô
 * 1° parâmetro distancia total em m.
 * 2° parâmetro distancia percorrida em m.
 * 3° parâmetro velocidade atual em m/s.
 * 4° parâmetro velocidade final em m/s.
 * Retorna 1 se você precisa desacelerar
 * e 0 caso não precise
 */
int need_to_decelerate(float total_distance, float travelled_distance,
		float actual_speed, float end_speed)
{

	float remaining_distance, needed_distance;

	remaining_distance = total_distance - travelled_distance;

	needed_distance = ((actual_speed * actual_speed - end_speed * end_speed) / 2)
			/ (MACHINE_MAX_LINEAR_DECELERATION);

	if (needed_distance < 0)
	{
		needed_distance = 0;
	}

	if (needed_distance < remaining_distance)
	{
		return 1;
	}

	return 0;

}

