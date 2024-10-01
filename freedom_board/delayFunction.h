#ifndef delayFunction
#define delayFunction

#include "MKL25Z4.h"

/* Delay Routine (Non-Optimal) */
static void delay(volatile uint32_t nof) {
	while(nof != 0) {
		__asm("NOP");
		nof--;
	}
}

#endif
