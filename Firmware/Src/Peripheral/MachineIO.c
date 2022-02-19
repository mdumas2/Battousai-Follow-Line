 /**
 * @file machine_io.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/MachineIO.h>

/**
 * Resumo: configura a interface de led rgb do usuário
 * 1° parâmetro liga um led específico.
 * Detalhes:
 * lê os ultimos 3 bits
 * |blue |green| red |
 * |-----|-----|-----|
 * |  4  |  2  |  1  |
 */
/*void set_full_color_led(uint8_t light)
 {
 uint8_t red = (light & 0x01);
 uint8_t green = (light & 0x02) >> 1;
 uint8_t blue = (light & 0x04) >> 2;

 if(red == 1) LL_GPIO_ResetOutputPin(fled_red_GPIO_Port, fled_red_Pin);
 else LL_GPIO_SetOutputPin(fled_red_GPIO_Port, fled_red_Pin);

 if(green == 1) LL_GPIO_ResetOutputPin(fled_green_GPIO_Port, fled_green_Pin);
 else LL_GPIO_SetOutputPin(fled_green_GPIO_Port, fled_green_Pin);

 if(blue == 1) LL_GPIO_ResetOutputPin(fled_blue_GPIO_Port, fled_blue_Pin);
 else LL_GPIO_SetOutputPin(fled_blue_GPIO_Port, fled_blue_Pin);
 }
 */

/**
 * Resumo: configura a interface de led do usuário
 * 1° parâmetro liga um led específico.
 * Detalhes:
 * lê os ultimos 4 bits
 * |led4 |led3 |led2 |led1 |
 * |-----|-----|-----|-----|
 * |  8  |  4  |  2  |  1  |
 */
void SetUiLed(uint8_t light)
{
	uint8_t led1 = (light & 0x01);
	uint8_t led2 = (light & 0x02) >> 1;
	uint8_t led3 = (light & 0x04) >> 2;
	uint8_t led4 = (light & 0x08) >> 3;

	if (led1 == 1)
		LL_GPIO_ResetOutputPin(GPIO_IN_LED1_GPIO_Port, GPIO_IN_LED1_Pin);
	else
		LL_GPIO_SetOutputPin(GPIO_IN_LED1_GPIO_Port, GPIO_IN_LED1_Pin);

	if (led2 == 1)
		LL_GPIO_ResetOutputPin(GPIO_IN_LED2_GPIO_Port, GPIO_IN_LED2_Pin);
	else
		LL_GPIO_SetOutputPin(GPIO_IN_LED2_GPIO_Port, GPIO_IN_LED2_Pin);

	if (led3 == 1)
		LL_GPIO_ResetOutputPin(GPIO_IN_LED3_GPIO_Port, GPIO_IN_LED3_Pin);
	else
		LL_GPIO_SetOutputPin(GPIO_IN_LED3_GPIO_Port, GPIO_IN_LED3_Pin);

	if (led4 == 1)
		LL_GPIO_ResetOutputPin(GPIO_IN_LED4_GPIO_Port, GPIO_IN_LED4_Pin);
	else
		LL_GPIO_SetOutputPin(GPIO_IN_LED4_GPIO_Port, GPIO_IN_LED4_Pin);
}

/**
 * Resumo: configura a interface de led do usuário dos sensores laterais
 * 1° parâmetro liga um led específico.
 * Detalhes:
 * lê os ultimos 2 bits
 * |right|left |
 * |-----|-----|
 * |  2  |  1  |
 */
void SetSideSensorUiLed(uint8_t light)
{
	uint8_t left = (light & 0x01);
	uint8_t right = (light & 0x02) >> 1;

	if (left == 1)
		LL_GPIO_ResetOutputPin(GPIO_IN_LEDL_GPIO_Port, GPIO_IN_LEDL_Pin);
	else
		LL_GPIO_SetOutputPin(GPIO_IN_LEDL_GPIO_Port, GPIO_IN_LEDL_Pin);

	if (right == 1)
		LL_GPIO_ResetOutputPin( GPIO_IN_LEDR_GPIO_Port, GPIO_IN_LEDR_Pin);
	else
		LL_GPIO_SetOutputPin( GPIO_IN_LEDR_GPIO_Port, GPIO_IN_LEDR_Pin);
}

/**
 * Resumo: lê o estado do botão sw1
 * Detalhes:
 * Retorna 1 se o botão estiver acionado e 0 caso ele não esteja
 */
uint8_t GetPushSw1(void)
{
	return (uint8_t) LL_GPIO_IsInputPinSet(GPIO_IN_SW1_GPIO_Port,
	GPIO_IN_SW1_Pin);
}

/**
 * Resumo: lê o estado do botão sw2
 * Detalhes:
 * Retorna 1 se o botão estiver acionado e 0 caso ele não esteja
 */
uint8_t GetPushSw2(void)
{
	return (uint8_t) LL_GPIO_IsInputPinSet(GPIO_IN_SW2_GPIO_Port,
	GPIO_IN_SW2_Pin);
}
