/*holds the main function logic*/
#include <stdio.h>
#include <iostream>
#include "session.h"
#include "Communication.h"
#include "WaveFormReader.h"
//#include "testing.h"
#include "lock_example.h"
#include "GetDevice.h"
#include <string>
#include <visa.h>
#include <assert.h>

ViSession flushingbuffer(ViSession rm){

	ViStatus status;
	std::cout << "flushing buffer..." << std::endl;
	status = viFlush(rm, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
	if (status < VI_SUCCESS) {
		printf("Error flushing buffer");
	}else{
		std::cout << "buffer flushing was successful." << std::endl;
	}
	return rm;
}

void closesession(ViSession vi, ViSession rm) {
	std::cout << "closing all sessions..." << std::endl;
	viClose(vi);
	viClose(rm);
}

double* WaveReader(ViSession rm, long* elemts, ViChar errorBuffer[256]) {

	ViStatus status;
	char stringinput[512];
	ViChar buffer[256];
	ViChar c;
	double* pointer;
	pointer = NULL;
	long count;
	ViUInt32 retCnt;

	//status = viSetAttribute(rm, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
	//status = viSetAttribute(rm, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);

	/*status = viSetAttribute(rm, VI_ATTR_TMO_VALUE, 5000);
	if (status < VI_SUCCESS) goto error;*/

	std::cout << "Reading waveform..." << std::endl;

	status = viPrintf(rm, "header off\n");
	if (status < VI_SUCCESS) goto error;

	status = viQueryf(rm, "hor:reco?\n", "%ld", elemts);
	if (status < VI_SUCCESS) goto error;

	assert(elemts != NULL);

	printf("number of elements : %ld\n", *elemts);

	status = viPrintf(rm, "data:start %d;data:stop %d", 0, (*elemts) - 1);
	if (status < VI_SUCCESS) goto error;

	/*float yoffset, ymult;

	status = viQueryf(rm, "wfm outpre:yoff?\n", "%f", &yoffset);
	if (status < VI_SUCCESS) goto error;

	printf("y-offset : %d", yoffset);

	status = viQueryf(rm, "WMFOUTPRE:YMULT?\n", "%ld", &ymult);
	if (status < VI_SUCCESS) goto error;

	printf("y-mult : %d", ymult);*/

	status = viPrintf(rm, "DATA:ENCDG RIBINARY;WIDTH 1\n");
	if (status < VI_SUCCESS) goto error;

	//strcpy(stringinput, "CURVE?\n");
	//status = viPrintf(rm, stringinput, (ViUInt32)strlen(stringinput), &retCnt);
	status = viPrintf(rm, "CURVE?\n");
	if (status < VI_SUCCESS) goto error;

	/*status = viFlush(rm, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
	if (status < VI_SUCCESS) goto error;*/

	flushingbuffer(rm);

	status = viScanf(rm, "%c", &c);
	if (status < VI_SUCCESS) goto error;

	//printf("c : %c\n", c);

	assert(c == '#');

	//printf("c : %c\n", c);

	status = viScanf(rm, "%c", &c);
	if (status < VI_SUCCESS) goto error;
	assert(c >= '0' && c <= '9');

	//printf("c output : %c\n", c);

	count =  c - '0';

	//printf("count : %ld\n", count);

	for (int i = 0; i < count; i++) {
		status = viScanf(rm, "%c", &c);
		if (status < VI_SUCCESS) goto error;
		assert(c >= '0' && c <= '9');
	}

	pointer = (double*)malloc(*elemts*sizeof(double));

	for (int i = 0; i < *elemts; i++) {
		status = viScanf(rm, "%c", &c);
		if (status < VI_SUCCESS) goto error;
		pointer[i] = (double)c;
	}

	flushingbuffer(rm);

	return pointer;

error:
	std::cout << "ERROR IN WAVE_FORM_READER" << std::endl;
	viStatusDesc(rm, status, errorBuffer);
	fprintf(stderr, "failure : %s\n", errorBuffer);
	if (pointer != NULL)free(pointer);
	return NULL;
}

//MAIN FUNCTION
int main(int argc, char* argv[])
{
	//RedirectIOToConsole(); //see, if this works

	ViStatus status; //helpful status variable
	ViChar errorBuf[256]; //error buffer
	ViUInt32 retCount, retCnt;
	static char stringinput[512];
	static unsigned char newbuffer[100];
	static unsigned char newbuffer2[1024];
	long elements; //TODO: maybe get rid of this instantiation
	double* wave = NULL;
	//std::string s;

	/**************************************************************************************/
	//ViStatus status;
	//ViChar buffer[256]; //TODO: probably to be removed
	//char c;
	long count;
	//long i;
	//double*	ptr = NULL; //TODO: move to Session.h
	/**************************************************************************************/

	// initialize sessions
	//status = startRMSession(rm);
	//if (status < VI_SUCCESS) goto error;

	//status = startVISession(rm, vi);//using DEFAULT_DEVICE!
	//if (status < VI_SUCCESS) goto error;

	//status = viGetAttribute(vi, VI_ATTR_RSRC_MANF_NAME, (ViPAttrState)buffer);
	//printf("Manufacturer: %s\n", buffer);

	//check the buffers
	//status = viGetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, (ViPAttrState)buffer);
	//printf("READ BUFFER MODE: %s\n", buffer);

	//print buffer size
	/*status = viGetAttribute(vi, VI_ATTR_RD_BUF_SIZE, buffer);
	s = buffer;
	printf("buffer size: %s\n", s);*/

	//Reading test
	//open_session();
	status = viOpenDefaultRM(&vi);
	status = viOpen(vi, "USB::0x0699::0x0364::c020219::INSTR", VI_NULL, VI_NULL, &rm);
	if (status < VI_SUCCESS) goto error;

	/*strcpy(stringinput, "*IDN?\n");
	status = viWrite(rm, (ViBuf) stringinput, (ViUInt32)strlen(stringinput), &retCount);
	if (status < VI_SUCCESS) goto error;

	status = viRead(rm, newbuffer, 100, &retCount);
	if (status < VI_SUCCESS) goto error;

	printf("device :  %*s\n", retCount , newbuffer);

	std::cout << "testing done" << std::endl;*/

	/*strcpy(stringinput, "curve?\n");
	status = viWrite(rm, (ViBuf) stringinput, (ViUInt32)strlen(stringinput), &retCount);
	if (status < VI_SUCCESS) goto error;
	   
	status = viRead(rm, newbuffer2, 100, &retCount);
	if (status < VI_SUCCESS) goto error;

	printf("curve data :  %*s\n", retCount, newbuffer2);*/
	//works too
	//std::cout << newbuffer2 << std::endl;

	/*static char c;

	status = viPrintf(rm, "CURVE?\n");
	if (status < VI_SUCCESS) goto error;

	flushingbuffer(rm);

	status = viScanf(rm, "%c", &c);
	if (status < VI_SUCCESS) goto error;

	printf("c : %c\n",c);

	assert(c == '#');

	printf("c : %c\n", c);

	status = viScanf(rm, "%c", &c);
	if (status < VI_SUCCESS) goto error;
	assert(c >= '0' && c <= '9' );

	count = c - '0';

	printf("count : %d\n", (int)count);

	for (int i = 0; i < (int)count;i++) {
		status = viScanf(rm, "%c", &c);
		if (status < VI_SUCCESS) goto error;
	}*/

	wave = WaveReader(rm, &elements, errorBuf);

	if (wave != NULL) {
		for (int i = 0; i < elements; i++) {
			printf("data %d : %f\n", i, wave[i]);
		}
	}

	/*status = viRead(rm, buffer, 100, &retCount);
	if (status < VI_SUCCESS) goto error;
	printf("\ndata :  %*s\n", retCount, buffer);*/

	   

	/**************************************************************************************/
	//// Turn headers off, this makes parsing easier
	/*status = viPrintf(vi, (ViString)"header off\n");
	if (status < VI_SUCCESS) goto error;
	*/
	/**************************************************************************************/

	/*wave = WaveFormReader::readWave(vi, &elements);*/

	//print values
	/*if (wave != NULL){
		for (int i = 0; i < elements; i++){
			std::cout << i << " - ter Eintrag: " << wave[i] << std::endl;
		}
	}else{
		std::cout << "WAVE IS NULL" << std::endl;
	}*/

	closesession(vi, rm);

	return 0;
	
	error:
		std::cout << "ERROR IN MAIN" << std::endl;
		// Report error and clean up
		viStatusDesc(vi, status, errorBuf);
		fprintf(stderr, "MAIN failure: %s\n", errorBuf);
		closesession(vi, rm);
		//if (ptr != NULL) free(ptr);
		//return NULL;
}