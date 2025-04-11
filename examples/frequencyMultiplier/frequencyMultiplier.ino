//    FILE: frequencyMultiplier.ino
//  AUTHOR: Rob Tillaart
//    DATE: 2024-10-26
// PURPOSE: demo
//     URL: https://forum.arduino.cc/t/tacho-inlezen-en-puls-regelen/1315467/6

#include "Arduino.h"

const int inputPin = 9;
const int outputPin = 13;

//  INPUT VARS
int signal;
int lastSignal = HIGH;
uint32_t lastHigh = 0;

float multiplier = 0.15;        //  0.25, 2, 3, etc

//  OUTPUT VARS
uint32_t lastPulse = 0;
uint32_t outputInterval = 0;


void setup ()
{
  Serial.begin(115200);
  pinMode (outputPin, OUTPUT);
  pinMode (inputPin, INPUT);
}

void loop()
{
  //  take timestamp for this loop
  uint32_t now = millis();

  //  determine input frequency by measuring between
  //  RISING edge of HIGH pulses
  signal = digitalRead(inputPin);
  if ((signal == HIGH) && (lastSignal == LOW))
  {
    uint32_t inputInterval = now - lastHigh;
    outputInterval = inputInterval / multiplier / 2;
    lastHigh = now;  //  remember begin of last pulse

    Serial.println(inputInterval);
    Serial.println(outputInterval);
    Serial.println();
  }
  lastSignal = signal;

  //  generate output signal
  if (outputInterval > 0)
  {
    if ((now - lastPulse) >= outputInterval)
    {
      //  remember last invert output
      lastPulse = now;
      //  invert output
      digitalWrite(outputPin, !digitalRead(outputPin));
    }
  }

  //  switch to zero if no input pulse for 5 seconds
  if (now - lastHigh > 5000)
  {
    outputInterval = 0;
  }
}


//  -- END OF FILE --
