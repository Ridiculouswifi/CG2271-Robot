#ifndef QUEUE
#define QUEUE

#include "MKL25Z4.h"

#define Q_SIZE (32)

/* Defines the attributes of a queue */
typedef struct {
	uint8_t Data[Q_SIZE];
	uint16_t Head; // points to the oldest data element
	uint16_t Tail; // points to the next free space
	uint16_t Size; // quantity of elements in queue
} Q_T;

Q_T tx_q;
Q_T rx_q;

/* Initialises the queue to be empty */
void Q_Init(Q_T* q) {
	uint16_t i;
	for (i = 0; i < Q_SIZE; i++) {
		q -> Data[i] = 0;
	}
	q -> Head = 0;
	q -> Tail = 0;
	q -> Size = 0;
}

/* Checks if the queue is empty */
int Q_Empty(Q_T* q) {
	return q -> Size == 0;
}

/* Checks if the queue is full */
int Q_Full(Q_T* q) {
	return q -> Size == Q_SIZE;
}

/* Adds the element d to the queue */
int Q_Enqueue(Q_T* q, uint8_t d) {
	if (!Q_Full(q)) {
		q -> Data[q -> Tail++] = d;
		q -> Tail %= Q_SIZE;
		q -> Size++;
		return 1;
	} else {
		return 0;
	}
}

/* Removes and returns the element at Head */
uint8_t Q_Dequeue(Q_T* q) {
	uint8_t t = 0;
	if (!Q_Empty(q)) {
		t = q -> Data[q -> Head];
		q -> Data[q -> Head++] = 0;
		q -> Head %= Q_SIZE;
		q -> Size--;
	}
	return t;
}

#endif
