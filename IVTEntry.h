/*
 * IVTEntry.h
 *
 *  Created on: Sep 7, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "KernelEv.h"

typedef void interrupt(*pInterrupt)(...);
const int entriesNum=256;

class IVTEntry {
public:
	void signal();
	void callOldIR();
	void restore(IVTNo ivtNo, pInterrupt oldIR);
	void init(IVTNo ivtNo, pInterrupt newIR);


	IVTEntry(IVTNo ivtNo, pInterrupt newIR);
	virtual ~IVTEntry();

	static IVTEntry* IVTEntryTable[256];
	pInterrupt oldIR;

private:
	KernelEv* event;

	IVTNo ivtNo;
	friend class KernelEv;
};

#endif /* IVTENTRY_H_ */
