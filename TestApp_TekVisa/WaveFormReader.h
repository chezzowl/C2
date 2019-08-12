#pragma once
#include <visa.h>
#include <visatype.h>
#include <stdio.h>

class WaveFormReader
{
public:
	//WaveFormReader();
	//~WaveFormReader();

	/********** VARIABES ********/



	/********STATIC VARIABLES*********/
	/*
	offset and multiplication factors for properly converting
	the read wave form
	*/
	static float y_offset, y_mult;

	/**/
	static long* elemPtr;

	/***********FUNCTIONS*********/

	/**/
	static void initParams();

	/**/
	static double* readWave(ViSession& session, long* elements);


private:
	/*
	DEFAULT SETTINGS OF CONSTANTS
	apart from the default buffer size, those values don't have a particular
	motivation, maybe some will be found during the development process.
	For now, they serve the purpose of having an initialization.
	Definition is below the class definition.
	*/
	static const float YOFF_DEFAULT;
	static const float YMULT_DEFAULT;
};

//const float WaveFormReader::YOFF_DEFAULT = 5;
//const float WaveFormReader::YMULT_DEFAULT = 17;

