#ifndef ADCTOUCHSENSOR_h
#define ADCTOUCHSENSOR_h

#include "Arduino.h"

// Kiểm tra nếu là kiến trúc STM32 nói chung
#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_GENERIC_STM32F103C) || defined(ARDUINO_ARCH_STM32F1)
  #define ADCTOUCH_DEFAULT_DELAY 20
  #define ADCTOUCH_DIVIDER        4
#else
  #define ADCTOUCH_DEFAULT_DELAY 20
  #define ADCTOUCH_DIVIDER        1
#endif

class ADCTouchSensor
{
    private:
        float reference;
        int groundedPin;
        int touchPin;
        int touchDigitalPin;
        unsigned delayTime;
        bool valid;

    private:
        void groundPortable();
    
	public:
        ADCTouchSensor(int pin, int sacrificialPin=-1, unsigned delayTimeMicroseconds=ADCTOUCH_DEFAULT_DELAY);
        int readRaw1();
};
#endif