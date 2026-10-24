#include <Arduino.h>
#include "HSI.h"  
#include "ADCTouchSensor.h"

ADCTouchSensor pressure1(PA0, PA1, 50);
ADCTouchSensor pressure2(PA3, PA1, 50);
ADCTouchSensor pressure3(PA6, PA1, 50);
ADCTouchSensor pressure4(PB1, PA1, 50);

const unsigned int SAMPLES = 90;
float Ref1 = 0, Ref2 = 0, Ref3 = 0, Ref4 = 0;

// Đo xen kẽ: mỗi vòng đo lần lượt 4 chân, lặp lại SAMPLES lần rồi mới tính trung bình
void readSensorsInterleaved(unsigned int samples, float &out1, float &out2, float &out3, float &out4) {
  long total1 = 0, total2 = 0, total3 = 0, total4 = 0;

  for (unsigned int i = 0; i < samples; i++) {
    total1 += pressure1.readRaw1();
    total2 += pressure2.readRaw1();
    total3 += pressure3.readRaw1();
    total4 += pressure4.readRaw1();
  }

  out1 = (float)total1 / ADCTOUCH_DIVIDER / samples;
  out2 = (float)total2 / ADCTOUCH_DIVIDER / samples;
  out3 = (float)total3 / ADCTOUCH_DIVIDER / samples;
  out4 = (float)total4 / ADCTOUCH_DIVIDER / samples;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  printSystemClockInfo(Serial);
  analogReadResolution(12);

  Serial.println("Warming up ADC...");
  float test1, test2, test3, test4;
  for (int k = 0; k < 5; k++) {
    readSensorsInterleaved(100, test1, test2, test3, test4);
    delay(50);
  }

  Serial.println("Calibrating baseline (500 samples)...");
  readSensorsInterleaved(500, Ref1, Ref2, Ref3, Ref4);

  Serial.println("--- Bat dau do ---");
}

void loop() {
  uint32_t tStart = micros();

  float raw1, raw2, raw3, raw4;
  readSensorsInterleaved(SAMPLES, raw1, raw2, raw3, raw4);

  uint32_t tEnd = micros();
  float totalTimeMs = (tEnd - tStart) / 1000.0f;

  float value1 = raw1 - Ref1;
  float value2 = raw2 - Ref2;
  float value3 = raw3 - Ref3;
  float value4 = raw4 - Ref4;

  Serial.print(value1, 2);
  Serial.print("    ");
  Serial.print(value2, 2);
  Serial.print("    ");
  Serial.print(value3, 2);
  Serial.print("    ");
  Serial.print(value4, 2);
  Serial.println(";");
  Serial.print(totalTimeMs, 2);
  Serial.println(" ms");

  delay(500);
}