#ifndef robotBrain
#define robotBrain

#include "MKL25Z4.h"
#include "queue.h"
#include "robotSerial.h"

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
void handleCommand(uint8_t command) {
	if (command == 0x01) {
		// Move Front
		ledControl(GREEN);
		offLED();
	} else if (command == 0x02) {
		// Move Back
		ledControl(RED);
	} else if (command == 0x08) {
		// Turn Left on the spot
	} else if (command == 0x04) {
		// Turn Right on the spot
	} else if (command == 0x09) {
		// Move Front and Turn Left
	} else if (command == 0x05) {
		// Move Front and Turn Right
	} else if (command == 0x0a) {
		// Move Back and Turn Left
	} else if (command == 0x06) {
		// Move Back and Turn Right
	} else if (command == 0x00) {
		// Stop
		ledControl(BLUE);
		onLED();
	} else {
		// Command is not recognised, discard command
		ledControl(RED);
	}
}

#endif
