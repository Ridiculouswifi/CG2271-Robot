#ifndef robotLED
#define robotLED

#include "MKL25Z4.h"
#include "delayFunction.h"
#include "cmsis_os2.h"

#include "robotMacros.h"

#define LED1_PIN 1   // PTA1
#define LED2_PIN 2   // PTA2
#define LED3_PIN 4   // PTD4
#define LED4_PIN 12  // PTA12
#define LED5_PIN 4   // PTA4
#define LED6_PIN 5   // PTA5
#define LED7_PIN 8   // PTC8
#define LED8_PIN 9   // PTC9
#define LED9_PIN 13  // PTA13
#define LED10_PIN 5  // PTD5
#define REAR_LED_PIN 0  // PTE30

void initFrontLEDs(void) {
	// Enable clocks for Port A, Port C, and Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;
	
	// Configure each pin as GPIO and set as output based on port
	PORTA->PCR[LED1_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED1_PIN] |= PORT_PCR_MUX(1);  // LED1: PTA1
	GPIOA->PDDR |= (1 << LED1_PIN);

	PORTA->PCR[LED2_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED2_PIN] |= PORT_PCR_MUX(1);  // LED2: PTA2
	GPIOA->PDDR |= (1 << LED2_PIN);

	PORTD->PCR[LED3_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED3_PIN] |= PORT_PCR_MUX(1);  // LED3: PTD4
	GPIOD->PDDR |= (1 << LED3_PIN);

	PORTA->PCR[LED4_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED4_PIN] |= PORT_PCR_MUX(1);  // LED4: PTA12
	GPIOA->PDDR |= (1 << LED4_PIN);

	PORTA->PCR[LED5_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED5_PIN] |= PORT_PCR_MUX(1);  // LED5: PTA4
	GPIOA->PDDR |= (1 << LED5_PIN);

	PORTA->PCR[LED6_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED6_PIN] |= PORT_PCR_MUX(1);  // LED6: PTA5
	GPIOA->PDDR |= (1 << LED6_PIN);

	PORTC->PCR[LED7_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED7_PIN] |= PORT_PCR_MUX(1);  // LED7: PTC8
	GPIOC->PDDR |= (1 << LED7_PIN);

	PORTC->PCR[LED8_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED8_PIN] |= PORT_PCR_MUX(1);  // LED8: PTC9
	GPIOC->PDDR |= (1 << LED8_PIN);

	PORTA->PCR[LED9_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED9_PIN] |= PORT_PCR_MUX(1);  // LED9: PTA13
	GPIOA->PDDR |= (1 << LED9_PIN);

	PORTD->PCR[LED10_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED10_PIN] |= PORT_PCR_MUX(1);  // LED10: PTD5
	GPIOD->PDDR |= (1 << LED10_PIN);
}

void initRearLEDs(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	PORTD->PCR[REAR_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[REAR_LED_PIN] = PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << REAR_LED_PIN);
}

void turnOnRearLED() {
	GPIOD->PSOR |= (1 << REAR_LED_PIN); 
}

void turnOffRearLED() {
	GPIOD->PCOR |= (1 << REAR_LED_PIN);
}

void turnOnFrontLED(int ledNumber) {
	switch (ledNumber) {
		case 1: GPIOA->PSOR |= (1 << LED1_PIN); break;
		case 2: GPIOA->PSOR |= (1 << LED2_PIN); break;
		case 3: GPIOD->PSOR |= (1 << LED3_PIN); break;
		case 4: GPIOA->PSOR |= (1 << LED4_PIN); break;
		case 5: GPIOA->PSOR |= (1 << LED5_PIN); break;
		case 6: GPIOA->PSOR |= (1 << LED6_PIN); break;
		case 7: GPIOC->PSOR |= (1 << LED7_PIN); break;
		case 8: GPIOC->PSOR |= (1 << LED8_PIN); break;
		case 9: GPIOA->PSOR |= (1 << LED9_PIN); break;
		case 10: GPIOD->PSOR |= (1 << LED10_PIN); break;
		default: break;
    }
}

void turnOffFrontLED(int ledNumber) {
	switch (ledNumber) {
		case 1: GPIOA->PCOR |= (1 << LED1_PIN); break;
		case 2: GPIOA->PCOR |= (1 << LED2_PIN); break;
		case 3: GPIOD->PCOR |= (1 << LED3_PIN); break;
		case 4: GPIOA->PCOR |= (1 << LED4_PIN); break;
		case 5: GPIOA->PCOR |= (1 << LED5_PIN); break;
		case 6: GPIOA->PCOR |= (1 << LED6_PIN); break;
		case 7: GPIOC->PCOR |= (1 << LED7_PIN); break;
		case 8: GPIOC->PCOR |= (1 << LED8_PIN); break;
		case 9: GPIOA->PCOR |= (1 << LED9_PIN); break;
		case 10: GPIOD->PCOR |= (1 << LED10_PIN); break;
		default: break;
    }
}

void turnOffAllFrontLEDs() {
	for (int i = 1; i <= 10; i++) {
		turnOffFrontLED(i);
	}
}

void turnOnAllFrontLEDs() {
	for (int i = 1; i <= 10; i++) {
		turnOnFrontLED(i);
	}
}

void runFrontLEDs(void *argument) {
	for (;;) {
		for (int i = 1; i <= 10; i++) {
			osThreadFlagsWait(MOVING, osFlagsWaitAny, osWaitForever);

			turnOnFrontLED(i);  
			osDelay(300);
			turnOffFrontLED(i);
		}
	}
}

void stopFrontLEDs(void *argument) {
	for (;;) {
		osThreadFlagsWait(STATIONARY, osFlagsWaitAny, osWaitForever);

		turnOnAllFrontLEDs();
	}
}

void flashRearLEDsStop(void *argument) {
	for (;;) {
		osThreadFlagsWait(STATIONARY, osFlagsWaitAny, osWaitForever);

		turnOnRearLED();
		osDelay(250);
		turnOffRearLED();
		osDelay(250);
	}
}

void flashRearLEDsRun(void *argument) {
	for (;;) {
		osThreadFlagsWait(MOVING, osFlagsWaitAny, osWaitForever);

		turnOnRearLED();
		osDelay(500);
		turnOffRearLED();
		osDelay(500);
	}
}

#endif
