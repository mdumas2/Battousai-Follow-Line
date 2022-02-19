 /**
 * @file motor.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/Config.h>
#include <Peripheral/Motor.h>

/**
 * Resumo: Inicia o TIM1 e habilita as saidas
 */
void Tim1Start(void)
{
	LL_TIM_EnableAllOutputs(TIM1);

	LL_TIM_EnableCounter(TIM1);

}

/**
 * Resumo: Inicia o TIM9 e habilita as saidas
 */
void Tim9Start(void)
{
	LL_TIM_EnableAllOutputs(TIM9);

	LL_TIM_EnableCounter(TIM9);

}

/**
 * Resumo: configura o PWM que será enviado para os drivers
 * 1° parâmetro pwm do motor da esquerda
 * 2° parâmetro pwm do motor da direita
 * Detalhes:
 * Essa função tem limita os dados de entrada
 * sendo o limite inferior 0 e o superior 1599
 */
static void TimSetPwm(uint32_t left, uint32_t right)
{
	if (left > MACHINE_MAX_PWM_COUNT)
		left = MACHINE_MAX_PWM_COUNT;
	if (right > MACHINE_MAX_PWM_COUNT)
		right = MACHINE_MAX_PWM_COUNT;
	LL_TIM_OC_SetCompareCH1(TIM9, right);
	LL_TIM_OC_SetCompareCH4(TIM1, left);

	LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	LL_TIM_CC_DisableChannel(TIM9, LL_TIM_CHANNEL_CH1);

	LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	LL_TIM_CC_EnableChannel(TIM9, LL_TIM_CHANNEL_CH1);
}

/**
 * Resumo: controla a direção do motor sentido horario
 * e sentido anti horario
 * 1° parâmetro pwm do motor da esquerda
 * 2° parâmetro pwm do motor da direita
 */
void ControlMotor(int32_t left, int32_t right)
{
	if (left < 0)
	{
		left = -1 * left;
		LL_GPIO_ResetOutputPin(GPIO_OUT_MTR_L1_GPIO_Port, GPIO_OUT_MTR_L1_Pin);
		LL_GPIO_SetOutputPin(GPIO_OUT_MTR_L2_GPIO_Port, GPIO_OUT_MTR_L2_Pin);
	}
	else
	{
		LL_GPIO_SetOutputPin(GPIO_OUT_MTR_L1_GPIO_Port, GPIO_OUT_MTR_L1_Pin);
		LL_GPIO_ResetOutputPin(GPIO_OUT_MTR_L2_GPIO_Port, GPIO_OUT_MTR_L2_Pin);
	}

	if (right < 0)
	{
		right = -1 * right;
		LL_GPIO_ResetOutputPin(GPIO_OUT_MTR_R1_GPIO_Port, GPIO_OUT_MTR_R1_Pin);
		LL_GPIO_SetOutputPin(GPIO_OUT_MTR_R2_GPIO_Port, GPIO_OUT_MTR_R2_Pin);
	}
	else
	{
		LL_GPIO_SetOutputPin(GPIO_OUT_MTR_R1_GPIO_Port, GPIO_OUT_MTR_R1_Pin);
		LL_GPIO_ResetOutputPin(GPIO_OUT_MTR_R2_GPIO_Port, GPIO_OUT_MTR_R2_Pin);
	}

	TimSetPwm(left, right);
}

