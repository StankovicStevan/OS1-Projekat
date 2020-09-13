/*
 * List.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#include "List.h"

#include "Kernel.h"
#include "PCB.h"
#include "Thread.h"

extern int syncPrintf(const char *format, ...);

List::List() {//RADI
	first=0;
	//syncPrintf("Radi stvaranje liste\n");
}

List::~List() {
	lock();

	Elem* tek;//Pomocni pokazivac kojim idemo kroz listu
	tek=first;

	while(tek){
		Elem* pred;//Pomocni pokazivac pomocu kojeg cemo da brisemo element
		pred=tek;
		tek=tek->next;
		delete pred;
	}

	first=0;

	unlock();
}

void List::add(PCB* pcb){//RADI
	lock();

	Elem* tek=new Elem(pcb, 0);
	Elem* end;
	end=first;
	if(first){//Ako postoji prvi
		while(end->next){//Help postavljamo na poslednji
			end=end->next;
		}
		end->next=tek;
	}
	else{//Ako ne postoji prvi, first=tek
		first=tek;
	}

	//syncPrintf("Nit %d dodata u listu svih niti",pcb->id);

	unlock();
}

void List::remove(PCB* pcb){//Mislim da mi ne treba
	lock();
	Elem* tek;
	tek=first;
	Elem* pred=0;
	while(tek){
		if(tek->pcb!=pcb){//Ako nismo nasli datu nit, u prvoj iteraciji prethodni stavljamo na prvi, a tek pomeramo na sledeci
			pred=tek;
			tek=tek->next;
		}
		else{//U tekucem se nalazi data nit
			Elem* old=tek;//Pomocni element pomocu kome brisemo
			tek=tek->next;//Tekuci pomeramo na sledeci
			if(!pred){//Ako ne postoji prethodni, znaci da imamo samo prvi u listi
				first=tek;//Prvi stavljamo na null
			}
			else{
				pred->next=tek;
			}
			delete old;

		}
	}

	unlock();

}

Thread* List::getById(ID id){
	lock();
	Elem* tek=first;

	while(tek){
		if(tek->pcb->id==id){
			unlock();
			return tek->pcb->thread;
		}
		tek=tek->next;
	}

	unlock();
	return 0;
}

