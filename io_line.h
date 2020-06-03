#pragma once

#include <stdbool.h>

/*
 * Initialize GPIO for line sensor hardware 
 */
bool line_sensor_init();

/* return the status of the line sensor
 *
 * true = sensor is detecting a line 
 * false = sensor is not detecting a line 
*/
bool line_sensor_get();
int line_sensor_get_int();
