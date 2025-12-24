

#include <iom2560v.h>
#include <macros.h>
extern void interrupt_1ms(void);
unsigned int systick_ms = 0; //0--65535--0--65535
// TIMER0 initialize - prescale:256
//  WGM: CTC
//  desired value: 1mSec
//  actual value:  1.008mSec (-0.8%)
void timer0_init(void)
{
	TCCR0B = 0x00; // stop
	TCNT0 = 0xC2;  // set count
	OCR0A = 0x3E;
	TCCR0A = 0x02; // start timer
	TCCR0B = 0x04; // start timer
}

#pragma interrupt_handler timer0_compa_isr:iv_TIM0_COMPA
void timer0_compa_isr(void)
{
	systick_ms++;
	interrupt_1ms();
}