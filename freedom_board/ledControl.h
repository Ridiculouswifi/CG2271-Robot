#ifndef LED_CONTROL
#define LED_CONTROL

#include "MKL25Z4.h"

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))// Device header

#define EXTERNAL_LED 20

enum colour_t {
	RED, GREEN, BLUE, OFF
};

// Resets LEDs to display no light
void noLight(void) {
	GPIOD_PDOR |= MASK(BLUE_LED);
	GPIOB_PDOR |= (MASK(RED_LED) | MASK(GREEN_LED));
}

void onRed(void) {
	GPIOB_PDOR &= ~MASK(RED_LED);
}

void onBlue(void) {
	GPIOD_PDOR &= ~MASK(BLUE_LED);
}

void onGreen(void) {
	GPIOB_PDOR &= ~MASK(GREEN_LED);
}

void onLED(void) {
	GPIOE_PDOR |= MASK(EXTERNAL_LED);
}

void offLED(void) {
	GPIOE_PDOR &= ~MASK(EXTERNAL_LED);
}

void ledControl(enum colour_t colour) {
	if (colour == OFF) {
		noLight();
	} else if (colour == RED) {
		noLight();
		onRed();
	} else if (colour == BLUE) {
		noLight();
		onBlue();
	} else if (colour == GREEN) {
		noLight();
		onGreen();
	}
}

void initLED() {
	// Enable Clock Gating for PORTD and PORTB
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK);
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
	
	// Reset light
	noLight();
	
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE->PCR[EXTERNAL_LED] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[EXTERNAL_LED] |= PORT_PCR_MUX(1);
	
	GPIOE->PDDR |= (1 << EXTERNAL_LED);
	
	GPIOE->PDOR &= ~(1 << EXTERNAL_LED);
}

#endif
