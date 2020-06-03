#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "io_motorsensor.h"

/// GPIO presets for each motor sensor
Sensor s1 = {.gpio = 5};
Sensor s2 = {.gpio = 3};

bool sensor_thread_running = false;

Sensor *sensor_get(int sensor){
	switch(sensor){
		case 1: return &s1; break;
		case 2: return &s2; break;
		case 3: return &s1; break;
		case 4: return &s2; break;
	}
	return NULL;
}

void count_pulses(int gpio, int level, uint32_t tick){
	if(level != 0) return;

	if(gpio == s1.gpio) s1.pulse++;
	if(gpio == s2.gpio) s2.pulse++;
}

bool sensor_init(Sensor *s){
	if(gpioSetMode(s->gpio, PI_INPUT) < 0) return false;
	if(gpioSetPullUpDown(s->gpio, PI_PUD_UP) < 0) return false;
	if(gpioSetAlertFunc(s->gpio, count_pulses) < 0) return false;

	return true;
}

bool sensor_init_all(){
	if(!sensor_init(&s1)) return false;
	if(!sensor_init(&s2)) return false;
	return true;
}

void *sensor_thread(void *s){
	Sensor *sensor = (Sensor *)s;
	int new_count, last_count = 0;
	while(sensor_thread_running){
		usleep(100000);
		new_count = sensor->pulse;
		sensor->speed = (new_count - last_count) * 1.0 * 5.175;
		last_count = new_count;
	}
}

void sensor_thread_init(){
	sensor_thread_running = true;

	pthread_create(&s1.thread, NULL, sensor_thread, &s1);
	pthread_create(&s2.thread, NULL, sensor_thread, &s2);
}

void sensor_thread_kill(){
	sensor_thread_running = false;

	printf("Killing sensor thread...\n");

	pthread_join(s1.thread, NULL);
	pthread_join(s2.thread, NULL);

	printf("Sensor thread killed\n");
}

float sensor_get_dist(int sensor){
	Sensor *s = sensor_get(sensor);
	if(s == NULL) return -1;
	return s->speed;
}
