/*
 * KernelEv.h
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "Event.h"

class Event;
class Thread;
class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();
	void wait();
	void signal();

private:
	friend class Event;
	friend class IVTEntry;
	IVTNo ivtNo;
	Thread* owner;
	int value;

};

#endif /* KERNELEV_H_ */
