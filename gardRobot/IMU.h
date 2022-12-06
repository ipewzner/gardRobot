// IMU.h

#ifndef _IMU_h
#define _IMU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define I2C_SDA 14
#define I2C_SCL 15
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

class IMU
{
    // MPU control/status vars
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion* q;           // [w, x, y, z]         quaternion container
    VectorInt16* aa;         // [x, y, z]            accel sensor measurements
    VectorInt16* gy;         // [x, y, z]            gyro sensor measurements
    VectorInt16* aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16* aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat* gravity;    // [x, y, z]            gravity vector

    float* euler;         // [psi, theta, phi]    Euler angle container
    float* ypr;           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    int* offset;

public:
    MPU6050* mpu;
    IMU(int offsetX = 0, int offsetY = 0, int offsetZ = 0);


    // class default I2C address is 0x68

    void update();
    void printYPR();
    float get_X_Angle();  //מיותר
    float get_Y_Angle();  //מיותר
    float get_Z_Angle();  //מיותר
    float get_Angle(size_t axel);
    float* get_AllAngle();
    void resetAxis(size_t);
    void init();
};
#endif


