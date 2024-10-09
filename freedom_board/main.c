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

void interfaceCommand(void *argument) {
	for(;;) {
		handleCommand(extractCommand());
	}
}

int main(void) {
	// Initialise peripherals etc
	SystemCoreClockUpdate();
	
	// Initialise OS
	osKernelInitialize();
	osThreadNew(interfaceCommand, NULL, NULL);
	osKernelStart();
	
	for(;;) {}
}
