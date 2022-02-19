 /**
 * @file mode.c
 * @author Mateus L.
 * @date 2021/4
 */

#include <Machine/mode.h>

void sm_loop(void)
{
	switch (select())
	{
	case STATE_1:
		set_ui_led(1);
		get_execute();
		set_execute(0);
		break;
	case STATE_2:
		set_ui_led(2);
		get_execute();
		set_execute(0);
		break;
	case STATE_3:
		set_ui_led(3);
		get_execute();
		set_execute(0);
		break;
	case STATE_4:
		set_ui_led(4);
		get_execute();
		set_execute(0);
		break;
	case STATE_5:
		set_ui_led(5);
		get_execute();
		set_execute(0);
		break;
	case STATE_6:
		set_ui_led(6);
		get_execute();
		set_execute(0);
		break;
	case STATE_7:
		set_ui_led(7);
		get_execute();
		set_execute(0);
		break;
	case STATE_8:
		set_ui_led(9);
		get_execute();
		set_execute(0);
		break;
	case STATE_10:
		set_ui_led(11);
		get_execute();
		set_execute(0);
		break;
	case STATE_12:
		set_ui_led(13);
		get_execute();
		set_execute(0);
		break;
	case STATE_14:
		set_ui_led(14);
		get_execute();
		set_execute(0);
		break;
	case STATE_15:
		set_ui_led(15);
		get_execute();
		set_execute(0);
		break;
	}
}
