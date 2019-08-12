#include "WaveFormReader.h"
#include "Communication.h"
#include <assert.h>
#include <iostream>

//BS_branch
//initial values, set at compiling time
float WaveFormReader::y_offset = 0;
float WaveFormReader::y_mult = 1;

/*******WaveFormReader functions******/

//void WaveFormReader::initParams()
//{
//	//const float WaveFormReader::YOFF_DEFAULT = 5;
//	//const float WaveFormReader::YMULT_DEFAULT = 17;
//	/*WaveFormReader::y_offset = WaveFormReader::YOFF_DEFAULT;
//	WaveFormReader::y_mult = WaveFormReader::YMULT_DEFAULT;*/
//	WaveFormReader::y_offset = 0;
//	WaveFormReader::y_mult = 1;
//}

/*
TODOS:
- how to print ViChars
- file saving
- move some reading logic to session.cpp if necessary / convenient
*/
double* WaveFormReader::readWave(ViSession& session, long* elems_ptr)
{
	//status variable
	ViStatus status;
	ViChar buffer[256]; //TODO: probably to be removed
	char c;
	long count, i;
	double*		ptr = NULL; //TODO: move to Session.h
	//variable to track error occurance
	int err_idx = -5;

	assert(elems_ptr != NULL);

	// Turn headers off, this makes parsing easier
	status = viPrintf(session, (ViString)"header off\n");
	if (status < VI_SUCCESS) 
	{
		goto error;
	}
	std::cout << "header turned off" << std::endl;

	// hor:reco? asks for the record length, saving it to elements
	status = viQueryf(session, (ViString)"hor:reco?\n", (ViString)"%ld", elems_ptr);
	if (status < VI_SUCCESS)
	{
		goto error;
	}

	std::cout << "record length: " << *elems_ptr << std::endl;

	// Make sure start, stop values for curve query match the full record length
	//--> data reading starts at one, finishes at record length
	//TODO: make sure, dereferencing the element pointer is legit
	status = viPrintf(session, (ViString)"data:start 1;data:stop %d\n", *elems_ptr);
	if (status < VI_SUCCESS)
	{
		goto error;
	}

	
	//return ptr;

	//// Get the yoffset to help calculate the vertical values.
	////yoffset passed by reference, meaning that changes to the variable inside the function 
	////are passed through to the actual argument!
	//status = viQueryf(session, (ViString)"WFMOutpre:YOFF?\n", (ViString)"%f", &WaveFormReader::y_offset);
	//if (status < VI_SUCCESS) goto error;

	//// Get the ymult to help calculate the vertical values.
	//status = viQueryf(session, (ViString)"WFMOutpre:YMULT?\n", (ViString)"%f", &WaveFormReader::y_mult);
	//if (status < VI_SUCCESS) goto error;

	std::cout << "y offset: " << WaveFormReader::y_offset << std::endl;
	std::cout << "y multiplication factor: " << WaveFormReader::y_mult << std::endl;

	// Request 8bit binary data on the curve query
	//TODO: move to Session.cpp
	status = viPrintf(session, (ViString)"DATA:ENCDG RIBINARY;WIDTH 1\n");
	if (status < VI_SUCCESS) goto error;

	// Request the curve
	//TODO: move to Session.cpp
	status = viPrintf(session, (ViString)"CURVE?\n");
	if (status < VI_SUCCESS) goto error;
	std::cout << "curve loaded" << std::endl;

	// Always flush if a viScanf follows a viPrintf or viBufWrite.
	status = viFlush(session, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
	if (status < VI_SUCCESS) goto error;
	std::cout << "necessary flush after loading curve" << std::endl;

	// Get first char and validate
	status = viSetAttribute(session, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_DISABLE);
	status = viScanf(session, (ViString)"%c", &c);
	if (status < VI_SUCCESS) goto error;
	assert(c == '#');

	//// Get width of element field.
	status = viScanf(session, (ViString)"%c", &c);
	if (status < VI_SUCCESS) goto error;
	//assert(c >= '0' && c <= '9');

	//// Read element characters
	count = c - '0'; //TODO: WHY -23?? is that normal behaviour??
	for (i = 0; i < count; i++) {
		status = viScanf(session, (ViString)"%c", &c);
		if (status < VI_SUCCESS) goto error;
		assert(c >= '0' && c <= '9');
	}

	//// Read waveform into allocated storage
	////TODO: see if this causes problems!
	ptr = (double*)malloc(* elems_ptr * sizeof(double));

	for (i = 0; i < 205; i++) {
		status = viScanf(session, (ViString)"%c", &c);
		if (status < VI_SUCCESS)
		{
			err_idx = i;
			//goto error;
		}
		ptr[i] = (((double)c) - WaveFormReader::y_offset) * WaveFormReader::y_mult;
	}

	//status = viFlush(session, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
	//if (status < VI_SUCCESS) goto error;

	return ptr;

	error:
		// Report error and clean up
		std::cout << "ERROR at index: " << err_idx << std::endl;
		viStatusDesc(session, status, buffer);
		fprintf(stderr, "failure: %s\n", buffer);
		if (ptr != NULL) free(ptr);
		return NULL;

	/*while (true)
	{

	}*/
}

//double* WaveFormReader::readWave(ViSession& session, long* elems_ptr)
//{
//	double*		ptr = NULL;
//	ViStatus status;
//	ViUInt32 retCount;
//	//status = viReadToFile(session, “curve.bin”, 20, &retCount);
//	status = viReadToFile(session, (ViString)"curve.bin", 20, &retCount);
//	
//	return ptr;
//}