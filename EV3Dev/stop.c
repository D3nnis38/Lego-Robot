#include <stdio.h>
#include "../ev3dev-c/source/ev3/brick.h"
#include <unistd.h>

#define Sleep(msec) usleep((msec)*1000) /*Definerar sleep där Sleep(1000)= 1 sekund*/

#define MOTOR_RIGHT OUTC
#define MOTOR_LEFT OUTB
#define MOTOR_ARM OUTA
#define SENSOR_GYRO IN3
#define SENSOR_SONIC IN4

#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT) /*Mask används för att kunna hänvisa till båda på samma gång*/

int main(void)
{
    if (!brick_init())
        return (1); /*Instansierar klossen och tillåter remote-connection*/
    printf("*** ( EV3 ) Kill! ***\n");
    tacho_stop(MOTOR_BOTH);
    brick_uninit();
    printf("dying...\n");
    return (0);
}