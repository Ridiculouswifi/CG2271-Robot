#ifndef robotBrain
#define robotBrain

#include "MKL25Z4.h"
#include "queue.h"
#include "robotSerial.h"
#include "robotMotorControl.h"

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
		ledControl(GREEN);
		offLED();
		motorControl(STRAIGHT, 50); // Hard Coded for forward movement
		
	} else if (command == 0x02) {
		// Move Back
		ledControl(YELLOW);
		offLED();
		motorControl(REVERSE, 50);
		
	} else if (command == 0x08) {
		// Turn Left on the spot
		ledControl(PURPLE);
		motorControl(TURN_ANTICLOCKWISE, 90);
		
	} else if (command == 0x04) {
		// Turn Right on the spot
		ledControl(PURPLE);
		motorControl(1, 90);
		
	} else if (command == 0x09) {
		// Move Front and Turn Left
		ledControl(SKY);
		motorControl(FORWARD_LEFT_TURN, 90);
		
	} else if (command == 0x05) {
		// Move Front and Turn Right
		ledControl(SKY);
		motorControl(FORWARD_RIGHT_TURN, 50);
		
	} else if (command == 0x0a) {
		// Move Back and Turn Left
		ledControl(SKY);
		motorControl(BACKWARDS_LEFT_TURN, 50);
		
	} else if (command == 0x06) {
		// Move Back and Turn Right
		ledControl(SKY);
		motorControl(BACKWARDS_RIGHT_TURN, 50);
		
	} else if (command == 0x00) {
		// Stop
		ledControl(RED);
		onLED();
		motorControl(10, 0);
		
	} else {
		// Command is not recognised, discard command
		ledControl(BLUE);
		onLED();
		motorControl(10, 0);
	}
}

#endif
