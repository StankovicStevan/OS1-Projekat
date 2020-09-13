/*
 * SemList.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#include "SemList.h"

#include "Kernel.h"
#include "SCHEDULE.h"

extern int syncPrintf(const char *format, ...);

SemList::SemList() {
	first=0;
	count=0;
}

SemList::~SemList() {
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
	count=0;

	unlock();
}

void SemList::add(KernelSem* sem){//RADI
	lock();
	//syncPrintf("Dodat semafor u listu sa pocetnom vrednoscu %d\n",count);
	Elem* tek=new Elem(sem, 0);
	Elem* end;
	end=first;
	if(first){//Ako postoji prvi
		while(end->next){//End postavljamo na poslednji
			end=end->next;
		}
		end->next=tek;
	}
	else{//Ako ne postoji prvi, first=tek
		first=tek;
	}
	count++;
	//syncPrintf("Trenutan broj elemenata u semaforu je %d\n",count);

	unlock();
}

void SemList::remove(KernelSem* sem){//Mislim da mi ne treba
	lock();
	Elem* tek;
	tek=first;
	Elem* pred=0;
	while(tek){
		if(tek->sem!=sem){//Ako nismo nasli datu nit, u prvoj iteraciji prethodni stavljamo na prvi, a tek pomeramo na sledeci
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
	count--;

	unlock();

}

void SemList::timerSem(){
	lock();
	//syncPrintf("Usao u timerSem funkciju\n");
	Elem* tek=KernelSem::semaphores1->first;
	if(tek==0){
		unlock();
		return;
	}
	//syncPrintf("Velicina liste je %d",KernelSem::semaphores1->count);
	//if(tek==0)syncPrintf("PROBLEM %d\n",User::semaphores->getCount());
	while(tek){//Prolazimo kroz listu semafora i za svaku nit za koju istekne vreme odblokiramo je na datom semaforu
		tek->sem->semaphoreBlocked->update(tek->sem);
		//syncPrintf("Pozvana update funkcija iz timerSem\n");

		//PROBLEM

		//syncPrintf("TEK JE SLEDECI\n");

		tek=tek->next;
	}

	unlock();
}

int SemList::getCount(){
	return this->count;
}
