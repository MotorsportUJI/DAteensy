#include <Arduino.h>

#include "lib/OBD2/OBD2.h"
#include "lib/persistence/persistance.h"
#include "lib/SD/SDstore.h"

#include "lib/SteeringWheel/SteeringWheel.h"
#include "lib/Radio/Radio.h"

#include "lib/sensors/gear.h"
#include "lib/sensors/buttons.h"



#include "settings.h"

OBD2sensordata OBD2db = {0};
Packet RadioPacket = {0};


void setup() {
    // init serial
    #ifdef DEBUG
    Serial.begin(115200); 
    #endif


    initScreen(ScreenUART);
    initRadio(RadioUART);

    initOBD2(OBD2db);
    initSD();

    initGear();

    rpmled(0);
    OBD2db.engine_rpmA=0;
    OBD2db.engine_rpmB=0;

    pinMode(DEBUG_LED, OUTPUT);
}


uint32_t elapsed_minute = 0;
uint32_t elapsed_second = 0;
uint32_t elapsed_100ms = 0;
boolean previous_contact = false;

void loop() {
    // execute always


    OBD2events();

    // shutdown screen if contact is off
    if (isContact()){
        if (!previous_contact){
            enable();
            previous_contact = true;
        }
    } else{
        if (previous_contact){
            disable();
            previous_contact = false;
        }
    }

    // update screen
    sendRPM(OBD2RPM(OBD2db));
    sendCOLTMP(OBD2TMP(OBD2db.Engine_coolant_temperature));
    sendAIRTMP(OBD2TMP(OBD2db.intake_air_temperature));
    sendTPS(OBD2PC(OBD2db.relavite_throttle_position));
    //sendTrim1(OBD2Trim(OBD2db.long_term_fuel_trim));
    //sendTrim2(OBD2Trim(OBD2db.oxygen_sensor_long_term_fuel_trim));
    sendPressure1(OBD2db.absolute_barometric_presure);
    sendPressure2(OBD2db.intake_manifold_absolute_pressure);
    sendDTCcount(OBD2db.DTC_CNT);

    sendGear(getGear());

    //update rpm LEDS
    rpmledInverse(OBD2RPM(OBD2db)/1000);

    // check buttons
    checkbuttons();


    // execute each 100ms
    if (millis() - elapsed_100ms > 100){
        // print data to sd
        String to_save = "";
        to_save += millis();
        to_save += ",";
        to_save += OBD2toCSV(OBD2db);
        saveLine(to_save);
        elapsed_100ms = millis();
    }

    // execute each second
    if (millis() - elapsed_second > 1000){
        // update radio packet
        //RadioPacket.rpm = OBD2RPM(OBD2db);

        // send data over radio
        //sendPacket(RadioPacket);

        //printOBD2ALL(OBD2db);


        elapsed_second = millis();
    }

    // execute each minute
    if (millis() - elapsed_minute > 60* 1000){

        // increase time alive counter
        increaseTimeCounter(EEPROM_base_address);
        elapsed_minute = millis();
        // increase engine on time??

    }

}