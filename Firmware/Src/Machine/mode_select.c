 /**
 * @file mode_select.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Machine/mode_select.h>
#include <Peripheral/Encoder.h>

static int execute;

int select(void)
{
	/**
	 * |led4 |led3 |led2 |led1 |
	 * |-----|-----|-----|-----|
	 * |  8  |  4  |  2  |  1  |
	 */
	int select = 0;
	float left=0;

	UpdateEncoder(&left, &left, left, &left, &left);

	if (execute == 0)
	{
		select = left;
		if (select > 15)
			select = 0;
		return select;
	}
	else
		return select;
}

int get_execute(void)
{
	int execute = GetPushSw1();
	if (execute > 1)
		execute = 0;
	if (execute == 1)
	{
		SetSideSensorUiLed(2);
		LL_mDelay(2000);
		SetSideSensorUiLed(0);
	}
	return execute;
}

void set_execute(int exec)
{
	execute = exec;
}

