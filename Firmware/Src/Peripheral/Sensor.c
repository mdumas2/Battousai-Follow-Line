 /**
 * @file sensor.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <stdio.h>
#include <math.h>
#include <Peripheral/MachineIO.h>
#include <Peripheral/Sensor.h>

#define NO_CONVERTION 0
#define CONVERTION 1
#define LEFT 2
#define RIGHT 3
#define CONVERTION_REDEAY 4
#define SENSOR_QUANTITY 12

#define ADC_CONVERT_DATA_SIZE	((uint32_t)8)

// local varaiable

// adc convertion data
uint16_t m_adcConvertData[ADC_CONVERT_DATA_SIZE];
// adc convertion left data
uint16_t m_adcConvertLeft[ADC_CONVERT_DATA_SIZE];
// adc convertion right data
uint16_t m_adcConvertRight[ADC_CONVERT_DATA_SIZE];

// enable convertion : 1, disable convertion : 0
uint8_t m_adcStatus = 0;

typedef struct sensores
{
	volatile uint16_t whiteLineValue;
	volatile uint16_t calibratedValue;
	volatile uint16_t maxValue;
	volatile uint16_t minValue;
	volatile uint16_t average;

} sensores;
sensores sensor[8];

/**
 * Resumo: Habilita o ADC2
 */
void Adc2Start(void)
{
	LL_ADC_Enable(ADC2);
}

/**
 * Resumo: faz a conversão do ADC2 e calcula a tensão na bateria
 * Retorna a tensão na bateria.
 */
float GetBatteryVoltage(void)
{
	uint16_t data = 0;
	float voltage = 0.0f;
	LL_ADC_REG_StartConversionSWStart(ADC2);
	while (LL_ADC_IsActiveFlag_EOCS(ADC2) == RESET)
		;
	LL_ADC_ClearFlag_EOCS(ADC2);
	data = LL_ADC_REG_ReadConversionData10(ADC2);
	voltage = (float) (BATT_CONVERTION_CONST * data);
	return voltage;
}

/**
 * Resumo: Habilita o ADC1
 */
void Adc1Start(void)
{
	LL_ADC_Enable(ADC1);
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);
}

/**
 * Resumo: Habilita a conversão do ADC1 com o DMA
 */
void Adc1Dma2StartConvertion(void)
{
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);

	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_0,
			LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
			(uint32_t) &m_adcConvertData, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC_CONVERT_DATA_SIZE);

	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);

	LL_ADC_REG_StartConversionSWStart(ADC1);

}

/**
 * Resumo: Inicia a conversão
 */
void AdcStartConvertion(void)
{
	m_adcStatus = CONVERTION_REDEAY;
}

/**
 * Resumo: Para a conversão
 */
void AdcStopConvertion(void)
{
	m_adcStatus = NO_CONVERTION;
}

/**
 * Resumo: Realiza o calculo para reconhecer a posção da linha
 * entre os sensores
 */
float SensingLineSensor(void)
{

	volatile float valorPosicao;
	float m;
	float d;

	m = ((0 * (sensor[7].calibratedValue - sensor[0].calibratedValue))
			+ (3.5 * (sensor[6].calibratedValue - sensor[1].calibratedValue))
			+ (2 * (sensor[5].calibratedValue - sensor[2].calibratedValue))
			+ (1 * (sensor[4].calibratedValue - sensor[3].calibratedValue)));
	d = (sensor[0].calibratedValue + sensor[1].calibratedValue
			+ sensor[2].calibratedValue + sensor[3].calibratedValue
			+ sensor[4].calibratedValue + sensor[5].calibratedValue
			+ sensor[6].calibratedValue + sensor[7].calibratedValue);

	valorPosicao = asin(-m / d);

	return valorPosicao;
}

/**
 * Resumo: Inicia os valores do sensor para em seguida realizar a calibração
 */
