#ifndef ROBOT_MACROS
#define ROBOT_MACROS

#include "cmsis_os2.h"

#define MASK(x) (1 << (x))

#define MOVING 0x01
#define STATIONARY 0x02

#define IN_PROGRESS 0x03
#define END 0x04

osThreadId_t frontRun, frontStop, rearRun, rearStop;
osThreadId_t movingAudio, stopAudio;

#endif
