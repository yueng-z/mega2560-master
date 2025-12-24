#ifndef __ESP32_H
#define __ESP32_H 
#include "board.h"
#include <iom2560v.h>
#include <macros.h>

void esp32Init(void);

void uart2_init(void);
void esp32Process(void);



//void delay_ms(int n);

#endif