void LineSensorInit(void)
{
	UpdateLineSensor();
	for (int j = 0; j < SENSOR_QUANTITY; j++)
	{
		sensor[j].minValue = 400;
		sensor[j].maxValue = 2300;
		sensor[j].whiteLineValue = 260;
	}
}

/**
 * Resumo: Realiza a calibração dos sensores
 */
void CalibrateSensor(void)
{
	for (int j = 0; j < SENSOR_QUANTITY; j++)
	{
		if (m_adcConvertData[j] > sensor[j].maxValue)
		{
			sensor[j].maxValue = m_adcConvertData[j];
		}
		if (m_adcConvertData[j] < sensor[j].minValue)
		{
			sensor[j].minValue = m_adcConvertData[j];
		}
	}
	for (int i = 0; i < 8; i++)
	{
		sensor[i].average = (((sensor[i].maxValue - sensor[i].minValue)) / 2);
	}
}

/**
 * Resumo: realiza a leitura dos sensores em relação
 * aos valores calibrados
 */
void GetSensorCalibrated(void)
{
	int i;
	for (i = 0; i < SENSOR_QUANTITY; i++)
	{
		sensor[i].calibratedValue = m_adcConvertRight[i];
		if (sensor[i].calibratedValue >= sensor[i].average)
		{
			sensor[i].calibratedValue = 3000;
		}
		else
		{
			sensor[i].calibratedValue = m_adcConvertRight[i];
		}
	}
}

/**
 * Resumo: atualiza a função dos sensores
 */
void UpdateLineSensor(void)
{
	volatile uint16_t i = 0;
	if (m_adcStatus == LEFT)
	{

		// LL_GPIO_ResetOutputPin(fet_side_left_GPIO_Port, fet_side_left_Pin);
		LL_GPIO_ResetOutputPin(GPIO_OUT_HEAD_GPIO_Port, GPIO_OUT_HEAD_Pin);

		// LL_GPIO_SetOutputPin(fet_side_right_GPIO_Port, fet_side_right_Pin);
		LL_GPIO_SetOutputPin(GPIO_OUT_HEAD_GPIO_Port, GPIO_OUT_HEAD_Pin);

		for (i = 0; i < 250; i++)
			;

		for (i = 0; i < ADC_CONVERT_DATA_SIZE; i++)
		{
			m_adcConvertLeft[i] = m_adcConvertData[i];
		}
		m_adcStatus = RIGHT;
		Adc1Dma2StartConvertion();
	}
	else if (m_adcStatus == RIGHT)
	{

		// LL_GPIO_ResetOutputPin(fet_side_right_GPIO_Port, fet_side_right_Pin);
		LL_GPIO_ResetOutputPin(GPIO_OUT_HEAD_GPIO_Port, GPIO_OUT_HEAD_Pin);

		for (i = 0; i < ADC_CONVERT_DATA_SIZE; i++)
		{
			m_adcConvertRight[i] = m_adcConvertData[i];
		}
		GetSensorCalibrated();
		SensingLineSensor();
		m_adcStatus = CONVERTION_REDEAY;
	}
}

/**
 * Resumo: verifica a conversão do ADC1 com DMA
 */
void CheckAdcProcess(void)
{
	volatile int i;

	if (m_adcStatus == CONVERTION_REDEAY)
	{

		for (i = 0; i < 500; i++)
			;
		m_adcStatus = LEFT;
		Adc1Dma2StartConvertion();
	}
}

/**
 * Resumo: função de callback
 * Detalhes:
 * Essa função chama DMA2_Stream0_IRQHandler(void)
 * limpa a flag EOCS e atualiza o sensor
 */
void Adc1Dma2TransferCompleteCallback(void)
{
	// clear EOCS Flag
	LL_ADC_ClearFlag_EOCS(ADC1);
	// update line sensor value
	UpdateLineSensor();

}

/**
 * Resumo: printa os valores do adc
 */
void show_adc(void)
{
	// you right printf and check adc_convert_left/right
}
