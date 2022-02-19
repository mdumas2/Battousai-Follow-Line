 /**
 * @file pid.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Controller/pid.h>
#include <Peripheral/Config.h>
#include <stdlib.h>

float pwm_l_pid, pwm_r_pid, pwm_r_ff, pwm_l_ff, real_l_pwm, real_r_pwm;

void feedforward(int n_r, int n_l, float baterry_voltage, float acceleration)
{

	//n é o número de rotações no motor e pode ser encontrado a partir do valor de contagem do encoder
	//acceleration é a aceleração pedida para o robô

	float t_tire; //torque do pneu
	float t_motor; //torque do motor
	float current; //corrente no motor
	float motor_voltage_r, motor_voltage_l; //tensão necessária no motor
	float counter_emf_r, counter_emf_l; //força contra eletromotriz no motor
	float r_duty, l_duty;

	t_tire = MACHINE_WEIGHT * acceleration / 2 * MACHINE_WHEEL_RADIUS;
	t_motor = t_tire / GEAR_RATION;
	current = t_motor / MACHINE_TORQUE_CONSTANT;

	counter_emf_r = MACHINE_VOLTAGE_CONSTANT * n_r;
	counter_emf_l = MACHINE_VOLTAGE_CONSTANT * n_l;

	motor_voltage_r = MACHINE_MOTOR_RESISTANCE * current + counter_emf_r;
	motor_voltage_l = MACHINE_MOTOR_RESISTANCE * current + counter_emf_l;

	//Ao controlar o motor de fato, a relação de trabalho r_duty da onda PWM é alterada
	r_duty = KP_FF * (motor_voltage_r / baterry_voltage);
	l_duty = KP_FF * (motor_voltage_l / baterry_voltage);
	pwm_r_ff = duty_to_pwm(r_duty);
	pwm_l_ff = duty_to_pwm(l_duty);

}

/**
 * Resumo: Calcula o valor do pid
 * 1° parâmetro velocidade linear alvo em m/s.
 * 2° parâmetro velocidade angular alvo em deg/s.
 * 3° parâmetro velocidade linear atual em m/s.
 * 4° parâmetro velocidade angular atual em deg/s.
 */
void set_pid_parameter(float pv_x, float pv_w, float speed_x, float speed_w)
{

	float pos_pwm_x;
	float pos_pwm_w;

	erro_pos_x += speed_x - pv_x;
	erro_pos_w += speed_w - pv_w;
	pos_pwm_x = KP_X * erro_pos_x + KD_X * (erro_pos_x - old_error_pos_x);
	pos_pwm_w = KP_W * erro_pos_w + KD_W * (erro_pos_w - old_error_pos_w);

	old_error_pos_x = erro_pos_x;
	old_error_pos_w = erro_pos_w;

	pwm_l_pid = pos_pwm_x - pos_pwm_w;
	pwm_r_pid = pos_pwm_x + pos_pwm_w;
}

/**
 * Resumo: Calcula o valor final que será aplicado nos motores
 * levando em consideração o pid e o feedforward
 */
void set_final_pwm(void)
{

	real_l_pwm = pwm_l_ff + pwm_l_pid;
	real_r_pwm = pwm_r_ff - pwm_r_pid;

}

/**
 * Resumo: atualiza o valor do pwm do motor esquerdo
 * Retorna o valor do pwm do motor esquerdo.
 */
float update_pwm_l(void)
{
	return real_l_pwm;
}

/**
 * Resumo: atualiza o valor do pwm do motor direito
 * Retorna o valor do pwm do motor direito.
 */
float update_pwm_r(void)
{
	return real_r_pwm;
}
