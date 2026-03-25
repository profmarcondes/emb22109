#ifndef HELLOSCOCB_H
#define HELLOSCOCB_H

#include "HelloSC.h"

class HelloSCOCB: public HelloSC::OperationCallback {

public:
	HelloSCOCB(HelloSC* machine) : arduino(machine){}
	virtual ~HelloSCOCB();

	void synchronize();

	void setPeriod();
				
	void reset();
				
	void noOP();
				
	void inc();
				
	void dec();

private:
	HelloSC* arduino;

};

#endif