/*
 * SemList.h
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "KernelSe.h"
//#include "User.h"

//class User;
//class KernelSem;

class SemList {
public:
	SemList();
	virtual ~SemList();
	void add(KernelSem* sem);
	void remove(KernelSem* sem);
	void timerSem();
	int getCount();

	struct Elem{
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem* sem, Elem* next): sem(sem), next(next){};
	};

	Elem* first;
	int count;

	private:
		friend class KernelSem;
		friend class User;
};

#endif /* SEMLIST_H_ */
