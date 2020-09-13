/*
 * User.h
 *
 *  Created on: Jul 22, 2020
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "Thread.h"
#include "SemList.h"




#define lock(){asm{pushf;cli;}}
#define unlock(){asm{popf;}}

class PCB;
class Thread;
//class Starting;
//class Idle;
//class List;
class KernelSem;
class SemList;

typedef void interrupt(*pInterrupt)(...);
static pInterrupt oldISR;



class User {
public:
	static volatile int contextOnDemand;
	static void inic();
	static void restore();
	static void interrupt timerIR(...);
	static Starting* starting;
	static Idle* idle;
	//static SemList* semaphores;

//private:

	static volatile List* list;
	//static volatile unsigned tsp, tss;

	friend class PCB;
	friend class Thread;
	friend class Starting;
	friend class Idle;
	friend class List;
	friend class KernelSem;
	friend class SemList;

};

extern void tick();

#endif /* KERNEL_H_ */
