#pragma once
#include <visa.h>
#include <visatype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int open_session(void)
{
	int i;
	ViUInt32 retCount;
	ViSession defaultRM, instr;
	ViUInt32 numInstrs;
	ViFindList findList;
	ViUInt32 writeCount;
	ViStatus status;
	char instrResourceString[VI_FIND_BUFLEN];
	static unsigned char buffer[100];
	static char stringinput[512];


	/*
	* First we must call viOpenDefaultRM to get the manager
	* handle.  We will store this handle in defaultRM.
	*/
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("Could not open a session to the VISA Resource Manager!\n");
		exit(EXIT_FAILURE);
	}

	/* Find all the USB TMC VISA resources in our system and store the  */
	/* number of resources in the system in numInstrs.                  */
	status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);

	if (status < VI_SUCCESS) {
		printf("An error occurred while finding resources.\nHit enter to continue.");
		fflush(stdin);
		getchar();
		viClose(defaultRM);
		return status;
	}

	/*
	 * Now we will open VISA sessions to all USB TMC instruments.
	 * We must use the handle from viOpenDefaultRM and we must
	 * also use a string that indicates which instrument to open.  This
	 * is called the instrument descriptor.  The format for this string
	 * can be found in the function panel by right clicking on the
	 * descriptor parameter. After opening a session to the
	 * device, we will get a handle to the instrument which we
	 * will use in later VISA functions.  The AccessMode and Timeout
	 * parameters in this function are reserved for future
	 * functionality.  These two parameters are given the value VI_NULL.
	 */

	for (i = 0; i < numInstrs; i++)
	{
		if (i > 0) viFindNext(findList, instrResourceString);
			//printf("%d", instrResourceString);

		status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);

		if (status < VI_SUCCESS){
			printf("Cannot open a session to the device %d.\n", i + 1);
			continue;
		}

		/*
		 * At this point we now have a session open to the USB TMC instrument.
		 * We will now use the viWrite function to send the device the string "*IDN?\n",
		 * asking for the device's identification.
		 */
		strcpy(stringinput, "*IDN?\n");
		status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
		if (status < VI_SUCCESS)
		{
			printf("Error writing to the device %d.\n", i + 1);
			status = viClose(instr);
			continue;
		}

		/*
		* Now we will attempt to read back a response from the device to
		* the identification query that was sent.  We will use the viRead
		* function to acquire the data.  We will try to read back 100 bytes.
		* This function will stop reading if it finds the termination character
		* before it reads 100 bytes.
		* After the data has been read the response is displayed.
		*/
		status = viRead(instr, buffer, 100, &retCount);
		if (status < VI_SUCCESS) {
			printf("Error reading a response from the device %d.\n", i + 1);
		}
		else {
			printf("\nDevice %d: %*s\n", i + 1, retCount, buffer);
		}
		status = viClose(instr);
	}


	/*
	 * Now we will close the session to the instrument using
	 * viClose. This operation frees all system resources.
	 */
	 /*status = viClose(defaultRM);
	 printf("Hit enter to continue.");
	 fflush(stdin);
	 getchar();*/

	return 0;
}