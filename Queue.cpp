/*
 * Queue.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#include "Queue.h"

#include "Kernel.h"
#include "PCB.h"
//#include "KernelSe.h"
#include "SCHEDULE.h"

extern int syncPrintf(const char *format, ...);

Queue::Queue() {
	head=tail=0;
	count=0;
}

Queue::~Queue() {
	lock();

	Elem* tek=head;
	while(tek){
		Elem* pred;
		pred=tek;
		tek=tek->next;
		delete pred;
	}
	head=tail=0;
	//head->next=0;
	//tail->next=0;
	count=0;

	unlock();
}

void Queue::put(PCB* pcb){
	lock();

	Elem* pred=new Elem(pcb, 0);
	if(head==0){
		head=pred;
		tail=pred;
	}
	else{
		tail->next=pred;
		tail=pred;
	}
	count++;

	unlock();
}

PCB* Queue::get(){
	lock();

	PCB* back;
	if(head==0){
		unlock();
		return 0;
	}
	else{
		Elem* pred=head;
		head=head->next;
		back=pred->pcb;
		delete pred;
		if(head==0)tail=0;
		count--;
	}

	unlock();
	return back;
}

int Queue::getCount(){
	return count;
}

PCB* Queue::remove(PCB* pcb){

	lock();
	if(head==0)return 0;
	Elem* tek=head;
	Elem* pred=0;
	while(tek){
		if(tek->pcb==pcb){
			if(!pred){
				head=head->next;
				if(head==0)tail=0;
			}
			else if(tek->next==0){
				pred->next=0;
				tail=pred;
			}
			else{
				pred->next=tek->next;
			}
			count--;
			delete pred;
			break;
		}
		else{
			pred=tek;
			tek=tek->next;
		}
	}
	unlock();
	return pcb;
}

void Queue::update(KernelSem* sem){
	lock();

	Elem* tek=head;
	/*if(tek==0){
			//syncPrintf("PROBLEM");
			unlock();
			return;
		}*/
	//syncPrintf("Usao u update funkciju\n");
	//syncPrintf("Broj elemenata reda blokiranih je %d\n",sem->semaphoreBlocked->getCount());

	while(tek){
			if(tek->pcb->semaphoreWait>0){
			tek->pcb->semaphoreWait--;
			//syncPrintf("SemaphoreWait=%d\n",tek->pcb->semaphoreWait);
				if(tek->pcb->semaphoreWait==0){
					PCB*pcb=sem->semaphoreBlocked->remove(tek->pcb);
					//syncPrintf("Tr vr reda blokiranih %d",sem->semaphoreBlocked->count);
					if(pcb){
						pcb->state=PCB::READY;
						//syncPrintf("Id niti je%d\n",tek->pcb->id);
						Scheduler::put(pcb);
						sem->value++;
						//syncPrintf("Vrednost semafora je%d\n",sem->value);
					}
				}
			}
		tek=tek->next;
	}
	if(tek==0){
		unlock();
		return;
	}

	unlock();
}

