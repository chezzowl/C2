/*holds the main function logic*/
#include <stdio.h>
#include <iostream>
#include "session.h"
#include "Communication.h"
#include "WaveFormReader.h"
#include <string>
#include <string.h>
#include <visa.h>
#include "testing.h"

using namespace std;

//MAIN FUNCTION
int main(int argc, char* argv[])
{
	//RedirectIOToConsole(); //see, if this works

	ViStatus status; //helpful status variable
	ViChar errorBuf[256]; //error buffer
	ViChar buffer[256];
	long elements; //TODO: maybe get rid of this instantiation
	double* wave = NULL;

	/**************************************************************************************/
	//ViStatus status;
	//ViChar buffer[256]; //TODO: probably to be removed
	char c;
	long count, i;
	double *ptr = NULL; //TODO: move to Session.h

	/**************************************************************************************/

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
	
	cout << "Printing buffer : ";
	for (int i = 0; i < 256; i++) {
		cout << buffer[i];

		if (i == 255) {
			cout << endl;
		}
	}


	/**************************************************************************************/
	//// Turn headers off, this makes parsing easier
	//status = viPrintf(vi, (ViString)"header off\n");
	//if (status < VI_SUCCESS) goto error;

	/**************************************************************************************/

	wave = WaveFormReader::readWave(vi, &elements);

	//print values
	if (wave != NULL)
	{
		for (int i = 0; i < elements; i++){
			cout << i << " - ter Eintrag: " << wave[i] << endl;
		}
	}else{
		cout << "WAVE IS NULL" << endl;
	}

		error:
		cout << "ERROR IN MAIN" << endl;
		// Report error and clean up
		viStatusDesc(vi, status, errorBuf);
		fprintf(stderr, "MAIN failure: %s\n", errorBuf);
		//if (ptr != NULL) free(ptr);
		//return NULL;
		
	return 0;
}