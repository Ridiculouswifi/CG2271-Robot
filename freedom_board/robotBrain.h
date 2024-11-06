#ifndef robotBrain
#define robotBrain

#include "MKL25Z4.h"
#include "queue.h"
#include "robotSerial.h"
#include "robotMotorControl.h"

#include "robotMacros.h"

#include "ledControl.h"

/* Functions that decode data and execute actions */

/* Extracts one packet from the buffer */
uint8_t extractCommand(void) {
	if (Q_Empty(&rx_q)) {
		return 0xFF;
	}
	return Q_Dequeue(&rx_q);
}

/* Executes the action in the packet */
void handleCommand(int command) {
	
	if (command == 0x01) {
		// Move Front
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, MOVING);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(GREEN);
		motorControl(STRAIGHT, 70); // Hard Coded for forward movement
		
	} else if (command == 0x02) {
		// Move Back
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(WHITE);
		motorControl(REVERSE, 70);
		
	} else if (command == 0x08) {
		// Turn Left on the spot
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(BLUE);
		motorControl(TURN_ANTICLOCKWISE, 85);
		
	} else if (command == 0x04) {
		// Turn Right on the spot
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(BLUE);
		motorControl(TURN_CLOCKWISE, 85);
		
	} else if (command == 0x09) {
		// Move Front and Turn Left
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(SKY);
		motorControl(FORWARD_LEFT_TURN, 60);
		
	} else if (command == 0x05) {
		// Move Front and Turn Right
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(SKY);
		motorControl(FORWARD_RIGHT_TURN, 60);
		
	} else if (command == 0x0a) {
		// Move Back and Turn Left
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(SKY);
		motorControl(BACKWARDS_LEFT_TURN, 60);
		
	} else if (command == 0x06) {
		// Move Back and Turn Right
		osThreadFlagsSet(frontRun, MOVING);
		osThreadFlagsSet(rearRun, MOVING);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(STATIONARY);
		osThreadFlagsClear(END);
		
		ledControl(SKY);
		motorControl(BACKWARDS_RIGHT_TURN, 60);
		
	} else if (command == 0x00) {
		// Stop
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(MOVING);
		osThreadFlagsClear(END);
		
		ledControl(RED);
		motorControl(STOP, 0);
		
	} else if (command == 0x0F) {
		// No Processed Data, Stop
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(MOVING);
		osThreadFlagsClear(END);
		
		ledControl(PURPLE);
		motorControl(STOP, 0);
	
	} else if (command == 0xAA) {
		// Controller Issues
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(MOVING);
		osThreadFlagsClear(END);
		
		ledControl(OFF);
		motorControl(STOP, 0);
	
	} else {
		// Command is not recognised, discard command
		osThreadFlagsSet(frontStop, STATIONARY);
		osThreadFlagsSet(rearStop, STATIONARY);
		osThreadFlagsSet(movingAudio, IN_PROGRESS);
		osThreadFlagsClear(MOVING);
		osThreadFlagsClear(END);
		
		ledControl(YELLOW);
		motorControl(STOP, 0);
	}
	
}

#endif
