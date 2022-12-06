// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Motor{
	size_t legA, legB, channelA, channelB;
	bool reverse;
	int freq;//5000
	int resolution;//8

 public:	  
	 static size_t channels;

	Motor(size_t, size_t, bool=0);
	void stop();
	void move(int);
	void init();
};
#endif

