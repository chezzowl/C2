#pragma once
#include <visa.h>
#include <visatype.h>
//#include <TekVisa.h>

/*Defines all the main function variables*/

/********************VARIABLES**************************/

/*device name*/
ViString dev;

/*the bufsize we are reading*/
ViUInt32 bufsize;

/*necessary ViSessions, i.e. one default resource manager and the session itself*/
ViSession rm, vi;

/*
indicators, whether a session is running (may be useful later on)
TODO: see, if this can be replaced by a function reading some sort of session
attribute, which is NULL, if the session is closed (or something of that sort)
*/
//false by default
bool defaultRM_isOpen;
bool defaultVI_isOpen;