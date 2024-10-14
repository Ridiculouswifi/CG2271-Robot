#ifndef robotAudio
#define robotAudio

#include "MKL25Z4.h"  // Include device-specific header for KL25Z

#define PWM_FREQUENCY 48000 // Assuming the core clock is running at 48 MHz

// Frequency for different musical notes (in Hz)
#define NOTE_C 262
#define NOTE_D 294
#define NOTE_E 330
#define NOTE_F 349
#define NOTE_G 392
#define NOTE_A 440
#define NOTE_B 494

void initPWM(void) {
    // Enable clock for TPM0 and Port C (assuming the buzzer is connected to a pin on Port C)
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // Enable clock for TPM0
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // Enable clock for Port C

    // Configure the port pin for PWM (e.g., PTC1 for TPM0_CH0)
    PORTC->PCR[1] = PORT_PCR_MUX(4);  // Set PTC1 to TPM0_CH0 (generate pwm)

    // Set TPM clock source
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  

    TPM0->SC = TPM_SC_PS(0);  // Prescaler divide by 1
    TPM0->SC |= TPM_SC_CMOD(1);  // Enable TPM counter

    // Set the PWM mode for TPM0 channel 0
    TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Edge-aligned PWM, high-true pulses
}

// Function to play a note at a specified frequency (in Hz)
void playNote(uint16_t frequency) {
    // Calculate the MOD value to generate the desired frequency
    TPM0->MOD = (PWM_FREQUENCY / frequency) - 1;

    // Set the duty cycle to 50% (or adjust as necessary)
    TPM0->CONTROLS[0].CnV = TPM0->MOD / 2;
}

// Function to stop playing the note (turn off the sound)
void stopNote(void) {
    TPM0->CONTROLS[0].CnV = 0;  // Set duty cycle to 0 (no sound)
}


#endif
