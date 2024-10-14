#ifndef robotLED
#define robotLED

#include "MKL25Z4.h"

void initLED(void) {
    // Enable clock for both Port B and Port D
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  // Port B for Red and Green LEDs
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;  // Port D for Blue LED

    // Configure the pins as GPIO (Pin Control Register)
    PORTB->PCR[18] = PORT_PCR_MUX(1); // PTB18 as GPIO (Red LED)
    PORTB->PCR[19] = PORT_PCR_MUX(1); // PTB19 as GPIO (Green LED)
    PORTD->PCR[1] = PORT_PCR_MUX(1);  // PTD1 as GPIO (Blue LED)

    // Set the pins as output
    GPIOB->PDDR |= (1 << 18) | (1 << 19);  // Set PTB18, PTB19 as outputs (Red, Green)
    GPIOD->PDDR |= (1 << 1);               // Set PTD1 as output (Blue)
}

// Usage: setLedColor(1, 0, 0) sets only red LED on. setLedColor(1, 0, 1) sets both red and blue led on
void setLEDColor(uint8_t red, uint8_t green, uint8_t blue) {
    if (red) {
        GPIOB->PCOR = (1 << 18);  // Turn on Red LED (active low)
    } else {
        GPIOB->PSOR = (1 << 18);  // Turn off Red LED
    }
    
    // Control Green LED
    if (green) {
        GPIOB->PCOR = (1 << 19);  // Turn on Green LED (active low)
    } else {
        GPIOB->PSOR = (1 << 19);  // Turn off Green LED
    }

    // Control Blue LED
    if (blue) {
        GPIOD->PCOR = (1 << 1);  // Turn on Blue LED (active low)
    } else {
        GPIOD->PSOR = (1 << 1);  // Turn off Blue LED
    }
}


#endif
