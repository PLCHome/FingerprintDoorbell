#include "myTimer.h"

struct myMillsTimerT {    
    unsigned long time;
    void (*callback)( void );
};

myMillsTimerT myMillsTimer[MYTIMERS];


void myTimerSetup(){
  for (uint8_t i=0; i<MYTIMERS; i++){
    myMillsTimer[i].time = 0;
  }
}

void myTimerloop(){
  for (uint8_t i=0; i<MYTIMERS; i++){
    if (myMillsTimer[i].time && myMillsTimer[i].time < millis()){
        myMillsTimer[i].time=0;
        myMillsTimer[i].callback();
    }
  }
}

void myTimerSet(uint8_t num, unsigned long millisec, void (*callback)( void ) ){
    myMillsTimer[num].callback = callback;
    myMillsTimer[num].time = millis() + millisec;
}

void myTimerDelete(uint8_t num){
    myMillsTimer[num].time = 0;
}
