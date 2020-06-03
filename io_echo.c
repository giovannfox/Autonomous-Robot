#include "io_echo.h"

#include <inttypes.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

// FRONT
Echo e1 = {
	.gpio_echo = 20,
	.gpio_trig = 21
};

// SIDE
Echo e2 = {
	.gpio_echo = 15,
	.gpio_trig = 18
};

bool echo_thread_running = false;

Echo *get_echo(int echo){
	switch(echo){
		case 1: return &e1; break;
		case 2: return &e2; break;
	}

	return NULL;
}

bool echo_init(Echo *e){
	if(gpioSetMode(e->gpio_trig, PI_OUTPUT) < 0) return false;
	if(gpioSetMode(e->gpio_echo, PI_INPUT) < 0) return false;
	if(gpioWrite(e->gpio_trig, 0) < 0) return false;

	return true;
}

bool echo_init_all(){
	if(!echo_init(&e1)) return false;
	//if(!echo_init(&e2)) return false;
	return true;
}

float echo_get_dist(int echo){
	Echo *e = get_echo(echo);
	if(e == NULL) return 0.0;
	return e->distance;
}

void *echo_thread(void *e){
	struct timespec start, end;

	// Convert void * param into an Echo type
	Echo *echo = (Echo *)e;

	while(echo_thread_running){
		// Send pulse
		gpioWrite(echo->gpio_trig, 1);
		sleep(0.0001);
		gpioWrite(echo->gpio_trig, 0);

		// Wait to recieve pulse
		// Record times between receive pulse
		while (gpioRead(echo->gpio_echo) == 0) {
			clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		}
		while (gpioRead(echo->gpio_echo) == 1) {
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		}

		// Calculate distance
		uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
		echo->distance = (delta_us * 17150.0)/1000000.0;

		sleep(1);
	}
}

void echo_thread_init(){
	echo_thread_running = true;

	pthread_create(&e1.thread, NULL, echo_thread, &e1);
	//pthread_create(&e2.thread, NULL, echo_thread, &e2);
}

void echo_thread_kill(){
	echo_thread_running = false;

	printf("Killing echo thread...\n");

	pthread_join(e1.thread, NULL);
	//pthread_join(e2.thread, NULL);

	printf("Echo threads killed\n");
}
