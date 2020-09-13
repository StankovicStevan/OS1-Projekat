/*
 * List.h
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
#include "PCB.h"

typedef int ID;

class PCB;
class Thread;
class User;

class List {
public:
	List();
	virtual ~List();
	void add(PCB* pcb);
	void remove(PCB* pcb);
	Thread* getById(ID id);

	struct Elem{//Element liste, ima polja pcb(jer ce biti lista u koju smestamo niti) i next-pokazivac na sledeci
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb, Elem* next): pcb(pcb), next(next){};
	};
	Elem* first;

private:
	friend class PCB;
	friend class Thread;
	friend class User;
};

#endif /* LIST_H_ */
