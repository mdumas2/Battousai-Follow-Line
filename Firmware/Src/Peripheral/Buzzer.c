 /**
 * @file buzzer.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/Buzzer.h>

void Tim11Start(void)
{
	LL_TIM_CC_EnableChannel(TIM11, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM11);
}

static void Tim11PwmOut(uint32_t pwm)
{
	if (pwm > 99)
		pwm = 99;
	LL_TIM_OC_SetCompareCH1(TIM11, pwm);
}

void ControlBuzzer(uint16_t scale, uint16_t timeBeep)
{
	LL_TIM_SetPrescaler(TIM11, scale);
	Tim11PwmOut(99);
	LL_mDelay(timeBeep);
	Tim11PwmOut(0);
}
