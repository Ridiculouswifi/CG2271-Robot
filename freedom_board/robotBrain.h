#ifndef robotBrain
#define robotBrain

#include "MKL25Z4.h"
#include "queue.h"
#include "robotSerial.h"

/* Functions that decode data and execute actions */

/* Extracts one packet from the buffer */
uint8_t extractCommand(void) {
	if (Q_Empty(&rx_q)) {
		return 0;
	}
	return Q_Dequeue(&rx_q);
}

/* Executes the action in the packet */
void handleCommand(uint8_t command) {
	if (command == 0x01) {
		// Move Front
	} else if (command == 0x02) {
		// Move Back
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
	}
	// Command is not recognised, discard command
}

#endif
