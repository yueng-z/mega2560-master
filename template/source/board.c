/* This putchar works with the M2560 UART0. It will work with other devices
 * if their UART registers use the same names.  If the UART IO locations
 * are different from the M2560, then you will also have to change the
 * #include header file statement.
 *
 * For device with multiple UARTs and if you want to use a different UART,
 * change the IO register names as appropriate.
 */
#include <iom2560v.h>
#include <stdio.h>
#include "board.h"
#include "timer.h"
#include "i2c.h"
#include <ASSERT.H>
#include "key.h"
#include "queue.h"
extern int _textmode;

const uint16_t port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&PORTA,
	(uint16_t)&PORTB,
	(uint16_t)&PORTC,
	(uint16_t)&PORTD,
	(uint16_t)&PORTE,
	(uint16_t)&PORTF,
	(uint16_t)&PORTG,
	(uint16_t)&PORTH,
	NOT_A_PORT,
	(uint16_t)&PORTJ,
	(uint16_t)&PORTK,
	(uint16_t)&PORTL,
};

const uint16_t port_to_input_PGM[] = {
	NOT_A_PIN,
	(uint16_t)&PINA,
	(uint16_t)&PINB,
	(uint16_t)&PINC,
	(uint16_t)&PIND,
	(uint16_t)&PINE,
	(uint16_t)&PINF,
	(uint16_t)&PING,
	(uint16_t)&PINH,
	NOT_A_PIN,
	(uint16_t)&PINJ,
	(uint16_t)&PINK,
	(uint16_t)&PINL,
};
/*
#define TIMER1A 3
#define TIMER2B 8
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
const uint8_t  digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	TIMER4A	, // PH 3 ** 6 ** PWM6	
	TIMER4B	, // PH 4 ** 7 ** PWM7	
	TIMER4C	, // PH 5 ** 8 ** PWM8	
	TIMER2B	, // PH 6 ** 9 ** PWM9	
	TIMER1A	 // PB 5 ** 11 ** PWM11	
};*/
QUEUE_ALLOCATE_DATA(uartQueue, 64);
Queue q;

void Print_Binary(unsigned int x)
{
    if (x > 1)
    {
        Print_Binary(x >> 1);
    }
    putchar((x & 1) ? '1' : '0');
}

void _assert(char *value, char * file, int line)
{
	printf("ParaError:%s, File: %s, Line:%d\r\n",value, file,line);
}

void port_init(void)
{
	PORTA = 0x00;
	DDRA = 0xFF;
	PORTB = 0x00;
	DDRB = 0xB7;
	PORTC = 0x00; // m103 output only
	DDRC = 0xF0;
	PORTD = 0x00;
	DDRD = 0x00;
	PORTE = 0x00;
	DDRE = 0x00;
	PORTF = 0xFF;
	DDRF = 0xFF;
	PORTG = 0x00;
	DDRG = 0x00;
	PORTH = 0x00;
	DDRH = 0x78;
	PORTJ = 0x00;
	DDRJ = 0x00;
	PORTK = 0x00;
	DDRK = 0x00;
	PORTL = 0x00;
	DDRL = 0x00;
}

// SPI initialize
//  clock rate: 4000000hz
void spi_init(void)
{
	SPCR = 0x50; // setup SPI
	SPSR = 0x01; // setup SPI
}
// ADC initialize
//  Conversion time: 104uS
void adc_init(void)
{
	ADCSRA = 0x00; // disable adc
	ADMUX = 0x00;  // select adc input 0
	ACSR = 0x80;
	ADCSRA = 0x85;
	ADMUX = 0x40; // select adc input 0
}

// UART0 initialize
//  desired baud rate: 115200
//  char size: 8 bit
//  parity: Disabled
void uart0_init(void)
{
	UCSR0B = 0x00; // disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x08; // set baud rate lo
	UBRR0H = 0x00; // set baud rate hi
	UCSR0B = 0x98;
}
#pragma interrupt_handler uart0_rx_isr:iv_USART0_RXC
void uart0_rx_isr(void)
{
 #if ESP32_DEBUG
  UDR2 = UDR0;
 #endif
}

