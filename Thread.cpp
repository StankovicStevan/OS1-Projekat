/*
 * Thread.cpp
 *
 *  Created on: Jul 20, 2020
 *      Author: OS1
 */

#include "Thread.h"

#include "Kernel.h"
#include "PCB.h"
#include "List.h"
#include "SCHEDULE.h"
#include "Queue.h"

extern int syncPrintf(const char *format, ...);

Thread::Thread(StackSize stackSize,Time timeSlice) {//Svaku nit pravimo sa zadatim stackSize-om i timeSlice-om
	lock();

	myPCB=new PCB(this,stackSize, timeSlice);//Pravimo PCB za svaku nit

	//syncPrintf("Napravljen PCB niti\n");

	unlock();

}

Thread::~Thread() {
	lock();


	waitToComplete();//Pri svakom pozivu destruktora niti zovemo waitToComplete funkciju
	//Brisemo myPCB(za datu napravljenu nit) jer je dinamicka promenljiva
	User::list->remove(this->myPCB);
	delete myPCB;

	unlock();
}

void Thread::waitToComplete(){
	lock();

	if((this==User::idle)||(this->myPCB->state==PCB::OVER) /*||(this==User::starting)*/ || (this->myPCB==PCB::NEW) || (this->myPCB==PCB::running)){
		unlock();
		//syncPrintf("Usao u IF\n");
		return;
	}


	PCB::running->state=PCB::BLOCKED;
	myPCB->queueBlocked->put((PCB*)PCB::running);
	//syncPrintf("NIT KOJA CEKA: %d NA NIT: %d\n",PCB::running->id,this->myPCB->id);

	//syncPrintf("waitToComplete\n");

	unlock();
	dispatch();

}

void Thread::start(){
	lock();

	//syncPrintf("Usao u start metodu klase Thread\n");

	/*if(myPCB->state==PCB::READY){
		unlock();
		return;
	}*/
	this->myPCB->state=PCB::READY;//Stavljamo nit da je READY
	this->myPCB->createStack();//Pravimo joj pocetni stek
	Scheduler::put(myPCB);//Stavljamo je u Scheduler


	unlock();
}

void Thread::setPCB(){
	myPCB=this->myPCB;
}

PCB* Thread::getPCB(){
	return this->myPCB;
}

ID Thread::getId(){
	return this->myPCB->id;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id){
	return User::list->getById(id);
}

void dispatch(){
	lock();

	//syncPrintf("Usao u dispatch funkcije THREAD-a\n");
	User::contextOnDemand=1;//Preotimanje procesora
	User::timerIR();//Pozivanje prekidne rutine

	unlock();
}
