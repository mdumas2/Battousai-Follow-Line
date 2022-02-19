#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <stdint.h>

void interrupt(void);
void set_control_flag(uint8_t flag);

#endif /* __MACHINE_IO_H */
