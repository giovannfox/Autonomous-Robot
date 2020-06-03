#pragma once
#include <pthread.h>
#include <stdbool.h>

typedef struct{
	int gpio_echo;
	int gpio_trig;
    double distance;

    pthread_t thread; 
} Echo;

/*
 * Initialize GPIO for all echo sensors
 */
bool echo_init_all();

/*
 * Start data-grabbing threads for all echo sensors
 */
void echo_thread_init();

/*
 * 
 */
void echo_thread_kill();

/*
 * Return current distance value for an echo sensor
 */
float echo_get_dist(int echo);
