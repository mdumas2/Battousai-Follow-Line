 /**
 * @file flash.c
 * @author yuta S.
 * @date 2020/4/15
 * https://github.com/YutaSeya/rokisi
 * @modified by Mateus L.
 * @date 2021/4
 */

#include <Peripheral/Flash.h>
#include <string.h>

//1byte
#define FLASH_TYPEPROGRAM_BYTE  0x00000000U
//0x1011
#define FLASH_SENTOR11					0x58

// endereço da flash que sera usado ( setor11 )
const uint32_t m_startAddress = 0x80E0000; //endereço de inicio do setor11
const uint32_t m_endAdress = 0x80FFFFF;

/**
 * Resumo: Aguarda pelo processamento da FLASH
 */
static void FlashWaitBusy(void)
{
	while (((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY) == 1)
		;
}
/**
 * Resumo: Apaga o setor11 da FLASH
 */
void FlashErase(void)
{
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= FLASH_SENTOR11 & FLASH_CR_SNB_Msk;
	FLASH->CR |= FLASH_CR_STRT;
	FlashWaitBusy();
}

/**
 * Resumo: Escreve o byte em um endereço especifico
 * 1° parâmetro endereço onde quer escrever
 * 2° parâmetro informação que sera escrita
 */
void FlashWriteByte(uint32_t address, uint8_t data)
{

	FLASH->CR &= ~(FLASH_CR_PSIZE);

	FLASH->CR |= FLASH_TYPEPROGRAM_BYTE;
	FLASH->CR |= FLASH_CR_PG;

	*(__IO uint8_t*) address = data;

	FlashWaitBusy();

	FLASH->CR &= ~(FLASH_CR_PG);
}

/**
 * Resumo: apaga e escreve um dado em um setor da memória
 * 1° parâmetro uint32_t endereço inicial
 * 2° parâmetro uint8_t * dado que vai ser escrito
 * 3° parâmetro uint32_t tamanho do dado
 * Detalhes:
 * Exemplo:
 * temos a estrutura:
 * typedef struct
 * {
 * idade int;
 * nome char[16];
 * }Pessoa;
 *
 * Pessoa a;
 * a.idade = 17;
 * a.nome = "sora";
 *
 * FLASH_WriteData (start_address, (uint8_t *)&a, sizeof (Pessoa));
 */
void FlashWriteData(uint32_t address, uint8_t* data, uint32_t size)
{
	FlashUnlock();

	FlashErase();

	do
	{
		FlashWriteByte(address, *data);
	} while (++address, ++data, --size);

	FlashLock();
}

/**
 * Resumo: escreve um dado em um setor da memória
 * 1° parâmetro uint32_t endereço inicial
 * 2° parâmetro uint8_t * dado que vai ser escrito
 * 3° parâmetro uint32_t tamanho do dado
 * Detalhes:
 * A condião de uso é que o setor onde a informação vai ser
 * escrita tem que estar limpo
 * Exemplo:
 * temos a estrutura:
 * typedef struct
 * {
 * idade int;
 * nome char[16];
 * }Pessoa;
 *
 * Pessoa a;
 * a.idade = 17;
 * a.nome = "sora";
 *
 * FLASH_WriteData (start_address, (uint8_t *)&a, sizeof (Pessoa));
 *
 *  *  Pessoa b;
 *  b.idade = 40;
 *  b.nome = "maou"
 *
 *  FLASH_AddWriteData(start_address + sizeof(Human), (uint8_t*)&b, sizeof(Pessoa));
 */
void FlashAddWriteData(uint32_t address, uint8_t* data, uint32_t size)
{
	FlashUnlock();

	do
	{
		FlashWriteByte(address, *data);
	} while (++address, ++data, --size);

	FlashLock();

}

/**
 * Resumo: le um dado de um setor da memória
 * 1° parâmetro uint32_t endereço
 * 2° parâmetro uint8_t * dado que vai ser lido
 * 3° parâmetro uint32_t tamanho do dado
 * Detalhes:
 * Exemplo:
 * temos a estrutura:
 * typedef struct
 * {
 * idade int;
 * nome char[16];
 * }Pessoa;
 *
 * Pessoa a;
 *
 * FLASH_ReadData(start_address, (uint8_t*)&a, sizeof(Pessoa));
 */
void FlashReadData(uint32_t address, uint8_t* data, uint32_t size)
{
	memcpy(data, (uint8_t*) address, size);
}
