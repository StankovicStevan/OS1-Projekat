/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelEv.h"
#include <dos.h>
#include "Kernel.h"

IVTEntry* IVTEntry::IVTEntryTable[entriesNum]={0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newIR) {
	lock();

	if(ivtNo>=0 && ivtNo<entriesNum){
		ivtNo=ivtNo;
		this->IVTEntryTable[ivtNo]=this;
		event=0;
		oldIR=getvect(ivtNo);
		init(ivtNo, newIR);
		//setvect(ivtNo, newIR);
	}

	unlock();

}

void IVTEntry::init(IVTNo ivtNo, pInterrupt newIR){
	lock();

	setvect(ivtNo, newIR);

	unlock();
}

void IVTEntry::callOldIR(){
	if(oldIR==0) return;
	oldIR();
}

IVTEntry::~IVTEntry() {
	lock();

	if(oldIR)oldIR();
	restore(ivtNo, oldIR);
	//setvect(ivtNo,oldIR);
	IVTEntryTable[ivtNo]=0;

	delete this->event;

	unlock();
}

void IVTEntry::signal(){
	lock();
	if(event!=0){
		event->signal();
	}
	unlock();
}

void IVTEntry::restore(IVTNo ivtNo, pInterrupt oldIR){
	lock();

	setvect(ivtNo,oldIR);

	unlock();
}



