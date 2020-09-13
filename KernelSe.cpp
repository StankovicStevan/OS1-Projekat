/*
 * KernelSe.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#include "KernelSe.h"

#include "Kernel.h"
//#include "Semaphor.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "SemList.h"


extern int syncPrintf(const char *format, ...);

SemList* KernelSem::semaphores1=new SemList();

KernelSem::KernelSem(int init) {//Dodati u listu semafora
	lock();

	//syncPrintf("Broj elemenata semafora pre dodavanja je %d\n",User::semaphores->getCount());
	//this->sem=sem;
	this->value=init;
	this->semaphoreBlocked= new Queue();
	//User::semaphores->add(this);
	semaphores1->add(this);
	//semaphores->add(this);
	//syncPrintf("Dodat trenutni semafor i broj semafora je %d\n",User::semaphores->getCount());

	unlock();
}

KernelSem::~KernelSem() {
	lock();

	delete semaphoreBlocked;

	unlock();
}

int KernelSem::signal(int n){//RADI
	lock();
	//syncPrintf("Usao u signal SA VREDNOSCU %d",n);
	if(n==0){
		//syncPrintf("Usao u if za N=0\n");
		this->value++;
		if(this->value<=0){
			PCB* pcb;
			pcb=semaphoreBlocked->get();
			if(pcb){
			pcb->state=PCB::READY;
			Scheduler::put(pcb);
			}
		}
	}
	if(n>0){
		//syncPrintf("Usao u if za N>0\n");
		for(int i=0; i<n; i++){
			if(semaphoreBlocked->get()==0){
				n=i;
				//syncPrintf("Izlazimo za N=%d",n);
				break;
			}
			if(this->value<=0){
				//syncPrintf("Value je %d\n",this->value);
				PCB* pcb;
				pcb=semaphoreBlocked->get();
				pcb->state=PCB::READY;
				Scheduler::put(pcb);
			}
		}
		this->value+=n;
	}
	//syncPrintf("Vrednost signala je %d\n",n);

	unlock();
	return n;
}

int KernelSem::wait(Time time){//Svakom PCB-u smo dodali polje semaphoreWait koje incijalno postavljamo na -1,
	//ako nije postavljeno na -1 onda je blokirana na postavljeni TimeSlice
	lock();
	//syncPrintf("Usao u WAIT funkciju\n");
	int returnValue=0;
	this->value--;
	//syncPrintf("Vrednost semafora %d\n",this->value);
	if(this->value<0){
		if(time<0) {

			unlock();
			return -1;
		}
		if(time==0){//Ako je prosledjen argument 0, onda radimo standardni wait
			returnValue=dream();

			unlock();

			return returnValue;
		}
		if(time>0){//Ako je prosledjen argument razlicit od 0, onda blokiramo pozivajucu nit na TIME time slice-ova
			//syncPrintf("Usao u if za TIME>0\n");
			PCB::running->semaphoreWait=time;
			returnValue=dream();
			//syncPrintf("Izlaz iz if za TIME>0\n");
			unlock();

			return returnValue;
		}
	}
	//Ako je posle dekrementiranja vrednost semafora >=0 onda nista ne radimo, samo vracamo 1
	unlock();
	return 1;
}

int KernelSem::dream(){
	lock();
	PCB::running->state=PCB::BLOCKED;
	semaphoreBlocked->put((PCB*)PCB::running);
	//syncPrintf("Usao u dream funkciju, vrednost reda blokiranih je %d\n",semaphoreBlocked->getCount());
	unlock();

	dispatch();

	lock();

	//syncPrintf("Izasao iz dispatch-a");
	if(PCB::running->semaphoreWait==0){//Nit deblokirana zbog isteka vremena, wait vraca 0
		PCB::running->semaphoreWait=-1;
		unlock();
		return 0;
	}
	unlock();
	return 1;
}

int KernelSem::val() const{
	return this->value;
}

