#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

gpio_sensor=24
GPIO.setup(gpio_sensor, GPIO.IN, pull_up_down = GPIO.PUD_UP)

delta=0.5

try:
	while 1:
		print(GPIO.input(gpio_sensor))
		time.sleep(delta)
	
except KeyboardInterrupt:
	GPIO.cleanup()
