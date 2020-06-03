#include <pigpio.h>

#include "io_line.h"

int gpio_line = 2;

bool line_sensor_init(){
	if(gpioSetMode(gpio_line, PI_INPUT) < 0) return false;
	if(gpioSetPullUpDown(gpio_line, PI_PUD_UP) < 0) return false;
	return true;
}

bool line_sensor_get(){
    return gpioRead(gpio_line) ? true : false;
}

int line_sensor_get_int(){
    return gpioRead(gpio_line);
}
