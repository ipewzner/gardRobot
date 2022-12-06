#include "Angle.h"

Angle::Angle(float angle) {
    this->angle = angle;
}

Angle::Angle() {
    this->angle = 0;
}

/*
void Angle::operator=(Angle* angle) {
    this->angle = angle->angle;
}
   */

Angle* Angle::operator/(Angle* angle) {
    return new Angle(this->angle - angle->angle);
}

Angle* Angle::operator+(Angle* angle) {
    return new Angle(this->angle + angle->angle);
}

void Angle::operator+=(Angle* angle) {
    this->angle += angle->angle;
}

Angle* Angle::operator-(Angle* angle) {
    return new Angle(this->angle - angle->angle);
}

Angle* Angle::percent(float percent) {
    return new Angle((angle / 100) * percent);
}

/*
Angle* Angle::add(Angle* angle) {
    return new Angle(this->angle + angle->angle);
}
Angle* Angle::sub(Angle* angle) {
    return new Angle(this->angle - angle->angle);
}
     */

void Angle::add(float angle) {
    this->angle += angle;
}

float Angle::angleToDegree() {
    return (float)fmodf((double)abs(angle), (double)360);
    /*
   if (angle < 0) angle =abs(angle);
    if (angle > 360) angle = (float)fmod((double)angle,360);
    return angle;
    */
}

float Angle::delta(Angle* angle) {
    //return this->sub(angle).angle;
    return this->angle / angle->get_angle();
}

float Angle::get_angle() {
    return angle;
}

void Angle::set_angle(float angle) {
    this->angle = angle;
}


