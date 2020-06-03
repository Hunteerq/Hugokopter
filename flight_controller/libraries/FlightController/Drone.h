#ifndef Drone_h
#define Drone_h

#include <Wire.h>
#include <Servo.h>
#include <AccelGyro.h>
#include "Arduino.h"
#include <PID.h>



const float kp_pitch = 14;
const float ki_pitch = 0.0;
const float kd_pitch = 25;
const double kp_roll = kp_pitch;
const double ki_roll = ki_pitch;
const double kd_roll = kd_pitch;
const double kp_yaw = 0;
const double ki_yaw = 0;
const double kd_yaw = 0;
///////////////////////////////////////////////



class Drone {
    public:
        void initialize();
        void droneLoop();
        void setThrottle(int speed);
        void setDirection(int command, int drone_speed);
    private:
        Servo right_prop;
        Servo left_prop;
        Servo left_prop2;
        Servo right_prop2;
        AccelGyro accelGyro;
        PID pid_pitch, pid_roll, pid_yaw;
        int throttle = 1000;
        int motorLeft, motorLeft2, motorRight, motorRight2;
        int isWorking = 0;
        int state = 0;
        void setMotorsWhenDronIsOff();
        void setMotorsWhenDronIsOn(Rotation rotation);
        void setMotorPowers();
        void clampValue(int &value);
        void printAllMotors();
        void rotationGuard(Rotation rotation);
        void resetValues();
};
#endif