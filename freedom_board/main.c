/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

#include "MKL25Z4.h"

#include "delayFunction.h"
#include "robotLED.h"
#include "robotMotorControl.h"
#include "robotAudio.h"
#include "robotBrain.h"
#include "robotSerial.h"

#include "ledControl.h"

void interfaceCommand(void *argument) {
	for(;;) {
		uint8_t command = DATA;
		handleCommand(command);
	}
}

int main(void) {
	// Initialise peripherals etc
	SystemCoreClockUpdate();
	initLED();
	initUART2(BAUD_RATE);
	
	// Initialise OS
	//osKernelInitialize();
	//osThreadNew(interfaceCommand, NULL, NULL);
	//osKernelStart();
	
	while(1) {
		handleCommand(getData());
	}
}
