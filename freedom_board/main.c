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

#include "ledControl.h"

void interfaceCommand(void *argument) {
	for(;;) {
		//motorControl(STRAIGHT, 70);
		int command = getData();
		handleCommand(command);
	}
}

int main(void) {
	// Initialise peripherals etc
	SystemCoreClockUpdate();
	//initLEDTest();
	initUART2(BAUD_RATE);
	initMotor();
	//initPWM();
	//initFrontLEDs();
	//initRearLEDs();
	
	// Initialise OS
	osKernelInitialize();
	osThreadNew(interfaceCommand, NULL, NULL);
	//osThreadNew(runFrontLEDs, NULL, NULL);
	//osThreadNew(flashRearLEDs, NULL, NULL);
	osKernelStart();
	
	for(;;) {
	}
}
