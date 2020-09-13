/*
 * PCB.h
 *
 *  Created on: Jul 20, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include"thread.h"

class Thread;
//class List;
class User;
class Queue;



class PCB {
public:

	void createStack();
	static void wrapper();

	Thread* thread;
	StackSize stackSize;
	Time timeSlice;
	enum enumState {NEW=0, READY=1, BLOCKED=2, ACTIVE=3, OVER=4, IDLE=5};
	volatile enumState state;
	unsigned* stackPointer;
	int quantum;
	int id;
	Queue* queueBlocked;
	Time semaphoreWait;

	PCB(Thread *thread, StackSize stackSize, Time timeSlice);
	~PCB();

	unsigned int sp, ss;

private:

	friend class Thread;
	friend class List;
	friend class User;
	friend class Starting;
	friend class Idle;
	friend class Queue;
	friend class KernelSem;
	friend class KernelEv;

	static volatile PCB* running;
	static ID recentId;
};

#endif /* PCB_H_ */
