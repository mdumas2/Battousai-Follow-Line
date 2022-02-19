 /**
 * @file imu.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/imu.h>
#include <math.h>
#include <Peripheral/Config.h>

/// Numero de calibrações que sao feitas
#define NUMBER_CALIBRATION 256

//---------------------------------------------------------------------------------------------------
// Definições
#define SAMPLE_FREQ	1000		// frequencia das amostras em Hz
#define BETA_DEF		0.1f		// 2 * ganho proporcional
#define MPU6500_GYRO_FACTOR 16.4f
#define MPU6500_ACCEL_FACTOR 0.00239257812f

/**
 * ENUM dos endereços do MPU650
 */
enum MPU6500_DEF
{
	MPU6500_WHO_AM_I = 0x75,
	MPU6500_POWER_ON = 0x00,
	MPU6500_CONFIG = 0x00,
	MPU6500_GYRO_CONFIG = 0x18,
	MPU6500_ACCEL_CONFIG = 0x10,
	MPU6500_REG_WHO_AM_I = 0x75,
	MPU6500_REG_PWR_MGMT_1 = 0x6B,
	MPU6500_REG_CONFIG = 0x1A,
	MPU6500_REG_GYRO_CONFIG = 0x1B,
	MPU6500_REG_ACCEL_CONFIG = 0x1C,
	MPU6500_REG_ACCEL_XOUT_H = 0x3B,
	MPU6500_REG_ACCEL_XOUT_L = 0x3C,
	MPU6500_REG_ACCEL_YOUT_H = 0x3D,
	MPU6500_REG_ACCEL_YOUT_L = 0x3E,
	MPU6500_REG_ACCEL_ZOUT_H = 0x3F,
	MPU6500_REG_ACCEL_ZOUT_L = 0x40,
	MPU6500_REG_GYRO_XOUT_H = 0x43,
	MPU6500_REG_GYRO_XOUT_L = 0x44,
	MPU6500_REG_GYRO_YOUT_H = 0x45,
	MPU6500_REG_GYRO_YOUT_L = 0x46,
	MPU6500_REG_GYRO_ZOUT_H = 0x47,
	MPU6500_REG_GYRO_ZOUT_L = 0x48,
};

//---------------------------------------------------------------------------------------------------
// Definição das variaveis

volatile float beta = BETA_DEF;					// 2 * ganho proporcional (Kp)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;// quaternion da estrutura do sensor em relação à estrutura auxiliar
volatile float roll, pitch, yaw;

uint16_t cnt_mpu_calibration = 0;
uint8_t mpu_status = 1;

float mpu6500_gyro_x_offset = 0.0f;
float mpu6500_gyro_y_offset = 0.0f;
float mpu6500_gyro_z_offset = 0.0f;

float mpu6500_accel_x_offset = 0.0f;
float mpu6500_accel_y_offset = 0.0f;
float mpu6500_accel_z_offset = 0.0f;

/**
 * Resumo: Habilita a comunicação SPI3 com o encoder da direita
 * Detalhes:
 * Habilita o SPI2 e da o seta o CS do encoder da direita para alto.
 */
void spi2_start(void)
{
	LL_SPI_Enable(SPI2);
	LL_GPIO_SetOutputPin(GPIO_OUT_IMU_Cs_GPIO_Port, GPIO_OUT_IMU_Cs_Pin);
}

/**
 * Resumo: Realiza a comunicação com o SPI2
 * 1° parâmetro tx_data escreve em um array de dados.
 * 2° parâmetro rx_data le de um array de dados.
 * 3° parâmetro o tamanho das informações que seram passadas na comunicação
 * Detalhes:
 * Os arrays tx_data e rx_data precisam ter o mesmo tamanho que o 3° parâmetro
 * (length), por que a comunicação é do tipo full duplex.
 */
