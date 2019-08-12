//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <visa.h>
//#include <string>
//
//// this function reads the currently selected waveform and returns
//// it as an array of doubles.
//double* readwaveform(visession vi, long* elements) {
//	vistatus	status;
//	float		yoffset, ymult;
//	vichar		buffer[256];
//	vichar		c;
//	long		count, i;
//	double*		ptr = null;
//	unsigned char		szbuff[256] = { 0, };
//
//	assert(elements != null);
//
//	status = visetattribute(vi, vi_attr_wr_buf_oper_mode, vi_flush_on_access);
//	status = visetattribute(vi, vi_attr_rd_buf_oper_mode, vi_flush_on_access);
//
//	//todo: get buffer size
//	//vigetattribute(vi,)
//
//	// turn headers off, this makes parsing easier
//	status = viprintf(vi, (vistring)"header off\n");
//	if (status < vi_success) goto error;
//
//	// hor:reco? asks for the record length
//	status = viqueryf(vi, (vistring)"hor:reco?\n", (vistring)"%ld", elements);
//	if (status < vi_success) goto error;
//
//	// make sure start, stop values for curve query match the full record length
//	//--> data reading starts at one, finishes at record length
//	//todo: why is pointer passed here? maybe look into function declaration if possible?!
//	status = viprintf(vi, (vistring)"data:start 1;data:stop %d\n", *elements);
//	if (status < vi_success) goto error;
//
//	// get the yoffset to help calculate the vertical values.
//	//yoffset passed by reference, meaning that changes to the variable inside the function 
//	//are passed through to the actual argument!
//	status = viqueryf(vi, (vistring)"wfmoutpre:yoff?\n", (vistring)"%f", &yoffset);
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
//
//
//// this program reads a waveform from a tektronix
//// tds scope and writes the floating point values to 
//// stdout.
//int main(int argc, char* argv[])
//{
//	visession rm, vi;
//	vichar buffer[256];
//	//status variable
//	vistatus status;
//	//device name
//	vistring name = virsrc("usb::0x0699::0x0364::c020219::instr");
//
//	printf("device name: %s\n" , name);
//	status = viopendefaultrm(&rm);
//	if (status < vi_success)
//	{
//		printf("default rm failure");
//		goto error;
//	}
//	
//
//	status = viopen(rm, name, 0, 5000, &vi);
//	if (status < vi_success) 
//	{
//		printf("viopen failure");
//		goto error;
//	}
//
//	// get visa manufacturer name
//	//status = vigetattribute(vi, vi_attr_rsrc_manf_name, (vistring)buffer); (vipattrstate)
//	status = vigetattribute(vi, vi_attr_rsrc_manf_name, (vipattrstate)buffer); 
//	if (status < vi_success)
//	{
//		printf("get attribute failure");
//		goto error;
//	}
//	// set timeout to 5 seconds
//	//visetattribute(vi, vi_attr_tmo_value, 5000);
//	//printf(“manufacturer: %s\n”, buffer);
//
//	//printf("\nsuccess");
//	printf("manufacturer name: %s\n\n fuck yeeesss", buffer);
//	viclose(rm);
//
//	while (1)
//	{
//
//	}
//error:
//	printf("\nthick!");
//	vistatusdesc(vi, status, buffer);
//	fprintf(stderr, "error reason: %s\n", buffer);
//	while (1)
//	{
//
//	}
////	visession	rm = vi_null, vi = vi_null;
////	vistatus	status;
////	vichar		buffer[256];
////	double*		wfm = null;
////	long		elements, i;
////
////	// open a default session
////	status = viopendefaultrm(&rm);
////	if (status < vi_success)
////		printf("default rm error");
////		goto error;
////
////	// open the gpib device at primary address 1, gpib board 8
////	//status = viopen(rm, (vistring)"gpib8::1::instr", vi_null, vi_null, &vi);
////	status = viopen(rm, (vistring)"usb::0x0699::0x0364::c020219::instr", vi_null, vi_null, &vi);
////	if (status < vi_success) goto error;
////
////	// read waveform and write it to stdout
////	wfm = readwaveform(vi, &elements);
////	if (wfm != null) {
////		for (i = 0; i < elements; i++) {
////			printf("%f\n", wfm[i]);
////		}
////	}
////
////	// clean up
////	if (wfm != null) free(wfm);
////	viclose(vi); // not needed, but makes things a bit more understandable
////	viclose(rm);
////
////	return 0;
////
////error:
////	// report error and clean up
////	vistatusdesc(vi, status, buffer);
////	fprintf(stderr, "failure: %s\n", buffer);
////	if (rm != vi_null) viclose(rm);
////	if (wfm != null) free(wfm);
////	
////
////	while (1)
////	{
////
////	}
////
////	return 1;
//}
