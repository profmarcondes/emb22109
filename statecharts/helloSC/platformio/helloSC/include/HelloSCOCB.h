#ifndef HELLOSCOCB_H
#define HELLOSCOCB_H

#include <Arduino.h>
#include "HelloSC.h"

class HelloSCOCB: public HelloSC::OperationCallback {

public:
	HelloSCOCB(HelloSC* machine) : arduino(machine){}
	virtual ~HelloSCOCB();

	void setLed(bool value);
	void menu(String value);


private:
	HelloSC* arduino;

};

#endif
