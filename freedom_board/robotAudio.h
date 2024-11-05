#ifndef robotAudio
#define robotAudio

#include "MKL25Z4.h"

#include "cmsis_os2.h"

// Define PWM frequency and buzzer pin
#define TPM_CLOCK 20971520  // TPM clock frequency (20.97152 MHz)
#define BUZZER_PIN 1        // PTD1 (TPM0_CH1) for buzzer output

// Define frequencies for each musical note (in Hz)
#define NOTE_D5  587
#define NOTE_A4  440
#define NOTE_A5  880
#define NOTE_G5  784
#define NOTE_FS5 740
#define NOTE_E5  659
#define NOTE_CS5 554
#define NOTE_B5  988
#define REST     0

// Define delays
#define ONE 100
#define TWO 200
#define THREE 300
#define FOUR 400
#define FIVE 500
#define SIX 600
#define SEVEN 700
#define EIGHT 800
#define NINE 900
#define TEN 1000

// Function to initialize the PWM on PTD1
void initPWM(void) {
    // Enable clock for Port D and TPM0
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

    // Set PTD1 to TPM0_CH1 (Alternative 4 function)
    PORTD->PCR[BUZZER_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BUZZER_PIN] |= PORT_PCR_MUX(4);

    // Set TPM clock source to MCGFLLCLK (20.97152 MHz)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // Set TPM0 prescaler to divide by 1 and enable counter
    TPM0->SC = TPM_SC_PS(0) | TPM_SC_CMOD(1);

    // Configure TPM0 Channel 1 for edge-aligned PWM, high-true pulses
    TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
}

// Function to play a tone at a given frequency and duration
void playTone(uint16_t frequency, uint32_t duration) {
    if (frequency == REST) {
        TPM0->CONTROLS[1].CnV = 0;  // Stop PWM output for rest
    } else {
        uint32_t mod = (TPM_CLOCK / frequency) - 1;
        TPM0->MOD = mod;                     // Set MOD value
        TPM0->CONTROLS[1].CnV = mod / 2;     // Set duty cycle to 50%
    }

	osDelay(duration); 			// Delay to maintain tone for the specified duration
    TPM0->CONTROLS[1].CnV = 0;  // Stop PWM after duration
}

// Function to play the celebration melody
void celebrate(void *argument) {
	for(;;) {
		playTone(NOTE_D5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_A5, 275);
		osDelay(200);
		playTone(NOTE_G5, 275);
		osDelay(200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_CS5, 350);
		osDelay(1000);

		playTone(NOTE_CS5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_FS5, 275);
		osDelay(200);
		playTone(NOTE_E5, 275);
		osDelay(200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_FS5, 350);
		osDelay(1000);
		

		playTone(NOTE_D5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_A5, 275);
		osDelay(200);
		playTone(NOTE_G5, 275);
		osDelay(200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_CS5, 350);
		osDelay(1000);

		playTone(NOTE_CS5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_FS5, 275);
		osDelay(200);
		playTone(NOTE_E5, 275);
		osDelay(200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_FS5, 350);
		osDelay(1000);

		playTone(NOTE_FS5, 500);
		osDelay(300);
		playTone(NOTE_A5, 500);
		osDelay(300);
		playTone(NOTE_G5, 200);
		playTone(NOTE_A5, 200);
		playTone(NOTE_G5, 200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 500);
		osDelay(400);
		playTone(NOTE_CS5, 500);
		osDelay(300);
		playTone(NOTE_E5, 500);
		osDelay(300);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_G5, 200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_D5, 500);
		osDelay(400);
		playTone(NOTE_FS5, 500);
		osDelay(300);
		playTone(NOTE_A5, 500);
		osDelay(300);
		playTone(NOTE_G5, 200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_G5, 200);
		playTone(NOTE_A5, 200);
		playTone(NOTE_B5, 500);
		osDelay(400);
		playTone(NOTE_A5, 350);
		playTone(NOTE_G5, 200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_G5, 500);
		osDelay(300);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_G5, 200);
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_D5, 500);

		TPM0->CONTROLS[1].CnV = 0;  // Ensure the buzzer is off after the melody
	}
}

#endif
