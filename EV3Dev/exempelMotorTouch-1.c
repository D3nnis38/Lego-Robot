#include <stdio.h>
#include "../ev3dev-c/source/ev3/brick.h"
#include <unistd.h>

#define Sleep(msec) usleep((msec)*1000) /*Definerar sleep där Sleep(1000)= 1 sekund*/

#define MOTOR_RIGHT OUTA
#define MOTOR_LEFT OUTB
#define MOTOR_C OUTC
#define MOTOR_D OUTD
#define SENSOR_TOUCH IN1
#define SENSOR_2 IN2
#define SENSOR_3 IN3
#define SENSOR_4 IN4

#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT) /*Mask används för att kunna hänvisa till båda på samma gång*/

int max_hastighet; /* variabel för max hastighet på motorn */
POOL_T touchSensor;

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
		printf("Stoppa in vänstra motorn i port A,\n"
			   "Stoppa in högra motorn i port B.\n");
		brick_uninit();
		return (0); /* Stänger av sig om motorer ej är inkopplade */
	}

	touchSensor = sensor_search(LEGO_EV3_TOUCH); //Registrerar en touch sensor på touchSensor variabeln
	touch_set_mode_touch(touchSensor);			 //sätter vilket "mode" sensorn skall ha

	tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.75); // Sätter hastigheten på båda motorerna till 75% av maxhastigheten

	tacho_run_forever(MOTOR_BOTH);
	Sleep(3000);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
	tacho_stop(MOTOR_RIGHT);
	Sleep(2000);

	tacho_run_forever(MOTOR_BOTH);
	while (!sensor_get_value(0, touchSensor, 0))
		; //Så länge sensorn inte är intryckt kommer while loopen köras
	tacho_stop(MOTOR_BOTH);

	brick_uninit();
	printf("dying...\n");
	return (0);
}
