
#ifndef __TIMER_H
#define __TIMER_H 1
#include "board.h"

#define time_after(a, b) ((S16)((S16)(b) - (S16)(a)) < 0)
#define Timer_PassedDelay(startTime, msDelay) (time_after(Timer_GetTickCount(), (startTime + msDelay)))
#define interval(a, b) ((S16)((S16)(b) - (S16)(a))) // systick decount

#define Timer_GetTickCount() (systick_ms)

void timer0_init(void);
extern int systick_ms;

#endif