#include "HelloSCOCB.h"
#include <Arduino.h>
#include "arduinoPins.h"

HelloSCOCB::~HelloSCOCB() {
}

void HelloSCOCB::setLed(bool value){
	if(value)
		digitalWrite(led_red_pin, HIGH);
	else
		digitalWrite(led_red_pin, LOW);
}

void HelloSCOCB::menu(String value){
	if(value == "PERIOD_INC" || value == "PERIOD_DEC")
		Serial.println(value + " = " + String(arduino->timer().getPeriod()) + " ms");
	else
		Serial.println(value);
}