//UART1 initialize
// desired baud rate:9600
// actual baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart1_init(void)
{
 UCSR1B = 0x00; //disable while setting baud rate
 UCSR1A = 0x00;
 UCSR1C = 0x06;
 UBRR1L = 0x67; //set baud rate lo
 UBRR1H = 0x00; //set baud rate hi
 UCSR1B = 0x98;
}

#pragma interrupt_handler uart1_rx_isr:iv_USART1_RXC
void uart1_rx_isr(void)
{
  UDR0 = UDR2;
}


//TIMER1 initialize - prescale:64
// WGM: 5) PWM 8bit fast, TOP=0x00FF
// desired value: 1mSec
// actual value:  4.096mSec (309.6%)
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFF; //setup
 TCNT1L = 0x01;
 OCR1AH = 0x00;
 OCR1AL = 0xFF;
 OCR1BH = 0x00;
 OCR1BL = 0xFF;
 OCR1CH = 0x00;
 OCR1CL = 0x00;
 ICR1H  = 0x00;
 ICR1L  = 0xFF;
 TCCR1A = 0x81;
 TCCR1C = 0x00;
 TCCR1B = 0x0B; //start Timer
}
//TIMER2 initialize - prescale:64
// WGM: PWM Fast
// desired value: 1mSec
// actual value:  4.096mSec (309.6%)
void timer2_init(void)
{
 TCCR2B = 0x00;    //stop
 TCNT2  = 0x01; //setup
 ASSR   = 0x00;  //set async mode
 OCR2A  = 0xFF;
 OCR2B  = 0x00;
 TCCR2A = 0x23;
 TCCR2B = 0x03; //start
}

//TIMER4 initialize - prescale:64
// WGM: 5) PWM 8bit fast, TOP=0x00FF
// desired value: 1mSec
// actual value:  4.096mSec (309.6%)
void timer4_init(void)
{
 TCCR4B = 0x00; //stop
 TCNT4H = 0xFF; //setup
 TCNT4L = 0x01;
 OCR4AH = 0x00;
 OCR4AL = 0xFF;
 OCR4BH = 0x00;
 OCR4BL = 0xFF;
 OCR4CH = 0x00;
 OCR4CL = 0xFF;
 ICR4H  = 0x00;
 ICR4L  = 0xFF;
 TCCR4A = 0xA9;
 TCCR4C = 0x00;
 TCCR4B = 0x0B; //start Timer
}
// call this routine to initialize all peripherals
void boardInit(void)
{
	// stop errant interrupts until set up
	CLI();		  // disable all interrupts
	XMCRA = 0x00; // external memory
	XMCRB = 0x00; // external memory
	port_init();
	adc_init();
	timer0_init();  //timebase
	timer1_init(); //PWM
	timer2_init(); //PWM
	timer4_init();  //PWM
	spi_init();
	uart0_init();
	uart1_init();	
	I2C_init();
	

	MCUCR = 0x00;
	EICRA = 0x00;  // pin change int edge 0:3
	EICRB = 0x00;  // pin change int edge 4:7
	PCICR = 0x00;  // pin change int enable
	PCMSK0 = 0x00; // pin change mask
	PCMSK1 = 0x00; // pin change mask
	PCMSK2 = 0x00; // pin change mask
	EIMSK = 0x00;
	TIMSK0 = 0x02; // timer0 interrupt sources
	TIMSK1 = 0x00; // timer1 interrupt sources
	TIMSK2 = 0x00; // timer2 interrupt sources
	TIMSK3 = 0x00; // timer3 interrupt sources
	TIMSK4 = 0x00; // timer4 interrupt sources
	TIMSK5 = 0x00; // timer5 interrupt sources
	PRR0 = 0x00;
	PRR1 = 0x00;
	
	q = QUEUE_INITIALISE_DATA( uartQueue, 64 );

	SEI(); // re-enable interrupts
	// all peripherals are now initialized
}
#define IS_PORT(port)       (((port) == PA)||((port) == PB)||((port) == PC)||((port) == PD)|| \
								((port) == PE)||((port) == PF)||((port) == PG)||((port) == PH)|| \
								((port) == PJ)||((port) == PK)||((port) == PL))
