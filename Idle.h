/*
 * Idle.h
 *
 *  Created on: Jul 25, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.h"

class Idle: public Thread {
public:
	Idle();
	void run();
	void start();
private:
	friend class PCB;
	friend class User;
};

#endif /* IDLE_H_ */
