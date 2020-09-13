/*
 * Main.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: OS1
 */

#include "Kernel.h"
#include "Thread.h"

extern int userMain(int argc, char* argv[]);
extern int syncPrintf(const char *format, ...);

int main(int argc, char* argv[]){
	int result=0;
	User::inic();
	result=userMain(argc, argv);
	User::restore();
	//syncPrintf("Happy end :)\n");
	return result;
}