static void spi2_communication(uint8_t *tx_data, uint8_t *rx_data,
		uint8_t length)
{
	uint8_t count = length;

	LL_GPIO_ResetOutputPin(GPIO_OUT_IMU_Cs_GPIO_Port, GPIO_OUT_IMU_Cs_Pin);

	if (LL_SPI_IsActiveFlag_RXNE(SPI2) == SET)
		LL_SPI_ReceiveData8(SPI2);

	if (LL_SPI_IsEnabled(SPI2) == RESET)
		LL_SPI_Enable(SPI2);

	while (count > 0)
	{
		LL_SPI_TransmitData8(SPI2, *tx_data++);
		while (LL_SPI_IsActiveFlag_TXE(SPI2) == RESET)
			;
		while (LL_SPI_IsActiveFlag_RXNE(SPI2) == RESET)
			;
		*rx_data++ = LL_SPI_ReceiveData8(SPI2);
		count--;
	}

	LL_GPIO_SetOutputPin(GPIO_OUT_IMU_Cs_GPIO_Port, GPIO_OUT_IMU_Cs_Pin);

}

/**
 * Resumo: Escreve um valor para o mpu-6500
 * 1° parâmetro reg seta o registrador onde você irá escrever.
 * 2° parâmetro value é o valor que você quer escrever
 */
static void mpu6500_write_byte(uint8_t reg, uint8_t val)
{
	uint8_t tx_data[2];
	uint8_t rx_data[2];

	tx_data[0] = reg & 0x7F;
	tx_data[1] = val;

	spi2_communication(tx_data, rx_data, 2);
}

/**
 * Resumo: Lê um valor do mpu-6500
 * 1° parâmetro reg seta o registrador de onde você irá ler.
 * Detalhes: retorna o dado do registrador escolhido
 */
static uint8_t mpu6500_read_byte(uint8_t reg)
{
	uint8_t tx_data[2];
	uint8_t rx_data[2];

	tx_data[0] = reg | 0x80;
	tx_data[1] = 0x00;

	spi2_communication(tx_data, rx_data, 2);

	return rx_data[1];
}

/**
 * Resumo: Inicia o MPU6500
 * Detalhes: retorna 1 se conseguir inicar e 0 se não conseguir
 */
uint8_t mpu6500_init(void)
{
	uint8_t who_am_i = 0;
	LL_mDelay(10);
	who_am_i = mpu6500_read_byte(MPU6500_REG_WHO_AM_I);
	if (who_am_i != MPU6500_WHO_AM_I)
	{
		LL_mDelay(10);
		who_am_i = mpu6500_read_byte(MPU6500_REG_WHO_AM_I);
		if (who_am_i != MPU6500_WHO_AM_I)
			return 0;
	}

	mpu6500_write_byte(MPU6500_REG_PWR_MGMT_1, MPU6500_POWER_ON);
	LL_mDelay(10);
	mpu6500_write_byte(MPU6500_REG_CONFIG, MPU6500_CONFIG);
	LL_mDelay(10);
	mpu6500_write_byte(MPU6500_REG_GYRO_CONFIG, MPU6500_GYRO_CONFIG);
	LL_mDelay(10);
	mpu6500_write_byte(MPU6500_REG_ACCEL_CONFIG, MPU6500_ACCEL_CONFIG);
	LL_mDelay(10);

	return 1;
}

/**
 * Resumo: Inicia a calibração do MPU6500
 */
void mpu6500_start_calibration(void)
{
	cnt_mpu_calibration = 0;
	mpu_status = 0;

	mpu6500_gyro_x_offset = 0.0f;
	mpu6500_gyro_y_offset = 0.0f;
	mpu6500_gyro_z_offset = 0.0f;

	mpu6500_accel_x_offset = 0.0f;
	mpu6500_accel_y_offset = 0.0f;
	mpu6500_accel_z_offset = 0.0f;
}

/**
 * Resumo: Calibra o MPU6500
 * Detalhes:
 * Essa função é chamada pelo interrupt handling.
 */