#define IS_PWM(pwmCh) 	    (((pwmCh) == PWM_REG_R)||((pwmCh) == PWM_REG_G)||((pwmCh) == PWM_REG_B)||\
							((pwmCh) == PWM_DCM_FOR)||((pwmCh)== PWM_DCM_BAK))
							
void gpioWrite(uint8_t port, uint8_t pin, uint8_t val)
{
	volatile uint8_t *out;
	assert(IS_PORT(port));
	if (port == NOT_A_PIN)
		return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	out = (volatile unsigned char *)port_to_output_PGM[port];
	if (val == LOW)
	{
		*out &= ~BIT(pin);
	}
	else
	{
		*out |= BIT(pin);
	}
}

void UART1_send(U8* buf, U8 len)
{
	U8 i = 0;
	for(i= 0; i<len;i++)
	{
		UDR1 = *(buf+i);
		while(!(UCSR1A & (1 << UDRE0)));  
	}
}

void gpioToggle(uint8_t port, uint8_t pin)
{
	volatile uint8_t *out;
	assert(IS_PORT(port));
	if (port == NOT_A_PIN)
		return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	out = (volatile unsigned char *)port_to_output_PGM[port];
	*out ^= BIT(pin);
}

uint8_t gpioRead(uint8_t port, uint8_t pin)
{
	volatile uint8_t *reg;
	assert(IS_PORT(port));
	if (port == NOT_A_PIN)
		return LOW;
	reg = (volatile unsigned char *)port_to_input_PGM[port];
	if (*reg & BIT(pin))
		return HIGH;
	return LOW;
}

#define DEFAULT 1
int analogRead(uint8_t channel)
{
	uint8_t low, high;
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((channel >> 3) & 0x01) << MUX5);
	ADMUX = (DEFAULT << 6) | (channel & 0x07);
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC))
		;
	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

void pwmWrite(uint8_t pwmChanel, uint8_t val)
{
	assert(IS_PWM(pwmChanel));
	if (val > 255) val = 255; 

	switch(pwmChanel)
	{
		case PWM_REG_R:
			sbi(TCCR4A, COM4A1);
			OCR4A = val;	// set pwm duty
		break;

		case PWM_REG_G:
			sbi(TCCR4A, COM4B1);
			OCR4B = val;	// set pwm duty
		break;

		case PWM_REG_B:
			sbi(TCCR4A, COM4C1);
			OCR4C = val;	// set pwm duty
		break;

		case PWM_DCM_FOR:
			sbi(TCCR2A, COM2B1);
			OCR2B = val; // set pwm duty
		break;

		case PWM_DCM_BAK:
			sbi(TCCR1A, COM1A1);
			OCR1A = val; // set pwm duty
		break;
		default: break;
	}
}

uint8_t spitransferByte(uint8_t data)
{
    SPDR = data;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    NOP();
    while (!(SPSR & BIT(SPIF))) ; // wait
    return SPDR;
}


void spiTransferBuf(void *buf, uint8_t count) 
{
    uint8_t *p = (uint8_t *)buf;
	uint8_t in;
	if (count == 0) return;
    SPDR = *p;
    while (--count > 0) {
      uint8_t out = *(p + 1);
      while (!(SPSR & BIT(SPIF))) ;
      //in = SPDR;
      SPDR = out;
      //*p++ = in;
    }
    while (!(SPSR & BIT(SPIF))) ;
    //*p = SPDR;
}
  
int putchar(char c)
{
	if (_textmode && c == '\n')
		putchar('\r');
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
		;
	/* Putting data into buffer , sends the data */
	UDR0 = c;
	return c;
}
static uint8_t buzzEnable = 0;
static U16 buzzTimer = 0;
#define buzzClose() cbi(PORTB,4)
#define buzzOpen()  sbi(PORTB,4)
void buzzWorkOnce(void)
{
	buzzOpen();
	buzzEnable = 1;
	buzzTimer = Timer_GetTickCount();
}

static void buzz_process(void)
{	
	if(buzzEnable){
		if(Timer_PassedDelay(buzzTimer,50)){
			buzzEnable = 0;
			buzzClose();
		}
	}
}

void board_process(void)
{
	buzz_process();
	keyBoard_process();

}

