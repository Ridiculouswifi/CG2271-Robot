#ifndef robotAudio
#define robotAudio

#include "MKL25Z4.h"
#include "cmsis_os2.h"
#include "robotMacros.h"

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
#define NOTE_B4	 494
#define NOTE_FS4 370
#define NOTE_E4  330
#define NOTE_GS5 831
#define NOTE_C5  523
#define NOTE_D4  294
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

#define eightth 185
#define quarter 350
#define stop 20
// Notes and Delays for Music
int celebrateNotes[44] = {NOTE_D5, NOTE_A4, NOTE_D5, NOTE_A5, REST, NOTE_G5, REST, NOTE_FS5, NOTE_E5, NOTE_CS5, REST,
			NOTE_CS5, NOTE_A4, NOTE_CS5, NOTE_FS5, REST, NOTE_E5, REST, NOTE_CS5, NOTE_D5, NOTE_FS5, REST,
			NOTE_D5, NOTE_A4, NOTE_D5, NOTE_A5, REST, NOTE_G5, REST, NOTE_FS5, NOTE_E5, NOTE_CS5, REST,
			NOTE_CS5, NOTE_A4, NOTE_CS5, NOTE_FS5, REST, NOTE_E5, REST, NOTE_CS5, NOTE_D5, NOTE_FS5, REST};
int celebrateDelays[44] = {200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
			200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
			200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
			200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000};
int takeOnMeNotes[32] = {NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, REST, NOTE_B4, REST, NOTE_E5,
			REST, NOTE_E5, REST, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5,
			NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, REST, NOTE_D5, REST, NOTE_FS5,
			REST, NOTE_FS5, REST, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5};
int takeOnMeDelays[32] = {eightth, eightth, eightth, quarter, stop, quarter, stop, quarter,
			stop, quarter, stop, eightth, eightth, eightth, eightth, eightth,
			eightth, eightth, eightth, quarter, stop, quarter, stop, quarter,
			stop, quarter, stop, eightth, eightth, eightth, eightth, eightth};

// Function to initialize the PWM on PTD1
void initPWM(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;        // Enable clock for Port D
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;         // Enable clock for TPM0

    PORTD->PCR[BUZZER_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BUZZER_PIN] |= PORT_PCR_MUX(4); // Set PTD1 to TPM0_CH1

	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);         // Set TPM clock source to MCGFLLCLK (20.97152 MHz)

	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
    TPM0->SC |= TPM_SC_PS(3);  					// Set prescaler to divide by 4
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM0_C1SC |= TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);  // Configure TPM0 Channel 1 for edge-aligned PWM
	
	TPM0_C1V = 0;
	
	TPM0->SC |= TPM_SC_CMOD(1);					// Start Counter
}

// Function to play a tone at a given frequency and duration
void playTone(uint16_t frequency, uint32_t duration) {
    if (frequency == REST) {
        TPM0_C1V = 0;  // Stop PWM output for rest
    } else {
        uint32_t mod = (TPM_CLOCK / frequency / 4) - 1;
        TPM0_MOD = mod;                     // Set MOD value
        TPM0_C1V = mod / 2;     // Set duty cycle to 50%
    }

    osDelay(duration); 	// Delay to maintain tone for the specified duration
}

// Function to play the celebration melody
void celebrate(void *argument) {
    for (;;) {
		for (int i = 0; i < 44; i++) {
			osThreadFlagsWait(END, osFlagsWaitAny, osWaitForever);
			
			playTone(celebrateNotes[i], celebrateDelays[i]);
			playTone(REST, 3);
		}
	}
	
	
	/*
	for(;;) {
		osThreadFlagsWait(END, osFlagsWaitAny, osWaitForever);
		
		playTone(NOTE_D5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_A5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_G5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_CS5, 350);
		playTone(REST, 1000); // REST

		osThreadFlagsWait(END, osFlagsWaitAny, osWaitForever);
		
		playTone(NOTE_CS5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_FS5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_E5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_CS5, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_FS5, 350);
		playTone(REST, 1000); // REST
		
		int celebrateDelays[44] = {200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
				200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
				200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000,
				200, 200, 200, 275, 200, 275, 200, 200, 200, 350, 1000};

		osThreadFlagsWait(END, osFlagsWaitAny, osWaitForever);
		
		playTone(NOTE_D5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_A5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_G5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_FS5, 200);
		playTone(NOTE_E5, 200);
		playTone(NOTE_CS5, 350);
		playTone(REST, 1000); // REST

		osThreadFlagsWait(END, osFlagsWaitAny, osWaitForever);
		
		playTone(NOTE_CS5, 200);
		playTone(NOTE_A4, 200);
		playTone(NOTE_CS5, 200);
		playTone(NOTE_FS5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_E5, 275);
		playTone(REST, 200); // REST
		playTone(NOTE_CS5, 200);
		playTone(NOTE_D5, 200);
		playTone(NOTE_FS5, 350);
		playTone(REST, 1000); // REST
	}
    //TPM0->CONTROLS[1].CnV = 0;  // Ensure the buzzer is off after the melody
	*/
}

// Function to play the Take on Me melody
void takeOnMe(void *argument) {
	for(;;) {
		for (int i = 0; i < 32; i++) {
			osThreadFlagsWait(IN_PROGRESS, osFlagsWaitAny, osWaitForever);
			
			playTone(takeOnMeNotes[i], takeOnMeDelays[i]);
			playTone(REST, 15);
		}
	}
	
	
	/*
    for (;;) {
		osThreadFlagsWait(IN_PROGRESS, osFlagsWaitAny, osWaitForever);
		
        playTone(NOTE_FS5, TWO);
        playTone(NOTE_FS5, TWO);
        playTone(NOTE_D5, TWO);
        playTone(NOTE_B4, TWO);
        playTone(REST, TWO);
        playTone(NOTE_B4, TWO);
        playTone(REST, TWO);
        playTone(NOTE_E5, TWO);
        
		osThreadFlagsWait(IN_PROGRESS, osFlagsWaitAny, osWaitForever);
		
        playTone(REST, TWO);
        playTone(NOTE_E5, TWO);
        playTone(REST, TWO);
        playTone(NOTE_E5, TWO);
        playTone(NOTE_GS5, TWO);
        playTone(NOTE_GS5, TWO);
        playTone(NOTE_A5, TWO);
        playTone(NOTE_B5, TWO);
        
		osThreadFlagsWait(IN_PROGRESS, osFlagsWaitAny, osWaitForever);
		
        playTone(NOTE_A5, TWO);
        playTone(NOTE_A5, TWO);
        playTone(NOTE_A5, TWO);
        playTone(NOTE_E5, TWO);
        playTone(REST, TWO);
        playTone(NOTE_D5, TWO);
        playTone(REST, TWO);
        playTone(NOTE_FS5, TWO);
        
		osThreadFlagsWait(IN_PROGRESS, osFlagsWaitAny, osWaitForever);
		
        playTone(REST, TWO);
        playTone(NOTE_FS5, TWO);
        playTone(REST, TWO);
        playTone(NOTE_FS5, TWO);
        playTone(NOTE_E5, TWO);
        playTone(NOTE_E5, TWO);
        playTone(NOTE_FS5, TWO);
        playTone(NOTE_E5, TWO);

        // Repeat or end as needed
        //TPM0->CONTROLS[1].CnV = 0;  // Ensure the buzzer is off after the melody
    }
	*/
}

#endif
