#include "pathfind.h"

/* 
	pathfind.c - Utilizes I/O sensors to find a path across the tape 

	ROBOT STATES
	- follow (default):
		follow the line. there are no obstructions

	- block:
		cause: object being too close to front sensor
		behavior: an object is blocking the pathway. Wait max 
		of 5 seconds, then move on to move around. 
		Otherwise, return back to following.

	- move_around:
		cause: object did not move during block state
		behavior: circling around the blocking object, searching
		for the line again. If the line is not found after
		20 seconds, stop.

	- ramp:
		cause: encoder wheels are going slower than the given power
		behavior: increase speed until encoder wheels begin to relate 
		to power more
*/
