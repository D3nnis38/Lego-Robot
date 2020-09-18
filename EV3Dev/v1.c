#include <stdio.h>
#include "../ev3dev-c/source/ev3/brick.h"
#include <unistd.h>

#define Sleep(msec) usleep((msec)*1000) /*Definerar sleep där Sleep(1000)= 1 sekund*/

#define MOTOR_RIGHT OUTC
#define MOTOR_LEFT OUTB
#define MOTOR_ARM OUTA
#define SENSOR_4 IN4

#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT) /*Mask används för att kunna hänvisa till båda på samma gång*/

int max_hastighet; /* variabel för max hastighet på motorn */

int main(void)
{

	if (!brick_init())
		return (1); /*Instansierar klossen och tillåter remote-connection*/
	printf("*** ( EV3 ) Hello! ***\n");
	Sleep(2000);

	if (tacho_is_plugged(MOTOR_BOTH, TACHO_TYPE__NONE_))
	{														/* TACHO_TYPE__NONE_ = Alla typer av motorer */
		max_hastighet = tacho_get_max_speed(MOTOR_LEFT, 0); /*Kollar max hastigheten som motorn kan ha*/
		tacho_reset(MOTOR_BOTH);
	}
	else
	{
		if (!tacho_is_plugged(MOTOR_LEFT, TACHO_TYPE__NONE_))
		{
			printf("Stoppa in vänstra motorn i port B\n");
		}
		if (!tacho_is_plugged(MOTOR_RIGHT, TACHO_TYPE__NONE_))
		{
			printf("Stoppa in högra motorn i port C\n");
		}
		if (!tacho_is_plugged(MOTOR_ARM, TACHO_TYPE__NONE_))
		{
			printf("Stoppa in arm-motorn i port A\n");
		}
		brick_uninit();
		return (0); /* Stänger av sig om motorer ej är inkopplade */
	}

	tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.75); // Sätter hastigheten på båda motorerna till 75% av maxhastigheten

	tacho_run_forever(MOTOR_BOTH);
	Sleep(3000);
	tacho_stop(MOTOR_BOTH);

	brick_uninit();
	printf("dying...\n");
	return (0);
}
