#ifndef robotMotorControl
#define robotMotorControl

#include <MKL25Z4.h>

#define STRAIGHT 				0
#define TURN_CLOCKWISE 			1
#define REVERSE 				2
#define TURN_ANTICLOCKWISE 		3
#define FORWARD_RIGHT_TURN 		4
#define FORWARD_LEFT_TURN 		5
#define BACKWARDS_LEFT_TURN		6
#define BACKWARDS_RIGHT_TURN 	7
#define STOP 					8

void initMotor() {
	// Enable clock for TPM1 and TPM2
	SIM->SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[0] = PORT_PCR_MUX(3); // Set PTB0 to TPM0_CH0

	PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[1] = PORT_PCR_MUX(3); // Set PTB1 to TPM0_CH1

	// Set TPM clock source
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	// Set the modulo value for PWM frequency (1 kHz)
	TPM1->MOD = 4999; 
	TPM2->MOD = 4999;

	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);


	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(2));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

	// Configure PTB0 (TPM1 Channel 0) for PWM
	PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[0] = PORT_PCR_MUX(3); // Set PTB0 to TPM0_CH0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM1_C0V = 0;

	// Configure PTB1 (TPM1 Channel 1) for PWM
	PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[1] = PORT_PCR_MUX(3); // Set PTB1 to TPM0_CH1
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM1_C1V = 0;

	// Configure PTB2 (TPM2 Channel 0) for PWM
	PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[2] = PORT_PCR_MUX(3); // Set PTB2 to TPM0_CH4
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C0V = 0;

	// Configure PTB3 (TPM2 Channel 1) for PWM
	PORTB->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[3] = PORT_PCR_MUX(3); // Set PTB3 to TPM0_CH5
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C1V = 0;

	// Start the TPM0 timer
	TPM1->SC |= TPM_SC_CMOD(1); // Set the timer counter mode
	TPM2->SC |= TPM_SC_CMOD(1);
}

void motorControl(int state, int speed) {
	//speed should be between 0 to 100
	if (speed < 0) speed = 0;
	if (speed > 100) speed = 100;
	int dutyCycle = (speed * 4999) / 100; // Convert speed to duty cycle

	// Set PWM and direction based on motor state
	switch (state) {
	case STRAIGHT:
		TPM1_C0V = dutyCycle;	// A1 IN
		TPM1_C1V = 0; 			// A2 IN
		TPM2_C0V = dutyCycle;	// B1 IN
		TPM2_C1V = 0; 			// B2 IN
		break;

	case TURN_CLOCKWISE:
		TPM1_C0V = dutyCycle;	// A1 IN
		TPM1_C1V = 0; 			// A2 IN
		TPM2_C0V = 0; 			// B1 IN
		TPM2_C1V = dutyCycle;	// B2 IN
		break;

	case REVERSE:
		TPM1_C0V = 0; 			// A1 IN
		TPM1_C1V = dutyCycle;	// A2 IN
		TPM2_C0V = 0; 			// B1 IN
		TPM2_C1V = dutyCycle;	// B2 IN
		break;

	case TURN_ANTICLOCKWISE:
		TPM1_C0V = 0; 			// A1 IN
		TPM1_C1V = dutyCycle;	// A2 IN
		TPM2_C0V = dutyCycle;	// B1 IN
		TPM2_C1V = 0; 			// B2 IN
		break;

	case FORWARD_RIGHT_TURN:
		TPM1_C0V = dutyCycle; 		// A1 IN
		TPM1_C1V = 0; 				// A2 IN
		TPM2_C0V = dutyCycle * 0.7; // B1 IN
		TPM2_C1V = 0; 				// B2 IN
		break;

	case FORWARD_LEFT_TURN:
		TPM1_C0V = dutyCycle * 0.7; // A1 IN
		TPM1_C1V = 0; 				// A2 IN
		TPM2_C0V = dutyCycle; 		// B1 IN
		TPM2_C1V = 0; 				// B2 IN
		break;

	case BACKWARDS_LEFT_TURN:
		TPM1_C0V = 0; 				// A1 IN
		TPM1_C1V = dutyCycle * 0.7; // A2 IN
		TPM2_C0V = 0; 				// B1 IN
		TPM2_C1V = dutyCycle; 		// B2 IN
		break;

	case BACKWARDS_RIGHT_TURN:
		TPM1_C0V = 0; 				// A1 IN
		TPM1_C1V = dutyCycle; 		// A2 IN
		TPM2_C0V = 0; 				// B1 IN
		TPM2_C1V = dutyCycle * 0.7; // B2 IN
		break;

	default:
		// Stop motors if state is invalid
		TPM1_C0V = 0; // A1 IN
		TPM1_C1V = 0; // A2 IN
		TPM2_C0V = 0; // B1 IN
		TPM2_C1V = 0; // B2 IN
		break;
	}
}

#endif
