/*
 * KernelEv.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Thread.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include "IVTEntry.h"
#include "Kernel.h"

extern int syncPrintf(const char *format, ...);

KernelEv::KernelEv(IVTNo ivtNo) {
	lock();

	this->value=0;
	this->ivtNo=ivtNo;
	this->owner=PCB::running->thread;
	IVTEntry::IVTEntryTable[ivtNo]->event=this;
	//syncPrintf("Startovan dogadjaj\n");

	unlock();

}

KernelEv::~KernelEv() {
	lock();

	IVTEntry::IVTEntryTable[ivtNo]->event=0;
	//syncPrintf("Izbrisan dogadjaj\n");
	this->owner=0;

	unlock();
}

void KernelEv::wait(){//RADI
	lock();

	if(PCB::running->thread!=this->owner) {
		unlock();
		return;
	}
	//this->value--;

	if(this->value==0 ){
		this->owner->myPCB->state=PCB::BLOCKED;

		unlock();
		dispatch();
		lock();
	} else {
		this->value=0;
	}

	unlock();
}


void KernelEv::signal(){//RADI
	lock();

	if(this->value==0 && this->owner->myPCB->state==PCB::BLOCKED){
		//this->value++;
		this->owner->myPCB->state=PCB::READY;
		Scheduler::put(this->owner->myPCB);

		unlock();
		dispatch();
		lock();
	}
	else{
		this->value=1;
	}

	unlock();
}
