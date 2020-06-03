#pragma once

#include <stdbool.h>
#include <pthread.h>

typedef struct{
	int gpio;
	int pulse;
	float speed;
	pthread_t thread;
} Sensor;

// sensor funcs
bool sensor_init_all();
float sensor_get_dist(int sensor);
void sensor_thread_init();
void sensor_thread_kill();
