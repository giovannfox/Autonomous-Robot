#pragma once

#include <stdbool.h>

/*
 * Motor struct
 *
 * contains GPIO values for motors.
 * allows for functions which take a 'motor'
 * as a parameter.
 */
typedef struct{
	int gpio_enable;
	int gpio_fwd;
	int gpio_rev;
} Motor;

/*
 * Initialize all motors and their GPIOs
 */
bool motor_init_all();

/*
 * Turn a motor on. 
 * By default, the speed is set to 0%
 *
 * param - ID of target motor (1-4)
 */
bool motor_start(int motor);

/*
 * Turn a motor off
 *
 * params
 * motor - ID of target motor (1-4)
 */
bool motor_stop(int motor);

/*
 * Set the speed of the motor in it's current direction
 *
 * params
 * motor - ID of target motor (1-4)
 * speed - value from -100% - 100%
 * 
 * note: if a negative percent is given, motor will go backwards
 */
bool motor_set_speed(int motor, int speed);
