/*
 * PCB.cpp
 *
 *  Created on: Jul 20, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "List.h"
#include "Queue.h"
#include "Schedule.h"
#include <dos.h>
#include "Kernel.h"

extern int syncPrintf(const char *format, ...);

ID PCB::recentId=1;//Dodeljujemo novoj niti sledeci ID
volatile PCB* PCB::running=0;


PCB::PCB(Thread* thread, StackSize stackSize, Time timeSlice) {
	lock();

	this->thread=thread;
	this->stackSize=stackSize/sizeof(unsigned);
	this->timeSlice=timeSlice;
	this->quantum=0;
	this->state=NEW;
	this->id=recentId++;
	this->stackPointer=0;
	this->queueBlocked=new Queue();
	this->semaphoreWait=-1;
	User::list->add(this);
	//syncPrintf("Dodata nit %d u listu, usao u konstruktor PCB\n", this->id);

	unlock();
}

PCB::~PCB() {
	lock();

	delete[] this->stackPointer;
	delete this->queueBlocked;

	unlock();
}

void PCB::wrapper(){
	running->thread->run();
	//syncPrintf("Usao u wrapper");

	lock();

	running->state=OVER;
	while(running->queueBlocked->count>0){
		PCB* tek=running->queueBlocked->get();
		tek->state=READY;
		Scheduler::put(tek);
	}
	unlock();
	dispatch();

}

void PCB::createStack(){
	lock();

	unsigned* st=new unsigned[this->stackSize];

#ifndef BCC_BLOCK_IGNORE

	st[this->stackSize-1]=0x200; //Setovanje I flag-a u pocetnom PSW-u za nit

	//Postvljanje adrese funkcije koju ce nit da izvrsava(preko wrapper-a)
	st[this->stackSize-2]=FP_SEG(&(wrapper));
	st[this->stackSize-3]=FP_OFF(&(wrapper));

	//Sacuvali smo 9 registara uz ova 3 pri ulasku u prekidnu rutinu
	this->sp=FP_OFF(st+this->stackSize-12);
	this->ss=FP_SEG(st+this->stackSize-12);

#endif

	this->stackPointer=st;

	unlock();
}

