/*
 * Starting.h
 *
 *  Created on: Jul 25, 2020
 *      Author: OS1
 */

#ifndef STARTING_H_
#define STARTING_H_
#include "PCB.h"
#include "Thread.h"

class Thread;
class PCB;

class Starting:public Thread {
public:
	Starting();
	void startingPCB();
private:
	friend class PCB;
	friend class Thread;
	friend class User;
};

#endif /* STARTING_H_ */
