#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>

#include "io_motor.h"

// GPIO presets for each motor
Motor m1 = {
	.gpio_enable = 17, 
	.gpio_fwd = 27, 
	.gpio_rev = 22
};

Motor m2 = {
	.gpio_enable = 25, 
	.gpio_fwd = 23, 
	.gpio_rev = 24
};

Motor m3 = {
	.gpio_enable = 10, 
	.gpio_fwd = 9, 
	.gpio_rev = 11
};

Motor m4 = {
	.gpio_enable = 12, 
	.gpio_fwd = 8, 
	.gpio_rev = 7
};

#define MOTOR_MAX 100

// Create a new motor struct and initialize it
bool motor_init(Motor *m){
	if(gpioSetMode(m->gpio_enable, PI_OUTPUT) < 0) return false;
	if(gpioSetMode(m->gpio_fwd, PI_OUTPUT) < 0) return false;
	if(gpioSetMode(m->gpio_rev, PI_OUTPUT) < 0) return false;

	if(gpioSetPWMrange(m->gpio_fwd, MOTOR_MAX) < 0) return false;
	if(gpioSetPWMrange(m->gpio_rev, MOTOR_MAX) < 0) return false;

	return true;
}

bool motor_init_all(){
	if(!motor_init(&m1)) return false;
	if(!motor_init(&m2)) return false;
	if(!motor_init(&m3)) return false;
	if(!motor_init(&m4)) return false;

	printf("Init: Motors ready, max range %d\n", MOTOR_MAX);

	return true;
}

Motor *get_motor(int motor){
	switch(motor){
		case 1: return &m1; break;
		case 2: return &m2; break;
		case 3: return &m3; break;
		case 4: return &m4; break;
	}

	return NULL;
}

// Set a motor's speed and direction, then start it
bool motor_start(int motor){
	Motor *m = get_motor(motor);
	if(m == NULL) return false;

	printf("Motor: Start motor %d\n", motor);

	gpioWrite(m->gpio_enable, 1);

	// set default speed to 0
	motor_set_speed(motor, 0);

	return true;
}

bool motor_stop(int motor){
	Motor *m =  get_motor(motor);
	if(m == NULL) return false;
	
	printf("Motor: Stop motor %d\n", motor);

	motor_set_speed(motor, 0);
	gpioWrite(m->gpio_enable, 0);

	return true;
}

bool motor_set_speed(int motor, int speed){
	Motor *m =  get_motor(motor);
	if(m == NULL) return false;

	printf("Motor: Set motor %d speed -> %d\n", motor, speed);

	// detect direction
	// negative % is backwards, positive is forwards
	bool reverse = speed < 0;
	speed = abs(speed);

	if(reverse){
		gpioPWM(m->gpio_fwd, 0);
		gpioPWM(m->gpio_rev, speed);
	}else{
		gpioPWM(m->gpio_fwd, speed);
		gpioPWM(m->gpio_rev, 0);
	}

	return true;
}
