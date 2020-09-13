/*
 * Idle.cpp
 *
 *  Created on: Jul 25, 2020
 *      Author: OS1
 */

#include "Idle.h"

#include "Kernel.h"
#include "PCB.h"
#include "Thread.h"

extern int syncPrintf(const char *format, ...);

Idle::Idle():Thread(defaultStackSize,defaultTimeSlice) {}

void Idle::run(){
	while(1); //Vrti se u beskonacno petlji ako ne postoji nijedna druga nit
}

void Idle::start(){
	lock();
	this->myPCB->state=PCB::IDLE;
	this->myPCB->createStack();
	unlock();
}
