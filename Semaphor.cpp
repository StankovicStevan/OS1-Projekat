/*
 * Semaphor.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#include "Semaphor.h"

#include "Kernel.h"
#include "KernelSe.h"


extern int syncPrintf(const char *format, ...);

Semaphore::Semaphore(int init) {
	lock();
	//syncPrintf("POCETAK KONSTRUKTORA SEMAPHORE\n");
	myImpl = new KernelSem(init);
	//syncPrintf("Napravljen semafor\n");

	unlock();
}

Semaphore::~Semaphore() {
	lock();

	delete myImpl;

	unlock();
}

int Semaphore::wait(Time time){
	lock();

	int res=myImpl->wait(time);
	//syncPrintf("Povratna vrednost wait-a je %d\n",res);

	unlock();
	return res;
}

int Semaphore::signal(int n){
	lock();

	int res=myImpl->signal(n);
	//syncPrintf("Povratna vrednost signal-a je %d\n",res);

	unlock();
	return res;
}

int Semaphore::val() const{
	lock();

	int res=myImpl->val();

	unlock();
	return res;
}

