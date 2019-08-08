/*holds the main function logic*/
#include <stdio.h>
#include <iostream>
#include "session.h"
#include "Communication.h"
#include "WaveFormReader.h"
#include <string>

//MAIN FUNCTION
int main(int argc, char* argv[])
{
	//RedirectIOToConsole(); //see, if this works

	ViStatus status; //helpful status variable
	ViChar errorBuf[256]; //error buffer
	char buffer[256];
	long elements; //TODO: maybe get rid of this instantiation
	double* wave = NULL;
	std::string s;

	/**************************************************************************************/
	//ViStatus status;
	//ViChar buffer[256]; //TODO: probably to be removed
	char c;
	long count, i;
	double*		ptr = NULL; //TODO: move to Session.h

	/**************************************************************************************/

	std::cout << "TEST PRINT" << std::endl;

	// initialize sessions
	status = startRMSession(rm);
	if (status < VI_SUCCESS) goto error;

	status = startVISession(rm, vi);//using DEFAULT_DEVICE!
	if (status < VI_SUCCESS) goto error;

	status = viGetAttribute(vi, VI_ATTR_RSRC_MANF_NAME, (ViPAttrState)buffer);
	printf("Manufacturer: %s\n", buffer);

	//flushing necessary every time
	status = readBuf_flushing(vi, true);
	if (status < VI_SUCCESS) goto error;

	status = writeBuf_flushing(vi, true);
	if (status < VI_SUCCESS) goto error;

	//check the buffers
	//status = viGetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, (ViPAttrState)buffer);
	//printf("READ BUFFER MODE: %s\n", buffer);

	//print buffer size
	status = viGetAttribute(vi, VI_ATTR_RD_BUF_SIZE, buffer);
	s = buffer;
	printf("buffer size: %s\n", s);

	/**************************************************************************************/
	//// Turn headers off, this makes parsing easier
	//status = viPrintf(vi, (ViString)"header off\n");
	//if (status < VI_SUCCESS) goto error;

	/**************************************************************************************/

	wave = WaveFormReader::readWave(vi, &elements);

	//print values
	if (wave != NULL)
	{
		for (int i = 0; i < elements; i++)
		{
			std::cout << i << " - ter Eintrag: " << wave[i] << std::endl;
		}
	}

	else
	{
		std::cout << "WAVE IS NULL" << std::endl;
	}

	while (true)
	{

	}


	error:
		std::cout << "ERROR IN MAIN" << std::endl;
		// Report error and clean up
		viStatusDesc(vi, status, errorBuf);
		fprintf(stderr, "MAIN failure: %s\n", errorBuf);
		//if (ptr != NULL) free(ptr);
		//return NULL;
		while (true)
		{

		}
}