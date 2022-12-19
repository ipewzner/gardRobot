// Control.h

#ifndef _CONTROL_h
#define _CONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <PID_v1.h>
#include "Motor.h"
#include "IMU.h"
class Control {

	IMU* imu;
	Motor* motorA;
	Motor* motorB;	
	PID* pid;
	double Kp, Ki, Kd;
	size_t tiltAxle;
	

public:
	double Setpoint, Input, Output, balencePoint;

	Control(IMU* imu, Motor* motorA, Motor* motorB, double balencePoint = 0, double Kp = 0, double Ki = 0, double Kd = 0, size_t tiltAxle = 0);
	
	void setKp(double);
	void setKi(double);
	void setKd(double);
	void setPIDsetPoint(double);

	void init();
	void keepSteady();
	void move(int, int);
	void stop();
	void printKpid();	
	void set_tiltAxle(size_t);
	float tiltAngle();
	void setMotorDirection(size_t,bool);
};
#endif











