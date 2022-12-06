// 
// 
// 

#include "IMU.h"

IMU::IMU(int offsetX, int offsetY, int offsetZ) {
    Serial.println("MPU c-tor");

    mpu = new MPU6050();
    q = new     Quaternion();
    aa = new   VectorInt16();
    gy = new   VectorInt16();
    aaReal = new   VectorInt16();
    aaWorld = new   VectorInt16();
    gravity = new   VectorFloat();

    euler = new float[3];         // [psi, theta, phi]    Euler angle container
    ypr = new float[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    offset = new int[3];

    this->offset[0] = offsetX;
    this->offset[1] = offsetY;
    this->offset[2] = offsetZ;

    Serial.println("MPU c-tor end");
}

void IMU::init()
{
    Serial.println("MPU init");

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin(I2C_SDA,I2C_SCL);
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    //Wire.setTimeout(3000); //timeout value in uSec

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu->initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));

    devStatus = mpu->dmpInitialize();
  
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {

        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu->CalibrateAccel(6);
        mpu->CalibrateGyro(6);

        mpu->PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu->setDMPEnabled(true);

        delay(100);
        mpuIntStatus = mpu->getIntStatus();
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu->dmpGetFIFOPacketSize();
    }
    else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print("\nDMP Initialization failed (code " + String(devStatus) + ")");
        while (true) { delay(100); }
    }
    delay(5000);
    Serial.println("\nMPU init end");
}

void IMU::update() {
    //Serial.println("mpu update");

    while (!mpu->dmpGetCurrentFIFOPacket(fifoBuffer)) {
       // delay(100);
       // Serial.println("@");
    } // Get the Latest packet 

    mpu->dmpGetQuaternion(q, fifoBuffer);
    mpu->dmpGetGravity(gravity, q);
    mpu->dmpGetYawPitchRoll(ypr, q, gravity);
    mpu->dmpGetEuler(euler, q); //mpu.dmpGetEuler(euler, &q);
}

float IMU::get_X_Angle() {
    this->update();
    return (ypr[0] * 180 / M_PI);
}

float IMU::get_Y_Angle() {
    this->update();
    return (ypr[1] * 180 / M_PI);
}

float IMU::get_Z_Angle() {
    this->update();
    return (ypr[2] * 180 / M_PI);
}

float IMU::get_Angle(size_t axel) {
    this->update();
    axel= constrain(axel, 0, 2);
    return (ypr[axel] * 180 / M_PI);
}

float* IMU::get_AllAngle() {

    this->update();
    float yprAngles[3];

    for (size_t i = 0; i < 3; i++) {
        yprAngles[i] = (ypr[i] * 180 / M_PI);
    }
    return yprAngles;
}

void IMU::resetAxis(size_t axis) {

    switch (axis)
    {
    case 0:
        mpu->setXGyroOffset(0);
        break;
    case 1:
        mpu->setYGyroOffset(0);
        break;
    case 2:
        mpu->setZGyroOffset(0);
        break;
    default:
        break;
    }

}

void IMU::printYPR() {
    update();
    String result = "\tx " + String(ypr[0] * 180 / M_PI);
    result += "\ty " + String(ypr[1] * 180 / M_PI);
    result += "\tz " + String(ypr[2] * 180 / M_PI);
    Serial.println(result);
}