void mpu6500_calibrate_sensor(void)
{
	int16_t gyro_x = 0;
	int16_t gyro_y = 0;
	int16_t gyro_z = 0;
	int16_t accel_x = 0;
	int16_t accel_y = 0;
	int16_t accel_z = 0;

	gyro_x = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_XOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_XOUT_L)));

	gyro_y = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_YOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_YOUT_L)));

	gyro_z = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_ZOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_ZOUT_L)));

	accel_x = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_XOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_XOUT_L)));

	accel_y = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_YOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_YOUT_L)));

	accel_y = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_ZOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_ZOUT_L)));

	if (cnt_mpu_calibration < NUMBER_CALIBRATION)
	{
		mpu6500_gyro_x_offset += (float) gyro_x;
		mpu6500_gyro_y_offset += (float) gyro_y;
		mpu6500_gyro_z_offset += (float) gyro_z;
		mpu6500_accel_x_offset += (float) accel_x;
		mpu6500_accel_y_offset += (float) accel_y;
		mpu6500_accel_z_offset += (float) accel_z;
		cnt_mpu_calibration++;
	}
	else
	{
		mpu6500_gyro_x_offset = (float) (mpu6500_gyro_x_offset
				/ NUMBER_CALIBRATION);
		mpu6500_gyro_y_offset = (float) (mpu6500_gyro_y_offset
				/ NUMBER_CALIBRATION);
		mpu6500_gyro_z_offset = (float) (mpu6500_gyro_z_offset
				/ NUMBER_CALIBRATION);
		mpu6500_accel_x_offset = (float) (mpu6500_accel_x_offset
				/ NUMBER_CALIBRATION);
		mpu6500_accel_y_offset = (float) (mpu6500_accel_y_offset
				/ NUMBER_CALIBRATION);
		mpu6500_accel_z_offset = (float) (mpu6500_accel_z_offset
				/ NUMBER_CALIBRATION);
		mpu_status = 1;
	}
}

/**
 * Resumo: Pega o status do MPU6500
 * Detalhes: retorna 1 se ele estiver pronto e 0 caso esteja calibrando
 */
uint8_t mpu6500_get_status(void)
{
	return mpu_status;
}

/**
 * Resumo: Pega o valor medido pelo giroscópio no eixo x
 * Detalhes: retorna o valor em deg/sec
 */
float mpu6500_get_gyro_x(void)
{
	int16_t gyro = 0;
	float omega = 0.0f;

	gyro =
			(int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_XOUT_H)
					<< 8)
					| ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_XOUT_L)));
	omega = (float) ((gyro - mpu6500_gyro_z_offset) / MPU6500_GYRO_FACTOR);

	if (omega > -1.0f && omega < 1.0)
		omega = 0.0f;

	return omega;
}

/**
 * Resumo: Pega o valor medido pelo giroscópio no eixo y
 * Detalhes: retorna o valor em deg/sec
 */
float mpu6500_get_gyro_y(void)
{
	int16_t gyro = 0;
	float omega = 0.0f;

	gyro =
			(int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_YOUT_H)
					<< 8)
					| ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_YOUT_L)));
	omega = (float) ((gyro - mpu6500_gyro_z_offset) / MPU6500_GYRO_FACTOR);

	if (omega > -1.0f && omega < 1.0)
		omega = 0.0f;

	return omega;
}

/**
 * Resumo: Pega o valor medido pelo giroscópio no eixo z
 * Detalhes: retorna o valor em deg/sec
 */
float mpu6500_get_gyro_z(void)
{
	int16_t gyro = 0;
	float omega = 0.0f;

	gyro =
			(int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_ZOUT_H)
					<< 8)
					| ((int16_t) mpu6500_read_byte(MPU6500_REG_GYRO_ZOUT_L)));
	omega = (float) ((gyro - mpu6500_gyro_z_offset) / MPU6500_GYRO_FACTOR);

	if (omega > -1.0f && omega < 1.0)
		omega = 0.0f;

	return omega;
}

/**
 * Resumo: Pega o valor medido pelo aceleômetro no eixo x
 * Detalhes: retorna o valor em m/sec^2
 */
