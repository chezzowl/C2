#pragma once
#include <visa.h>
#include <visatype.h>
#include <iostream>
#include <assert.h>

//device name, if changed, alter here
#define ViString DEVICE = "TODO";
#define ViUInt32 BUFSIZE_DEFAULT = 1000;

void test() {
	std::cout << "Hello World" << std::endl;
}

//double* readwaveform(ViSession vi, long* elements) {
//	ViStatus	status;
//	float		yoffset, ymult;
//	ViChar		buffer[256];
//	ViChar		c;
//	long		count, i;
//	double* ptr = NULL;
//	unsigned char		szbuff[256] = { 0, };
//
//	assert(elements != NULL);
//
//	status = viSetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
//	status = viSetAttribute(vi, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
//
//	//todo: get buffer size
//	//vigetattribute(vi,)
//
//	// turn headers off, this makes parsing easier
//	status = viPrintf(vi, (ViString)"header off\n");
//	if (status < VI_SUCCESS) goto error;
//
//	// hor:reco? asks for the record length
//	status = viVQueryf(vi, "hor:reco?\n", "%ld", elements);
//	if (status < VI_SUCCESS) goto error;
//
//	// make sure start, stop values for curve query match the full record length
//	//--> data reading starts at one, finishes at record length
//	//todo: why is pointer passed here? maybe look into function declaration if possible?!
//	status = viPrintf(vi, "data:start 1;data:stop %d\n", *elements);
//	if (status < VI_SUCCESS) goto error;
//
//	// get the yoffset to help calculate the vertical values.
//	//yoffset passed by reference, meaning that changes to the variable inside the function 
//	//are passed through to the actual argument!
//	status = viQueryf(vi, (vistring)"wfmoutpre:yoff?\n", (vistring)"%f", &yoffset);
//	if (status < vi_success) goto error;
//
//	// get the ymult to help calculate the vertical values.
//	status = viqueryf(vi, (vistring)"wfmoutpre:ymult?\n", (vistring)"%f", &ymult);
//	if (status < vi_success) goto error;
//
//	// request 8bit binary data on the curve query
//	status = viprintf(vi, (vistring)"data:encdg ribinary;width 1\n");
//	if (status < vi_success) goto error;
//
//	// request the curve
//	status = viprintf(vi, (vistring)"curve?\n");
//	if (status < vi_success) goto error;
//
//	// always flush if a viscanf follows a viprintf or vibufwrite.
//	status = viflush(vi, vi_write_buf | vi_read_buf_discard);
//	if (status < vi_success) goto error;
//
//	// get first char and validate
//	status = visetattribute(vi, vi_attr_rd_buf_oper_mode, vi_flush_disable);
//	status = viscanf(vi, (vistring)"%c", &c);
//	if (status < vi_success) goto error;
//	assert(c == '#');
//
//	// get width of element field.
//	status = viscanf(vi, (vistring)"%c", &c);
//	if (status < vi_success) goto error;
//	assert(c >= '0' && c <= '9');
//
//	// read element characters
//	count = c - '0';
//	for (i = 0; i < count; i++) {
//		status = viscanf(vi, (vistring)"%c", &c);
//		if (status < vi_success) goto error;
//		assert(c >= '0' && c <= '9');
//	}
//
//	// read waveform into allocated storage
//	ptr = (double*)malloc(*elements * sizeof(double));
//
//	for (i = 0; i < *elements; i++) {
//		status = viscanf(vi, (vistring)"%c", &c);
//		if (status < vi_success) goto error;
//		ptr[i] = (((double)c) - yoffset) * ymult;
//	}
//
//	status = viflush(vi, vi_write_buf | vi_read_buf_discard);
//	if (status < vi_success) goto error;
//
//	return ptr;
//
//error:
//	// report error and clean up
//	vistatusdesc(vi, status, buffer);
//	fprintf(stderr, "failure: %s\n", buffer);
//	if (ptr != null) free(ptr);
//	return null;
//}