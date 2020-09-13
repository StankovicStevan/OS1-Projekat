/*
 * User.cpp
 *
 *  Created on: Jul 22, 2020
 *      Author: OS1
 */

#include "Kernel.h"

#include "Starting.h"
#include "Idle.h"
#include "PCB.h"
#include "List.h"
#include "Schedule.h"
#include "KernelSe.h"
#include "SemList.h"
#include "IVTEntry.h"
#include <dos.h>


extern void tick();
extern int syncPrintf(const char *format, ...);

volatile int User::contextOnDemand=0;
Starting* User::starting=0;
Idle* User::idle=0;
volatile List* User::list=0;
volatile Time counter=9;
volatile unsigned tsp, tss;



void User::inic(){//Izbaciti sve inicijalizacije promenljivih
	lock();

	list=new List();
	idle=new Idle();//Pravimo idle nit
	idle->start();
	starting=new Starting();//Pravimo pocetnu nit
	starting->startingPCB();

	//syncPrintf("Id starting niti je %d\n",starting->myPCB->id);

	PCB::running=starting->myPCB;//Nju stavimo da bude running
	PCB::running->state=PCB::ACTIVE;

	//syncPrintf("Id running niti je %d\n",PCB::running->id);

	//Ukljucimo je da bude aktivna
	//syncPrintf("Id idle niti je %d\n",idle->myPCB->id);


	//semaphores=new SemList();
	//syncPrintf("Inicijalni broj semafora je %d\n",User::semaphores->getCount());

	oldISR = getvect(0x1C);//Dohvatimo staru prekidnu rutinu sa adrese 1C
	setvect(0x1C, timerIR);//Na tu adresu postavimo nasu novu prekidnu rutinu timerIR
	setvect(0x60,oldISR);

	unlock();
}

void User::restore(){
	lock();

	/*if(PCB::running!=starting->myPCB){
		unlock();
		return;
	}*/

	setvect(0x1C, oldISR);
	delete starting;
	delete idle;
	delete KernelSem::semaphores1;
	delete[] IVTEntry::IVTEntryTable;
	delete list;
	//delete semaphores;
	unlock();
}

void interrupt User::timerIR(...){
	//syncPrintf("Usao u metodu timerIR, aktivna nit:%d\n",PCB::running->id);
	if(!contextOnDemand){
		tick();
#ifndef BCC_BLOCK_IGNORE
		asm int 0x60;//Poziv stare IR poziva se samo kada nije zahtevana promena konteksta, tako da se stara IR poziva samo kad je stvarno doslo do prekida
#endif

		//counter--;
	}
	if(!contextOnDemand){//Izmeniti
		//syncPrintf("Usao u if u TAJMERU\n");
		//syncPrintf("Broj semafora u tajmeru je %d\n",semaphores->getCount());
		//semaphores->timerSem();
		KernelSem::semaphores1->timerSem();
	}

	if((PCB::running!=idle->myPCB)&&(PCB::running->quantum==0)&&(contextOnDemand==0)){
		return; //Ako je quantum niti 0, tada nit traje beskonacno, osim ako ne dodje do eksplicitnog poziva prekidne rutine, tj dispatch
	}
	if(!contextOnDemand){
		//syncPrintf("Smanjujemo brojac\n");
		counter--;
	}
	if(counter==0 || contextOnDemand || PCB::running==idle->myPCB){
		//syncPrintf("Usao u PROMENU KONTEKSTA\n");
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
			}
#endif

		PCB::running->sp=tsp;
		PCB::running->ss=tss;
		//syncPrintf("Running id:%d, running stanje:%d\n",PCB::running->id,PCB::running->state);
		if((PCB::running->state==PCB::ACTIVE) && (PCB::running!=idle->myPCB) && (PCB::running->state!=PCB::IDLE)) {
			PCB::running->state=PCB::READY;
			//syncPrintf("Stavljamo nit %d u Scheduler u Timer-u\n",PCB::running->id);
			Scheduler::put((PCB*)PCB::running);
		}
		PCB::running=Scheduler::get();//Dohvatanje nove niti
		//syncPrintf("Novi running ima id niti %d\n",PCB::running->id);

		if(PCB::running==0){
			PCB::running=idle->myPCB;
			PCB::running->state=PCB::IDLE;
			//syncPrintf("Procesor je dobila idle nit\n");
		}
		else{
		PCB::running->state=PCB::ACTIVE;
		}

		tsp=PCB::running->sp;
		tss=PCB::running->ss;

#ifndef BCC_BLOCK_IGNORE
		asm{//Restauiramo sp i ss
			mov sp, tsp
			mov ss, tss
		}
#endif
		counter=PCB::running->quantum;
	}
	contextOnDemand=0;
}


