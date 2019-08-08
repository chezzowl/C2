//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <visa.h>
//#include <string>
//
//// This function reads the currently selected waveform and returns
//// it as an array of doubles.
//double* ReadWaveform(ViSession vi, long* elements) {
//	ViStatus	status;
//	float		yoffset, ymult;
//	ViChar		buffer[256];
//	ViChar		c;
//	long		count, i;
//	double*		ptr = NULL;
//	unsigned char		szBuff[256] = { 0, };
//
//	assert(elements != NULL);
//
//	status = viSetAttribute(vi, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
//	status = viSetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
//
//	//TODO: get buffer size
//	//viGetAttribute(vi,)
//
//	// Turn headers off, this makes parsing easier
//	status = viPrintf(vi, (ViString)"header off\n");
//	if (status < VI_SUCCESS) goto error;
//
//	// hor:reco? asks for the record length
//	status = viQueryf(vi, (ViString)"hor:reco?\n", (ViString)"%ld", elements);
//	if (status < VI_SUCCESS) goto error;
//
//	// Make sure start, stop values for curve query match the full record length
//	//--> data reading starts at one, finishes at record length
//	//TODO: why is pointer passed here? maybe look into function declaration if possible?!
//	status = viPrintf(vi, (ViString)"data:start 1;data:stop %d\n", *elements);
//	if (status < VI_SUCCESS) goto error;
//
//	// Get the yoffset to help calculate the vertical values.
//	//yoffset passed by reference, meaning that changes to the variable inside the function 
//	//are passed through to the actual argument!
//	status = viQueryf(vi, (ViString)"WFMOutpre:YOFF?\n", (ViString)"%f", &yoffset);
//	if (status < VI_SUCCESS) goto error;
//
//	// Get the ymult to help calculate the vertical values.
//	status = viQueryf(vi, (ViString)"WFMOutpre:YMULT?\n", (ViString)"%f", &ymult);
//	if (status < VI_SUCCESS) goto error;
//
//	// Request 8bit binary data on the curve query
//	status = viPrintf(vi, (ViString)"DATA:ENCDG RIBINARY;WIDTH 1\n");
//	if (status < VI_SUCCESS) goto error;
//
//	// Request the curve
//	status = viPrintf(vi, (ViString)"CURVE?\n");
//	if (status < VI_SUCCESS) goto error;
//
//	// Always flush if a viScanf follows a viPrintf or viBufWrite.
//	status = viFlush(vi, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
//	if (status < VI_SUCCESS) goto error;
//
//	// Get first char and validate
//	status = viSetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_DISABLE);
//	status = viScanf(vi, (ViString)"%c", &c);
//	if (status < VI_SUCCESS) goto error;
//	assert(c == '#');
//
//	// Get width of element field.
//	status = viScanf(vi, (ViString)"%c", &c);
//	if (status < VI_SUCCESS) goto error;
//	assert(c >= '0' && c <= '9');
//
//	// Read element characters
//	count = c - '0';
//	for (i = 0; i < count; i++) {
//		status = viScanf(vi, (ViString)"%c", &c);
//		if (status < VI_SUCCESS) goto error;
//		assert(c >= '0' && c <= '9');
//	}
//
//	// Read waveform into allocated storage
//	ptr = (double*)malloc(*elements * sizeof(double));
//
//	for (i = 0; i < *elements; i++) {
//		status = viScanf(vi, (ViString)"%c", &c);
//		if (status < VI_SUCCESS) goto error;
//		ptr[i] = (((double)c) - yoffset) * ymult;
//	}
//
//	status = viFlush(vi, VI_WRITE_BUF | VI_READ_BUF_DISCARD);
//	if (status < VI_SUCCESS) goto error;
//
//	return ptr;
//
//error:
//	// Report error and clean up
//	viStatusDesc(vi, status, buffer);
//	fprintf(stderr, "failure: %s\n", buffer);
//	if (ptr != NULL) free(ptr);
//	return NULL;
//}
//
//
//// This program reads a waveform from a Tektronix
//// TDS scope and writes the floating point values to 
//// stdout.
//int main(int argc, char* argv[])
//{
//	ViSession rm, vi;
//	ViChar buffer[256];
//	//status variable
//	ViStatus status;
//	//device name
//	ViString name = ViRsrc("USB::0x0699::0x0364::C020219::INSTR");
//
//	printf("DEVICE NAME: %s\n" , name);
//	status = viOpenDefaultRM(&rm);
//	if (status < VI_SUCCESS)
//	{
//		printf("DEFAULT RM FAILURE");
//		goto error;
//	}
//	
//
//	status = viOpen(rm, name, 0, 5000, &vi);
//	if (status < VI_SUCCESS) 
//	{
//		printf("VIOPEN FAILURE");
//		goto error;
//	}
//
//	// Get VISA Manufacturer Name
//	//status = viGetAttribute(vi, VI_ATTR_RSRC_MANF_NAME, (ViString)buffer); (ViPAttrState)
//	status = viGetAttribute(vi, VI_ATTR_RSRC_MANF_NAME, (ViPAttrState)buffer); 
//	if (status < VI_SUCCESS)
//	{
//		printf("GET ATTRIBUTE FAILURE");
//		goto error;
//	}
//	// Set Timeout to 5 seconds
//	//viSetAttribute(vi, VI_ATTR_TMO_VALUE, 5000);
//	//printf(“Manufacturer: %s\n”, buffer);
//
//	//printf("\nSUCCESS");
//	printf("Manufacturer name: %s\n\n FUCK YEEESSS", buffer);
//	viClose(rm);
//
//	while (1)
//	{
//
//	}
//error:
//	printf("\nTHICK!");
//	viStatusDesc(vi, status, buffer);
//	fprintf(stderr, "error reason: %s\n", buffer);
//	while (1)
//	{
//
//	}
////	ViSession	rm = VI_NULL, vi = VI_NULL;
////	ViStatus	status;
////	ViChar		buffer[256];
////	double*		wfm = NULL;
////	long		elements, i;
////
////	// Open a default Session
////	status = viOpenDefaultRM(&rm);
////	if (status < VI_SUCCESS)
////		printf("DEFAULT RM ERROR");
////		goto error;
////
////	// Open the gpib device at primary address 1, gpib board 8
////	//status = viOpen(rm, (ViString)"GPIB8::1::INSTR", VI_NULL, VI_NULL, &vi);
////	status = viOpen(rm, (ViString)"USB::0x0699::0x0364::C020219::INSTR", VI_NULL, VI_NULL, &vi);
////	if (status < VI_SUCCESS) goto error;
////
////	// Read waveform and write it to stdout
////	wfm = ReadWaveform(vi, &elements);
////	if (wfm != NULL) {
////		for (i = 0; i < elements; i++) {
////			printf("%f\n", wfm[i]);
////		}
////	}
////
////	// Clean up
////	if (wfm != NULL) free(wfm);
////	viClose(vi); // Not needed, but makes things a bit more understandable
////	viClose(rm);
////
////	return 0;
////
////error:
////	// Report error and clean up
////	viStatusDesc(vi, status, buffer);
////	fprintf(stderr, "failure: %s\n", buffer);
////	if (rm != VI_NULL) viClose(rm);
////	if (wfm != NULL) free(wfm);
////	
////
////	while (1)
////	{
////
////	}
////
////	return 1;
//}
