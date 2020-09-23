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
int check_for_parts();
void find_wall();
void get_distance();
int max_hastighet; /* variabel för max hastighet på motorn */

POOL_T gyroSensor;
POOL_T sonicSensor;
int gyroValue0 = 0;
int gyroValue1 = 0;

int main(void)
{

    if (!brick_init())
        return (1); /*Instansierar klossen och tillåter remote-connection*/
    printf("*** ( EV3 ) Hello! ***\n");
    Sleep(2000);

    if (!check_for_parts())
        return 0;

    tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.75); // Sätter hastigheten på båda motorerna till 75% av maxhastigheten
                                                          /*
    tacho_run_forever(MOTOR_BOTH);
    Sleep(3000);
    tacho_stop(MOTOR_BOTH);
    */
    //find_wall();
    get_distance();
    brick_uninit();
    printf("dying...\n");
    return (0);
}

int check_for_parts()
{
    if (tacho_is_plugged((MOTOR_BOTH | MOTOR_ARM), TACHO_TYPE__NONE_) && sensor_is_plugged((SENSOR_GYRO | SENSOR_SONIC), SENSOR_TYPE__NONE_))
    {                                                       /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        max_hastighet = tacho_get_max_speed(MOTOR_LEFT, 0); /*Kollar max hastigheten som motorn kan ha*/
        gyroSensor = sensor_search(LEGO_EV3_GYRO);
        sonicSensor = sensor_search(LEGO_EV3_US);
        tacho_reset(MOTOR_BOTH);
        return 1;
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
        if (!sensor_is_plugged(SENSOR_GYRO, SENSOR_TYPE__NONE_))
        {
            printf("Stoppa in gyroskopet i port 3\n");
        }
        if (!sensor_is_plugged(SENSOR_SONIC, SENSOR_TYPE__NONE_))
        {
            printf("Stoppa in sonic sensorn i port 4\n");
        }
        brick_uninit();
        return (0); /* Stänger av sig om motorer ej är inkopplade */
    }
}

void find_wall()
{
    tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 0.25);
    tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * -0.25);

    float shortest_distance = 999999;
    float shortest_degree;
    gyroValue0 = sensor_get_value(0, gyroSensor, 0);
    gyroValue1 = sensor_get_value(1, gyroSensor, 0);
    while (gyroValue0 < 360)
    {
        gyroValue0 = sensor_get_value(0, gyroSensor, 0);
        gyroValue1 = sensor_get_value(1, gyroSensor, 0);
        printf("Gyro0: %d , Gyro1: %d \n", gyroValue0, gyroValue1);
        tacho_run_forever(MOTOR_LEFT);
        tacho_run_forever(MOTOR_RIGHT);
    }
    sleep(500);

    tacho_stop(MOTOR_BOTH);
}
void get_distance()
{
    while (sensor_get_value(0, sonicSensor, 0) > 10)
    {
        sensor_get_value(0, sonicSensor, 0);
        printf("Distance: %d\n", sensor_get_value(0, sonicSensor, 0));
    }
}
