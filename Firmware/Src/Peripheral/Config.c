 /**
 * @file config.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/Config.h>
#include <stdlib.h>

volatile uint32_t ticks;

/**
 * Resumo: Transforma de graus para radianos
 * Detalhes:
 */
float DegreesToRadians(float deg)
{
	float rad = (deg * PI) / 180;
	return rad;
}

/**
 * Resumo: Transforma de radianos para graus
 * Detalhes:
 */
float RadiansToDegrees(float rad)
{
	float deg = (rad * 180) / PI;
	return deg;
}

/**
 * Resumo: faz a conversão de duty cycle para pwm
 * 1° parâmetro valor em duty cycle
 * Retorna o dado em pwm.
 */
int DutyToPWM(float duty)
{
	int pwm = abs(((MACHINE_MAX_PWM_COUNT * duty) / 100));
	return pwm;
}

/**
 * Resumo: faz a conversão de pwm para duty cycle
 * 1° parâmetro valor em duty pwm
 * Retorna o dado em duty cycle.
 */
float PwmToDuty(int pwm)
{
	float duty = ((100 * pwm) / MACHINE_MAX_PWM_COUNT);
	return duty;
}

/**
 * Resumo: faz a conversão de speed para duty cycle
 * 1° parâmetro valor em duty cycle
 * Retorna o dado em duty cycle.
 */
float SpeedToDuty(float speed)
{
	float duty = (2 * speed) / MACHINE_MAX_LINEAR_VELOCITY;
	return duty;
}

/**
 * Resumo: Faz a contagem de tempo do programa
 */
void ClockTick(void)
{
	ticks++;
}

/**
 * Resumo: Retorna a constante de tempo do programa
 */
uint32_t GetClockTicks(void)
{
	return ticks;
}

//1599=100%=MACHINE_MAX_LINEAR_VELOCITY
