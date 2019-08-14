#pragma once
#include <visa.h>
#include <visatype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//device name, if changed, alter here.. TODO
//#define ViString DEVICE = "TODO";
//#define ViUInt32 BUFSIZE_DEFAULT = 1000;

static ViSession defaultRM;
static ViSession instr;
static ViUInt32 numInstrs;
static ViFindList findList;
static ViUInt32 retCount;
static ViUInt32 writeCount;
static ViStatus status;
static char instrResourceString[VI_FIND_BUFLEN];
static ViChar c;
static double* ptr = NULL;

static unsigned char buffer[100];
static char stringinput[512];

double* test(long* elements) {
	int i, count;

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
		//return status;
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

	for (i = 0; i < numInstrs; i++){
		if (i > 0)
			viFindNext(findList, instrResourceString);

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
		//strcpy(stringinput, "DATA:ENCDG RIBINARY;WITH 1\n");
		status = viPrintf(instr, "DATA:ENCDG RIBINARY;WITH 1\n");
		if (status < VI_SUCCESS){
			printf("Error writing to the device %d.\n", i + 1);
			status = viClose(instr);
			continue;
		}

		//strcpy(stringinput, "CURVE?\n");
		status = viPrintf(instr, "CURVE?\n");
		if (status < VI_SUCCESS) {
			printf("Error writing to the device %d.\n", i + 1);
			status = viClose(instr);
			continue;
		}

		status = viFlush(instr, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
		if (status < VI_SUCCESS){
			printf("Error flushing buffer.");
			status = viClear(instr);
		}

		/*
		* Now we will attempt to read back a response from the device to
		* the identification query that was sent.  We will use the viRead
		* function to acquire the data.  We will try to read back 100 bytes.
		* This function will stop reading if it finds the termination character
		* before it reads 100 bytes.
		* After the data has been read the response is displayed.
		*/
		status = viScanf(instr,"%c", &c);
		if (status < VI_SUCCESS) {
			printf("Error scanning.");
			status = viClear(instr);
		}
		//assert(c=='#');

		// Get width of element field.
		status = viScanf(instr, "%c", &c);
		//if (status < VI_SUCCESS) goto error;
		assert(c >= '0' && c <= '9');

		// Read element characters
		count = c - '0'; //TODO: WHY -23?? is that normal behaviour??
		for (i = 0; i < count; i++) {
			status = viScanf(instr, "%c", &c);
			//if (status < VI_SUCCESS) goto error;
			assert(c >= '0' && c <= '9');
		}

		// Read waveform into allocated storage
		//TODO: see if this causes problems!
		ptr = (double*)malloc(*elements * sizeof(double));

		for (i = 0; i < 205; i++) {
			status = viScanf(instr, "%c", &c);
			if (status < VI_SUCCESS){
				printf("Error");
				//goto error;
			}
			ptr[i] = (double) c;
		}


		//status = viFlush(session, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
		//if (status < VI_SUCCESS) goto error;

		return ptr;
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