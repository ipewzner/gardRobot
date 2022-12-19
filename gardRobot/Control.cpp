 // 
// 
// 

#include "Control.h"
Control::Control(IMU* imu, Motor* motorA, Motor* motorB, double balencePoint, double Kp, double Ki, double Kd, size_t tiltAxle){
	this->imu = imu;
	this->motorA = motorA;
	this->motorB = motorB;
	
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	this->tiltAxle = tiltAxle;	
	this->Input = 0;
	this->Output = 0; 
	this->Setpoint = 0;
	this->balencePoint = balencePoint;

}

void Control::init(){

	pid = new PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
	pid->SetMode(AUTOMATIC);
	pid->SetOutputLimits(-255, 255);
	pid->SetSampleTime(10);
	motorA->stop();
	motorB->stop();
}

void Control::keepSteady() {
	Input = imu->get_Angle(tiltAxle);
	pid->Compute();
}

void Control::move(int rightValue, int leftValue) {
	
	Serial.println("rightValue: " + String(rightValue)+ "leftValue: " + String(leftValue));

	/*
	the set point make the robot to tiltand accelerate
	the right value is betwin 0 to 100 so we need to
	sub 50 and multiply it by 2 
	to get the value between -100 to 100
	and then add or sub the balence point in marge of 30 degree	
	*/
	Setpoint = balencePoint + (rightValue - 50) * 2 * 0.3;
	
	
	int turn = (leftValue-50);
	
	motorA->move(constrain(Output + turn,-255,255));
	motorB->move(constrain(Output - turn,-255,255));
}

void Control::setKp(double Kp) { this->Kp = Kp; Serial.println("set Kp:"+String(Kp));}
void Control::setKi(double Ki) { this->Ki = Ki; Serial.println("set Ki:" + String(Ki));}
void Control::setKd(double Kd) { this->Kd = Kd; Serial.println("set Kd:" + String(Kd));}
void Control::setPIDsetPoint(double setPoint) { this->Setpoint = setPoint; Serial.println("set setPoint:" + String(setPoint));
}

void Control::printKpid() {
	Serial.println("Kp: " + String(Kp) + ", Ki: " + String(Ki) + ", Kd: " + String(Kd));
}

void Control::stop() {
	motorA->stop();
	motorB->stop();
}
								
void Control::set_tiltAxle(size_t tiltAxle) {
	this->tiltAxle = constrain(tiltAxle, 0, 2);
}

float Control::tiltAngle(){
	return Input; // imu->get_Angle(tiltAxle);	//Input is from keepSteady()
}
void Control::setMotorDirection(size_t motor, bool direction) {
	
	switch (motor)
	{
	case 0:
		motorA->setDirection(direction);
		Serial.println("set a direction:" + String(direction));
		break;
	case 1:
		motorB->setDirection(direction);
		Serial.println("set b direction:" + String(direction));
		break;
	default:
		break;
	}
	
}
