#include "Arduino.h"
#include "ADCTouchSensor.h"

ADCTouchSensor::ADCTouchSensor(int pin, int sacrificialPin, unsigned delayTimeMicroseconds) {
    touchPin = pin;
    touchDigitalPin = pin;    
    reference = 0;
    groundedPin = sacrificialPin;
    delayTime = delayTimeMicroseconds;
    
    if (sacrificialPin >= 0) {
        valid = true;
    }   
    else {
        valid = false; 
    }
}

int ADCTouchSensor::readRaw1() {
    pinMode(touchDigitalPin, INPUT_PULLUP);
    pinMode(groundedPin, OUTPUT); 
    digitalWrite(groundedPin, 0);
    if (delayTime > 0)
        delayMicroseconds(delayTime);
    analogRead(groundedPin);
    pinMode(touchDigitalPin, INPUT_ANALOG);
    int adcValue = analogRead(touchPin); 
    return adcValue;
}