float mpu6500_get_accel_x(void)
{
	int16_t data = 0;
	float accel = 0.0f;
	data = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_XOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_XOUT_L)));
	accel = (float) ((data - mpu6500_accel_x_offset) / MPU6500_ACCEL_FACTOR);
	return accel;
}

/**
 * Resumo: Pega o valor medido pelo aceleômetro no eixo y
 * Detalhes: retorna o valor em m/sec^2
 */
float mpu6500_get_accel_y(void)
{
	int16_t data = 0;
	float accel = 0.0f;
	data = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_YOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_YOUT_L)));
	accel = (float) ((data - mpu6500_accel_x_offset) / MPU6500_ACCEL_FACTOR);
	return accel;
}

/**
 * Resumo: Pega o valor medido pelo aceleômetro no eixo z
 * Detalhes: retorna o valor em m/sec^2
 */
float mpu6500_get_accel_z(void)
{
	int16_t data = 0;
	float accel = 0.0f;
	data = (int16_t) (((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_ZOUT_H)
			<< 8) | ((int16_t) mpu6500_read_byte(MPU6500_REG_ACCEL_ZOUT_L)));
	accel = (float) ((data - mpu6500_accel_x_offset) / MPU6500_ACCEL_FACTOR);
	return accel;
}

/**
 * Resumo: Realiza a raiz quadrada inversa
 * Detalhes:
 * para saber mais sobre o código visite o site:
 * http://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
static float inv_sqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*) &i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**
 * Resumo: Transforma de Quaternion para Euler
 * Detalhes:
 */
static void compute_angles(void)
{
	roll = atan2f(q0 * q1 + q2 * q3, 0.5f - q1 * q1 - q2 * q2);
	pitch = asinf(-2.0f * (q1 * q3 - q0 * q2));
	yaw = atan2f(q1 * q2 + q0 * q3, 0.5f - q2 * q2 - q3 * q3);
}

/**
 * Resumo: Código de atualização do IMU
 * 1° parâmetro leitura do eixo x do giroscópio em rad/sec
 * 2° parâmetro leitura do eixo y do giroscópio em rad/sec
 * 3° parâmetro leitura do eixo z do giroscópio em rad/sec
 * 4° parâmetro leitura do eixo x do aceleômetro em m/sec^2
 * 5° parâmetro leitura do eixo y do aceleômetro em m/sec^2
 * 6° parâmetro leitura do eixo z do aceleômetro em m/sec^2
 * Detalhes: retorna a posição angular em Quaternion
 * para saber mais sobre o código visite o site:
 * https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
 * https://github.com/xioTechnologies/Fusion
 */
void update_imu(float gx, float gy, float gz, float ax, float ay, float az)
{
	float recip_norm;
	float s0, s1, s2, s3;
	float q_dot_1, q_dot_2, q_dot_3, q_dot_4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1,
			q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	q_dot_1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	q_dot_2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	q_dot_3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	q_dot_4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
	{

		// Normalise accelerometer measurement
		recip_norm = inv_sqrt(ax * ax + ay * ay + az * az);
		ax *= recip_norm;
		ay *= recip_norm;
		az *= recip_norm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1
				+ _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2
				+ _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recip_norm = inv_sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recip_norm;
		s1 *= recip_norm;
		s2 *= recip_norm;
		s3 *= recip_norm;

		// Apply feedback step
		q_dot_1 -= beta * s0;
		q_dot_2 -= beta * s1;
		q_dot_3 -= beta * s2;
		q_dot_4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += q_dot_1 * (1.0f / SAMPLE_FREQ);
	q1 += q_dot_2 * (1.0f / SAMPLE_FREQ);
	q2 += q_dot_3 * (1.0f / SAMPLE_FREQ);
	q3 += q_dot_4 * (1.0f / SAMPLE_FREQ);

	// Normalise quaternion
	recip_norm = inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recip_norm;
	q1 *= recip_norm;
	q2 *= recip_norm;
	q3 *= recip_norm;

	compute_angles();

}
