#ifndef STEERINGWHEEL
#define STEERINGWHEEL

#include <Arduino.h>
void initScreen(HardwareSerial &_serial);

void sendGear(int str);
void sendRPM(int str);

#endif