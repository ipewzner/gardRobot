// DcMotor.h

#ifndef _DCMOTOR_h
#define _DCMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DcMotorClass
{
 protected:


 public:
	void init();
};

extern DcMotorClass DcMotor;

#endif

