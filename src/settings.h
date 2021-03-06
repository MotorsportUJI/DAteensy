#ifndef SETTINGS
#define SETTINGS

#define ScreenUART Serial7
#define RadioUART Serial8

#define EEPROM_base_address 0

#define CAN CAN3
#define DEBUG

#define OIL_PRESSURE_PIN 9
// reading 1 from this pin means that the cable is connected to GND, reading 0 means that is disconnected

#define DEBUG_LED 12

#define GREEN_BUTTON 41 // clear dtc
#define RED_BUTTON 40 // switch screen view


#endif