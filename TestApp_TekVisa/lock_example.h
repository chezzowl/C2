#pragma once
#include <stdio.h>
#include <iostream>
#include <visa.h>

int lock(ViSession rm){

	static char string[256];
	ViUInt32 retCnt;

	for (int i = 1; i < 100; i++) {
		viLock(rm, VI_EXCLUSIVE_LOCK, VI_TMO_INFINITE, NULL, NULL);
		if (viWrite(rm, (ViBuf) "ch1:scale?", 10, &retCnt) < VI_SUCCESS) return 0;
		if (viRead(rm, (ViBuf)string, 256, &retCnt) < VI_SUCCESS) return 0;

		printf("%d: scale %s", i, string);

		viUnlock(rm);
	}
}