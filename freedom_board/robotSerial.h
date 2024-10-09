#ifndef robotSerial
#define robotSerial

#include "MKL25Z4.h"
#include "queue.h"

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

/*
#define LED_PIN 20 //
#define MOCK 0x69

volatile int DATA = 0;
*/

/* Functions transmit and receive data */

/* Initialises the UART2 to generate interrupts */
void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->C2 = 0;
	UART2->C3 = 0;
	
	UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	UART2->C2 |= UART_C2_TIE_MASK | UART_C2_RIE_MASK;
	
	Q_Init(&tx_q); // Initialises the transmit queue
	Q_Init(&rx_q); // Initialises the receive queue
}

/* Handles the interrupts */
void UART2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	// If the transmit flag is raised
	if (UART2->S1 & UART_S1_TDRE_MASK) {
		// Data register to able to accept more characters
		if (!Q_Empty(&tx_q)) {
			UART2->D = Q_Dequeue(&tx_q);
		} else {
			// Queue is empty, disable transmit (enable else where)
			UART2->C2 &= ~UART_C2_TIE_MASK;
		}
	}
	
	// If the receive flag is raised
	else if(UART2->S1 & UART_S1_RDRF_MASK) {
		// Data register has characters
		if (!Q_Full(&rx_q)) {
			Q_Enqueue(&rx_q, UART2->D);
		} else {
			// Queue is full, wait till queue is empty again
			
			/*
			// Clear Queue
			Q_Init(&rx_q);
			*/
		}
	}
}


#endif
