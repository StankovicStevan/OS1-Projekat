/*
 * Queue.h
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include "Thread.h"

class PCB;
class KernelSem;

class Queue {
public:
	Queue();
	virtual ~Queue();
	void put(PCB* pcb);
	PCB* get();
	int getCount();
	void update(KernelSem* sem);
	PCB* remove(PCB* pcb);
	//void ispis();

	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb, Elem* next): pcb(pcb), next(next){};
	};
	Elem* head;
	Elem* tail;
	int count;

private:
	friend class PCB;
	friend class User;
	friend class Thread;
	friend class KernelSem;
};

#endif /* QUEUE_H_ */
