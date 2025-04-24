
#ifndef TRAFFICLIGHTOCBS_H_
#define TRAFFICLIGHTOCBS_H_

#include "Semaforo.h"

class SemaforoOCBs : public Semaforo::OperationCallback {
public:
	SemaforoOCBs(Semaforo* machine) : arduino(machine){}
	virtual ~SemaforoOCBs();

	void synchronize();

private:
	Semaforo* arduino;
};

#endif /* TRAFFICLIGHTOCBS_H_ */
