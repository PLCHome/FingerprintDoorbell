#ifndef MYTIMER_H
#define MYTIMER_H

#include <Arduino.h>

#define MYTIMERS 2

#define NOSCAN 0
#define PINOFFTIMER 1

void myTimerSetup();
void myTimerloop();
void myTimerDelete(uint8_t num);
void myTimerSet(uint8_t num, unsigned long millisec, void (*callback)( void ) );



#endif