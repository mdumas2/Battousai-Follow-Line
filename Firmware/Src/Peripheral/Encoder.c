 /**
 * @file encoder.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <math.h>
#include <Peripheral/Config.h>
#include <Peripheral/Encoder.h>

#define MA700_MAX_COUNT 4090
#define MA700_HALF_COUNT 2044

int16_t m_leftPrevious = 0;
int16_t m_rightPrevious = 0;

float m_prevLeftVel = 0.0f;
float m_prevRightVel = 0.0f;

/**
 * Resumo: Habilita a comunicação SPI3 com o encoder da direita
 * Detalhes:
 * Habilita o SPI3 e da o seta o CS do encoder da direita para alto.
 */
void Spi3Start(void)
{
	LL_SPI_Enable(SPI3);
	LL_GPIO_SetOutputPin(GPIO_OUT_ENC_R_Cs_GPIO_Port, GPIO_OUT_ENC_R_Cs_Pin);
}

/**
 * Resumo: Realiza a comunicação com o SPI3
 * 1° parâmetro tx_data escreve em um array de dados.
 * 2° parâmetro rx_data le de um array de dados.
 * 3° parâmetro o tamanho das informações que seram passadas na comunicação
 * Detalhes:
 * Os arrays tx_data e rx_data precisam ter o mesmo tamanho que o 3° parâmetro
 * (length), por que a comunicação é do tipo full duplex.
 */
static void Spi3Communication(uint8_t *tx_data, uint8_t *rx_data,
		uint8_t length)
{
	uint8_t count = length;

	LL_GPIO_ResetOutputPin(GPIO_OUT_ENC_R_Cs_GPIO_Port, GPIO_OUT_ENC_R_Cs_Pin);

	if (LL_SPI_IsActiveFlag_RXNE(SPI3) == SET)
		LL_SPI_ReceiveData8(SPI3);

	if (LL_SPI_IsEnabled(SPI3) == RESET)
		LL_SPI_Enable(SPI3);

	while (count > 0)
	{
		LL_SPI_TransmitData8(SPI3, *tx_data++);
		while (LL_SPI_IsActiveFlag_TXE(SPI3) == RESET)
			;
		while (LL_SPI_IsActiveFlag_RXNE(SPI3) == RESET)
			;
		*rx_data++ = LL_SPI_ReceiveData8(SPI3);
		count--;
	}

	LL_GPIO_SetOutputPin(GPIO_OUT_ENC_R_Cs_GPIO_Port, GPIO_OUT_ENC_R_Cs_Pin);

}

/**
 * Resumo: Habilita a comunicação SPI1 com o encoder da esquerda
 * Detalhes:
 * Habilita o SPI1 e da o seta o CS do encoder da esquerda para alto.
 */
void Spi1Start(void)
{
	LL_SPI_Enable(SPI1);
	LL_GPIO_SetOutputPin(GPIO_OUT_ENC_L_Cs_GPIO_Port, GPIO_OUT_ENC_L_Cs_Pin);
}

/**
 * Resumo: Realiza a comunicação com o SPI1
 * 1° parâmetro tx_data escreve em um array de dados.
 * 2° parâmetro rx_data le de um array de dados.
 * 3° parâmetro o tamanho das informações que seram passadas na comunicação
 * Detalhes:
 * Os arrays tx_data e rx_data precisam ter o mesmo tamanho que o 3° parâmetro
 * (length), por que a comunicação é do tipo full duplex.
 */
static void Spi1Communication(uint8_t *tx_data, uint8_t *rx_data,
		uint8_t length)
{
	uint8_t count = length;

	LL_GPIO_ResetOutputPin(GPIO_OUT_ENC_L_Cs_GPIO_Port, GPIO_OUT_ENC_L_Cs_Pin);

	if (LL_SPI_IsActiveFlag_RXNE(SPI1) == SET)
		LL_SPI_ReceiveData8(SPI1);

	if (LL_SPI_IsEnabled(SPI1) == RESET)
		LL_SPI_Enable(SPI1);

	while (count > 0)
	{
		LL_SPI_TransmitData8(SPI1, *tx_data++);
		while (LL_SPI_IsActiveFlag_TXE(SPI1) == RESET)
			;
		while (LL_SPI_IsActiveFlag_RXNE(SPI1) == RESET)
			;
		*rx_data++ = LL_SPI_ReceiveData8(SPI1);
		count--;
	}

	LL_GPIO_SetOutputPin(GPIO_OUT_ENC_L_Cs_GPIO_Port, GPIO_OUT_ENC_L_Cs_Pin);

}
/**
 * Resumo: Retorna o valor obtido pelo encoder da direita MA700
 * Detalhes:
 * Retorna um dado de 12bits do encoder.
 */
