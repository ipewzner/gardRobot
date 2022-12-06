#pragma once
#ifndef _Angle_h
#define _Angle_h
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
class Angle
{
    float angle;
public:

    /**
    * @brief c-tor.
    *
    * @param angle
    */
    Angle(float angle);
    Angle();

    /**
     * @brief the '+' operator overloading.
     *
     * @param Angle*
     * @return Angle*
     */
    Angle* operator+(Angle* angle);
    void operator+=(Angle* angle);

    /**
    * @brief the '-' operator overloading.
    *
    * @param Angle*
    * @return Angle*
    */
    Angle* operator-(Angle* angle);

    // void operator=(Angle* angle);

     /**
     * @brief the '/' operator overloading.
     *
     * @param Angle*
     * @return Angle*
     */
    Angle* operator/(Angle* angle);

    /**
     * @brief the angel from this percent.
     *
     * @param percent
     * @return Angle*
     */
    Angle* percent(float percent);

    /**
     * @brief delete me.
     *
     * @param
     * @return
     */
    Angle* add(Angle*);

    /**
     * @brief add to the angle.
     *
     * @param angle
     */
    void add(float angle);

    /**
    * @brief delete me.
    *
    * @param
    * @return
    */
    Angle* sub(Angle*);

    /**
     * @brief convert angle to degree.
     *
     * @return float
     */
    float angleToDegree();

    /**
     * @brief delta between to angle.
     *
     * @param Angle*
     * @return float
     */
    float delta(Angle* angle);

    /**
     * @brief get the angle.
     *
     * @return float
     */
    float get_angle();

    /**
     * @brief set_angle.
     *
     * @param angle
     */
    void set_angle(float angle);
};

#endif