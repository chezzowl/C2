/*This file is supposed to hold all necessary variables/instances for a device communication*/
#pragma once
//#include "stdafx.h"
#include <visa.h>
#include <visatype.h>
#include <ostream>
#include <string>

/******************DEFAULT SETTINGS*********************/

///*default name of the device we are communicating with*/
const ViString DEVICE_DEFAULT = ViString("USB::0x0699::0x0364::C020219::INSTR");

///*default size of the read buffer*/
const ViUInt32 BUFSIZE_DEFAULT = 1000;


/******************FUNCTION DEFINITIONS******************/

/*starts the default resource manager and returns its success - flag*/
ViStatus startRMSession(ViSession& rm);

/*starts a ViSession and returns its success - flag*/
ViStatus startVISession(ViSession& resmanager, ViSession& session, ViString device = DEVICE_DEFAULT);

/*initializes the buffer*/
ViStatus set_ReadBuffer(ViSession& session, ViUInt32 size = BUFSIZE_DEFAULT);

/*
sets the session's buffer flushing strategy

Parameters:
-----------

ViSession session:

bool onAccess:
*/
ViStatus writeBuf_flushing(ViSession& session, bool onAccess);

/*
sets the session's buffer flushing strategy

Parameters:
-----------

ViSession session:

bool onAccess:
*/
ViStatus readBuf_flushing(ViSession& session, bool onAccess);

/*
appends basic session information to a given ostream
RECOMMENDATION: passing std::cout as the function argument results
in the information being printed
*/
void printSessionInfo(ViSession& session, std::ostream& os);

/*
allows printing of a specified error message

Inputs:
ViSession session - the session in which the error occured

std::ostream& os -

std::string message -

*/
void printErrorInfo(ViSession& session, std::ostream& os, std::string message);