static uint16_t MA700RightGetEncoderData(void)
{
	uint8_t tx_data[2];
	uint8_t rx_data[2];
	uint16_t data = 0;

	tx_data[0] = 0x00;
	tx_data[1] = 0x00;

	Spi3Communication(tx_data, rx_data, 2);

	data = (uint16_t) ((uint16_t) (rx_data[0] << 4)
			| (uint16_t) (rx_data[1] & 0x0C));

	return data;
}

/**
 * Resumo: Retorna o valor obtido pelo encoder da esquerda MA700
 * Detalhes:
 * Retorna um dado de 12bits do encoder.
 */
static uint16_t MA700LeftGetEncoderData(void)
{
	uint8_t tx_data[2];
	uint8_t rx_data[2];
	uint16_t data = 0;

	tx_data[0] = 0x00;
	tx_data[1] = 0x00;

	Spi1Communication(tx_data, rx_data, 2);

	data = (uint16_t) ((uint16_t) (rx_data[0] << 4)
			| (uint16_t) (rx_data[1] & 0x0C));

	return data;
}

/**
 * Resumo: Retorna os valores obtidos pelos encoders em unidades
 * fisicas reais
 * 1° parâmetro left ponteiro para a variavel left.
 * 2° parâmetro right ponteiro para a variavel right.
 * 3° parâmetro current_angle angulação atual do robô.
 * 4° parâmetro left ponteiro para a variavel position_x.
 * 5° parâmetro right ponteiro para a variavel position_y.
 * Detalhes:
 */
void UpdateEncoder(float *left, float *right, float current_angle,
		float *position_x, float *position_y)
{
	int16_t left_now = 0;
	int16_t right_now = 0;
	int16_t left_count = 0;
	int16_t right_count = 0;
	float left_omega = 0.0f;
	float right_omega = 0.0f;
	float left_now_vel = 0.0f;
	float right_now_vel = 0.0f;
	float avg = 0.0f;

	left_now = MA700LeftGetEncoderData();
	right_now = MA700RightGetEncoderData();

	left_count = left_now - m_leftPrevious;
	right_count = right_now - m_rightPrevious;

	m_leftPrevious = left_now;
	m_rightPrevious = right_now;

	if (left_count > MA700_HALF_COUNT)
	{
		left_count -= MA700_MAX_COUNT;
	}
	else if (left_count < -MA700_HALF_COUNT)
	{
		left_count += MA700_MAX_COUNT;
	}

	if (right_count > MA700_HALF_COUNT)
	{
		right_count -= MA700_MAX_COUNT;
	}
	else if (right_count < -MA700_HALF_COUNT)
	{
		right_count += MA700_MAX_COUNT;
	}

	// calculate wheel omega

	left_omega = (float) (-left_count * 2.0f / MA700_MAX_COUNT * PI / dt);
	right_omega = (float) (right_count * 2.0f / MA700_MAX_COUNT * PI / dt);

	left_now_vel = left_omega * MACHINE_WHEEL_RADIUS;
	right_now_vel = right_omega * MACHINE_WHEEL_RADIUS;

	*left = (left_now_vel + m_prevLeftVel) / 2.0f;
	*right = (right_now_vel + m_prevRightVel) / 2.0f;

	m_prevLeftVel = left_now_vel;
	m_prevRightVel = right_now_vel;

	avg = ((left_now_vel / MACHINE_FREQUENCY_HZ)
			+ (right_now_vel / MACHINE_FREQUENCY_HZ)) / 2.0f;

	*position_x += avg * cos(current_angle);
	*position_y += avg * sin(current_angle);

}
