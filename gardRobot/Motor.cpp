// 
// 
// 

#include "Motor.h"

size_t Motor::channels=1;

void Motor::init()
{
    ledcSetup(channelA, freq, resolution);
    ledcSetup(channelB, freq, resolution);
	
	ledcAttachPin(legA, channelA);
	ledcAttachPin(legB, channelB);

   //pinMode(legA, OUTPUT);   
   //pinMode(legB, OUTPUT);  
	stop();
}

Motor::Motor(size_t legA, size_t legB, bool reverse) {
    this->legA = legA;
	this->legB = legB;
	this->reverse = reverse;

	channelA = channels++;   
    freq = 5000;
    resolution = 8;
}
    
void Motor::stop() {
    ledcWrite(channelA,0);
	ledcWrite(channelB,0);
}

void Motor::move(int speed) {
    speed = constrain(speed, -255, 255);
    Serial.println("speed: " + String(speed));
   
    if (speed >= 0) {
        ledcWrite(channelA, speed);      //PWM
        ledcWrite(channelB, 0);
    }
    else { //reverse
        ledcWrite(channelA, 0);
        ledcWrite(channelB, -speed);       //PWM
    }    
	
}
