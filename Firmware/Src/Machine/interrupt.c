 /**
 * @file interrupt.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Machine/interrupt.h>
#include <Trace/mapping.h>
#include <Controller/trapzoid.h>

static int8_t control_flag = 0;

void interrupt(void)
{
}
void set_control_flag(uint8_t flag)
{
	control_flag = flag;
}

