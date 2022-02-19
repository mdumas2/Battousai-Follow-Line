#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

#define FLASH_KEY1               0x45670123U
#define FLASH_KEY2               0xCDEF89ABU

// endereço usado pela flash ( sector11 )
extern const uint32_t m_startAddress; //endereço de inicio do setor11
extern const uint32_t m_endAdress;

__STATIC_INLINE void FlashLock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;

}

__STATIC_INLINE void FlashUnlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void FlashWriteData(uint32_t address, uint8_t* data, uint32_t size);
void FlashAddWriteData(uint32_t address, uint8_t* data, uint32_t size);
void FlashReadData(uint32_t address, uint8_t* data, uint32_t size);

#endif /* __FLASH_H */
