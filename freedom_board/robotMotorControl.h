#ifndef robotMotorControl
#define robotMotorControl

#include "MKL25Z4.h"

#include "robotMacros.h"

/* Functions control motors for movement */

#define STRAIGHT          0
#define TURN_CLOCKWISE    1
#define REVERSE           2
#define TURN_ANTICLOCKWISE 3
#define FORWARD_RIGHT_TURN 4
#define FORWARD_LEFT_TURN 5
#define BACKWARDS_LEFT_TURN 6
#define BACKWARDS_RIGHT_TURN 7

//volatile int state = STRAIGHT; // Initial state
//volatile int speed = 0;         // Initial speed

void initMotor() {
    // Enable clock for TPM1
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

    // Set TPM clock source
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Use MCGFLLCLK or MCGPLLCLK

    // Set the modulo value for PWM frequency (1 kHz)
    TPM1->MOD = 4999; // Assuming 48MHz clock

    TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
    TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(2));
    TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Configure PTB0 (Channel 0) for PWM
    PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[0] = PORT_PCR_MUX(3); // Set PTB0 to TPM0_CH0
    TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    TPM1_C0V = 1;
    
    // Configure PTB1 (Channel 1) for PWM
    PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[1] = PORT_PCR_MUX(3); // Set PTB1 to TPM0_CH1
    TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    TPM1_C1V = 1;

    // Initialize PTB2 and PTB3 for motor direction control
    PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[2] = PORT_PCR_MUX(1); // Set PTB2 as GPIO
    PORTB->PCR[3] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[3] = PORT_PCR_MUX(1); // Set PTB3 as GPIO
    PTB->PDDR |= MASK(2) | MASK(3); // Set PTB2 and PTB3 as outputs

    // Start the TPM0 timer
    TPM1->SC |= TPM_SC_CMOD(1); // Set the timer counter mode
	}

void motorControl(int state, int speed) {
    //speed should be between 0 to 100
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    uint16_t dutyCycle = (speed * 4999) / 100; // Convert speed to duty cycle

    // Set PWM and direction based on motor state
    switch (state) {
        case STRAIGHT:
            TPM1_C0V = dutyCycle; // Motor 1
            TPM1_C1V = 4998; // Motor 2
            PTB->PSOR |= (1 << 2); // PTB2 (AIN2) HIGH
            PTB->PCOR |= (1 << 3); // PTB3 (BIN2) HIGH
            break;

        case TURN_CLOCKWISE:
            TPM1_C0V = dutyCycle; // Motor 1
            TPM1_C1V = dutyCycle; // Motor 2
            PTB->PSOR |= (1 << 3) | (1 << 2); // PTB3 (BIN2) HIGH
            //PTB->PSOR |= (1 << 2); // PTB2 (AIN2) HIGH
            break;

        case REVERSE:
            TPM1_C0V = dutyCycle; // Motor 1
            TPM1_C1V = dutyCycle; // Motor 2
            PTB->PSOR |= (1 << 3); // PTB3 (BIN2) HIGH
            PTB->PDOR &= ~(1 << 2); // PTB2 (AIN2) LOW
            break;

        case TURN_ANTICLOCKWISE:
            TPM1_C0V = dutyCycle; // Motor 1
            TPM1_C1V = dutyCycle; // Motor 2
            PTB->PCOR |= (1 << 2); // PTB2 (AIN2) LOW
            PTB->PCOR |= (1 << 3); // PTB3 (BIN2) LOW
            break;

        case FORWARD_RIGHT_TURN:
            TPM1_C0V = dutyCycle; // Motor 1 (faster)
            TPM1_C1V = dutyCycle * 0.7; // Motor 2 (slower)
            PTB->PSOR |= (1 << 2); // PTB2 (AIN2) HIGH
            PTB->PCOR |= (1 << 3); // PTB3 (BIN2) LOW
            break;

        case FORWARD_LEFT_TURN:
            TPM1_C0V = dutyCycle * 0.7; // Motor 1 (slower)
            TPM1_C1V = dutyCycle; // Motor 2 (faster)
            PTB->PSOR |= (1 << 2); // PTB2 (AIN2) HIGH
            PTB->PCOR |= (1 << 3); // PTB3 (BIN2) LOW
            break;

        case BACKWARDS_LEFT_TURN:
            TPM1_C0V = dutyCycle * 0.7; // Motor 1 (slower)
            TPM1_C1V = dutyCycle; // Motor 2 (faster)
            PTB->PSOR |= (1 << 3); // PTB3 (BIN2) HIGH
            PTB->PCOR |= (1 << 2); // PTB2 (AIN2) LOW
            break;

        case BACKWARDS_RIGHT_TURN:
            TPM1_C0V = dutyCycle; // Motor 1 (faster)
            TPM1_C1V = dutyCycle * 0.7; // Motor 2 (slower)
            PTB->PSOR |= (1 << 3); // PTB3 (BIN2) HIGH
            PTB->PCOR |= (1 << 2); // PTB2 (AIN2) LOW
            break;

        default:
            // Stop motors if state is invalid
						PTB->PCOR |= ((1 << 2) | (1 << 3));
            TPM1_C0V = 1; // Motor 1
            TPM1_C1V = 1; // Motor 2
            break;
    }
}


#endif
