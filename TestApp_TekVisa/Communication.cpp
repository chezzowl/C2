#include "Communication.h"
#include <stdio.h>
#include <iostream>

/*******Variable Declarations*******/
//ViString device;
//ViUInt32 bufsize;
//ViSession rm, vi;
//set default values
//bool defaultRM_isOpen = false;
//bool defaultVI_isOpen = false;

/******FUNCTION DEFINITIONS FROM SETTINGS.H********/

ViStatus startRMSession(ViSession& rm)
{
	return viOpenDefaultRM(&rm);
}

ViStatus startVISession(ViSession& resmanager, ViSession& session, ViString device)
{
	return viOpen(resmanager, device, NULL, NULL, &session);
}

ViStatus set_ReadBuffer(ViSession & session, ViUInt32 size)
{
	return viSetBuf(session, VI_READ_BUF, size);
}

ViStatus change_ReadBuffer(ViSession & session, int newBufSize)
{
	return ViStatus();
}

ViStatus readBuf_flushing(ViSession& session, bool onAccess)
{
	if (onAccess)
	{
		return viSetAttribute(session, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
	}
	else
	{
		return viSetAttribute(session, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_WHEN_FULL);
	}
}

ViStatus writeBuf_flushing(ViSession& session, bool onAccess)
{
	if (onAccess)
	{
		return viSetAttribute(session, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
	}
	else
	{
		return viSetAttribute(session, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_WHEN_FULL);
	}
}

void printSessionInfo(ViSession& session, std::ostream& os)
{
	os << "Session INFO" << std::endl;
}

void printErrorInfo(ViSession& session, std::ostream& os, std::string message)
{
	os << "ERROR INFO\n" << message << std::endl;
}