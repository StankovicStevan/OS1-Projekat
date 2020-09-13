/*
 * PREPENT.h
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#ifndef PREPENT_H_
#define PREPENT_H_

#include "IVTEntry.h"
#include "KernelEv.h"
#include "Thread.h"

#define PREPAREENTRY(ivtNo, oldIR)\
void interrupt interruptR##ivtNo(...); \
IVTEntry ivtEnt##ivtNo(ivtNo,interruptR##ivtNo);\
void interrupt interruptR##ivtNo(...){\
	if(oldIR)ivtEnt##ivtNo.callOldIR();\
	ivtEnt##ivtNo.signal();\
	dispatch();\
}



#endif /* PREPENT_H_ */
