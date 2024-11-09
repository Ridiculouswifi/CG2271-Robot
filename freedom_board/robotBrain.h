#ifndef robotBrain
#define robotBrain

#include "MKL25Z4.h"
#include "queue.h"
#include "robotSerial.h"
#include "robotMotorControl.h"
#include "robotAudio.h"

#include "robotMacros.h"

#include "ledControl.h"

int speed = 80;
int musicStatus = 0;

/* Functions that decode data and execute actions */

/* Extracts one packet from the buffer */
uint8_t extractCommand(void) {
	if (Q_Empty(&rx_q)) {
		return 0xFF;
	}
	return Q_Dequeue(&rx_q);
}

/* Determines Music to Play */
void playMusic(void *argument) {
	for(;;) {
		if (musicStatus == 1) {
			osThreadFlagsSet(inProgress, IN_PROGRESS);
			osThreadFlagsClear(END);
		} else if (musicStatus == 2) {
			osThreadFlagsSet(ending, END);
			osThreadFlagsClear(IN_PROGRESS);
		} else {
			osThreadFlagsClear(IN_PROGRESS);
			osThreadFlagsClear(END);
			playTone(REST, 100);
		}
	}
}

/* Executes the action in the packet */
void handleCommand(int command) {
	
	if (command == 0x01) {
		// Move Front
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(GREEN);
		motorControl(STRAIGHT, 60); // Hard Coded for forward movement
		
	} else if (command == 0x02) {
		// Move Back
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(WHITE);
		motorControl(REVERSE, 60);
		
	} else if (command == 0x08) {
		// Turn Left on the spot
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(BLUE);
		motorControl(TURN_ANTICLOCKWISE, 75);
		
	} else if (command == 0x04) {
		// Turn Right on the spot
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(BLUE);
		motorControl(TURN_CLOCKWISE, 75);
		
	} else if (command == 0x09) {
		// Move Front and Turn Left
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(SKY);
		motorControl(FORWARD_LEFT_TURN, 60);
		
	} else if (command == 0x05) {
		// Move Front and Turn Right
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(SKY);
		motorControl(FORWARD_RIGHT_TURN, 60);
		
	} else if (command == 0x0a) {
		// Move Back and Turn Left
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(SKY);
		motorControl(BACKWARDS_LEFT_TURN, 60);
		
	} else if (command == 0x06) {
		// Move Back and Turn Right
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsClear(STATIONARY);
		
		ledControl(SKY);
		motorControl(BACKWARDS_RIGHT_TURN, 60);
		
	} else if (command == 0x00) {
		// Stop
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsClear(MOVING);
		
		ledControl(RED);
		motorControl(STOP, 0);
		
	} else if (command == 0x0F) {
		// No Processed Data, Stop
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsClear(MOVING);
		
		ledControl(PURPLE);
		motorControl(STOP, 0);
	
	} else if (command == 0xAA) {
		// Controller Issues
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsClear(MOVING);
		
		ledControl(OFF);
		motorControl(STOP, 0);
	
	} else if (command == 0x10) {
		// Play in progress song
		musicStatus = 1;
	
	} else if (command == 0x20) {
		// Play ending song
		musicStatus = 2;
	
	} else if (command == 0x30) {
		// High Speed
		speed = 80;
		
		musicStatus = 0;
	
	} else if (command == 0x40) {
		// Low Speed
		speed = 50;
		
		musicStatus = 0;
		
	} else {
		// Command is not recognised, discard command
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsClear(MOVING);
		
		ledControl(YELLOW);
		motorControl(STOP, 0);
	}
	
}

#endif
