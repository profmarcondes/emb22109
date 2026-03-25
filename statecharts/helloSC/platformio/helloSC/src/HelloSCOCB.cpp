#include "HelloSCOCB.h"
#include "Arduino.h"
#include "arduinoPins.h"

HelloSCOCB::~HelloSCOCB() {
}

void HelloSCOCB::synchronize(){
	//synchronize lights
	//Serial.println("sync");
	if(arduino->getLed() == true)
		digitalWrite(led_red_pin, HIGH);
	else
		digitalWrite(led_red_pin, LOW);
}

void HelloSCOCB::setPeriod(){
	Serial.println("setPeriod");
}
void HelloSCOCB::reset(){
	Serial.println("reset");
}				
void HelloSCOCB::noOP(){
	Serial.println("noOP");
}				
void HelloSCOCB::inc(){
	Serial.println("inc = " + String(arduino->timer().getPeriod()) + " ms");
	
}				
void HelloSCOCB::dec(){
	Serial.println("dec = " + String(arduino->timer().getPeriod()) + " ms");
}