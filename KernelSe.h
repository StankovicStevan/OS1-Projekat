/*
 * KernelSe.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef KERNELSE_H_
#define KERNELSE_H_

//#include "Semaphor.h"
#include "Queue.h"
//#include "User.h"

class Queue;
//class User;
class SemList;

class KernelSem {
public:
	KernelSem(int init=1);
	virtual ~KernelSem();

	int signal(int n);
	int wait(Time maxTimeToWait);
	int val() const;
	int dream();

	int value;
	Queue* semaphoreBlocked;
	static SemList* semaphores1;

//private:

	//Semaphore* sem;

	//friend class Semaphor;
	friend class Queue;
	friend class User;
	friend class SemList;
};

#endif /* KERNELSE_H_ */
