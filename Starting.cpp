/*
 * Starting.cpp
 *
 *  Created on: Jul 25, 2020
 *      Author: OS1
 */

#include "Starting.h"
#include "Thread.h"
#include "PCB.h"

extern int syncPrintf(const char *format, ...);

Starting::Starting():Thread() {}

void Starting::startingPCB(){
	this->myPCB->state=PCB::READY;
}
