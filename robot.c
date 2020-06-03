/* 
	robot.c - Main brain of the robot. Utilizes the I/O classes and manages pathfinding at a high level

    encoder wheel right     GPIO 3
    encoder wheel left      GPIO 4

    echo sensor side TRIG   GPIO 18 
    echo sensor side ECHO   GPIO 15 

    echo sensor front TRIG  GPIO 20  
    echo sensor front ECHO  GPIO 14  
    line sensor             GPIO 2
*/
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdbool.h>


#include "io_echo.h"
#include "io_line.h"
#include "io_motor.h"
#include "io_motorsensor.h"

static volatile bool keepRunning = true;

void exitHandler(int dummy) {
    keepRunning = false;
}

bool robot_init(){
	// Initialize PigPio
	if (gpioInitialise() < 0)
		return false;

	// Initialize all components
	if(!echo_init_all()) return false;
	if(!line_sensor_init()) return false;
	if(!motor_init_all()) return false;
	if(!sensor_init_all()) return false;

	// Start threads
	sensor_thread_init();
	echo_thread_init();

	// Add interrupt handler
	signal(SIGINT, exitHandler);

	return true;
}

void robot_cleanup(){
	
	printf("Attempting to stop threads...\n");
	sensor_thread_kill();
	echo_thread_kill();

   // Close out of pigpio
   gpioTerminate();
}

// Print redouts of all robot sensors
void robot_info(){
	// Print echo stats
	printf("Echo front: %f\n", echo_get_dist(1));
	printf("Echo side: %f\n", echo_get_dist(2));

	// Print wheel encoder stats
	printf("Wheel speed L: %f\n", sensor_get_dist(1));
	printf("Wheel speed R: %f\n", sensor_get_dist(2));

	// Print line sensor stats
	printf("Line sensor: %d\n", line_sensor_get_int());
}

// Rev engines for half a second to get the car moving
void revup(){
    motor_start(3);
    motor_start(4);

	motor_set_speed(3, 50);
	motor_set_speed(4, 50);

	usleep(1000000 * 0.05);
}

void line_follow(){
	bool sensor1state= line_sensor_get();
	int robo_speed = 40;
	int robo_speed2 = 0;
	
	// dark for too many times? Right turn
	// light for too many times? Left turn

	// On line
	if(sensor1state){
		motor_set_speed( 3, robo_speed);
		motor_set_speed( 4, robo_speed2);
	}
	
	// Offline (went left?)
	if(!sensor1state){
		motor_set_speed( 3, 0);
		motor_set_speed( 4, robo_speed);
	}
}

void robot_stop(){
	printf("Killing motors...\n");
	motor_set_speed(3, 0);
	motor_set_speed(4, 0);
	motor_stop(3);
	motor_stop(4);
}

int main(void) {
	robot_init();


	int iter = 0;
	int run_time = 10 * 4;

	//revup();

	while (keepRunning) {
		//line_follow();

		robot_info();

		iter += 1;
		usleep(10000);
		printf("Frame %d/%d\n", iter, run_time);

		//if(iter >= run_time) break;
	}

	//robot_stop();
	
	robot_cleanup();
}



void loop(){

  int distanceRight = 0;
  int distanceLeft = 0;
  sleep(5);

  if (distance <= 20){
    motor_set_speed(3, 0);
    motor_set_speed(4, 0);
    sleep(3);
    motor_set_speed(3, -50);
    motor_set_speed(4, -50);
    sleep(4);
    motor_set_speed(3, 0);
    motor_set_speed(4, 0);
    sleep(3);
    distanceRight = lookRight();
    sleep(3);
    distanceLeft = lookLeft();
    sleep(4);

    if (sensor_get_dist >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    motor_set_speed(3, 50);
    motor_set_speed(4, 50);
  }
    sensor_get_dist(1) = count_pulses();
}

int lookRight(){
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
  delay(100);
}



