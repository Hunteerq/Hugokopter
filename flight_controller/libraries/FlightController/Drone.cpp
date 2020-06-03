#include <Wire.h>
#include <Servo.h>
#include <AccelGyro.h>
#include "Drone.h"
#include "Arduino.h"
#include <PID.h>

void Drone::initialize() {
    left_prop.attach(6);
    left_prop2.attach(7);
    right_prop2.attach(5);
    right_prop.attach(4); 
	setMotorsWhenDronIsOff();
    setMotorPowers();
    accelGyro.Initialize();
    if (accelGyro.IsConnected()) {
        isWorking = 1;
        accelGyro.CalibrateGyro();
    }
    Serial.print("IsWorking: ");
    Serial.println(isWorking);
    pid_pitch.SetConstants(kp_pitch, kd_pitch, ki_pitch);
    pid_roll.SetConstants(kp_roll, kd_roll, ki_roll);
    pid_yaw.SetConstants(kp_yaw, kd_yaw, ki_yaw);
}

void Drone::droneLoop() {
    Rotation rotation = accelGyro.GetRotation();
	//Serial.print(rotation.x);
	//Serial.print(" ");
	//Serial.print(rotation.y);
	//Serial.print(" ");
	//Serial.println(rotation.z);
    rotationGuard(rotation);
    switch(state * isWorking) {
        case 0:
            //Serial.print("State 0 | ");
            setMotorsWhenDronIsOff();
            break;
        case 1:
            //Serial.print("State 1 | ");
            setMotorsWhenDronIsOn(rotation);
            break;
    }
    //printAllMotors();
    setMotorPowers();
}

void Drone::rotationGuard(Rotation rotation) {
    if (rotation.x > 15.0 || rotation.x < -15.0 || rotation.y > 15.0 || rotation.y < -15.0) {
        state = 0;
    }
}

void Drone::setMotorsWhenDronIsOff() {
    motorLeft = 1000;
    motorLeft2 = 1000;
    motorRight = 1000;
    motorRight2 = 1000;
}

void Drone::setMotorsWhenDronIsOn(Rotation rotation) {
    float pid_roll_output, pid_yaw_output, pid_pitch_output;
    if (throttle >= 1100) {
        pid_roll_output = pid_roll.Update(-rotation.y);
        pid_yaw_output = pid_yaw.Update(rotation.z);
        pid_pitch_output = pid_pitch.Update(-rotation.x);
    } else {
        pid_roll_output = 0;
        pid_yaw_output = 0;
        pid_pitch_output = 0;
    }
    motorLeft = throttle + pid_roll_output + pid_pitch_output + pid_yaw_output;
    motorLeft2 = throttle + pid_roll_output - pid_pitch_output - pid_yaw_output;
    motorRight = throttle - pid_roll_output + pid_pitch_output - pid_yaw_output;
    motorRight2 = throttle - pid_roll_output - pid_pitch_output + pid_yaw_output;
    clampValue(motorLeft);
    clampValue(motorLeft2);
    clampValue(motorRight);
    clampValue(motorRight2);  
}

// TODO: Debug only
void Drone::printAllMotors() {
    Serial.print("l1: ");
    Serial.print(motorLeft);
    Serial.print("| l2: ");
    Serial.print(motorLeft2);
    Serial.print("| r1: ");
    Serial.print(motorRight);
    Serial.print("| r2: ");
    Serial.println(motorRight2);
}

void Drone::setMotorPowers() {
    left_prop.writeMicroseconds(motorLeft);
    left_prop2.writeMicroseconds(motorLeft2);
    right_prop.writeMicroseconds(motorRight);
    right_prop2.writeMicroseconds(motorRight2);
}

void Drone::setThrottle(int speed) {
    throttle = 1000 + 1000 * speed / 100.f;
    clampValue(throttle);
}

void Drone::clampValue(int& value) {
    if (value < 1000) {
        value = 1000;
    }
    if (value > 2000) {
        value = 2000;
    }
}

void Drone::setDirection(int command, int drone_speed) {
    switch(command) {
        case 1:
            pid_roll.SetDesiredValue(0.0);
            pid_pitch.SetDesiredValue(5.0);
            break;

        case 2:
            pid_pitch.SetDesiredValue(0.0);
            pid_roll.SetDesiredValue(5.0);
            break;

        case 3:
            pid_roll.SetDesiredValue(0.0);
            pid_pitch.SetDesiredValue(-5.0);
            break;

        case 4:
            pid_pitch.SetDesiredValue(0.0);
            pid_roll.SetDesiredValue(-5.0);
            break;

        case 5:
            throttle += 25;
            clampValue(throttle);
            break;

        case 6:
            throttle -= 25;
            clampValue(throttle);
            break;
        
        case 7:
            pid_pitch.SetDesiredValue(0.0);
            pid_roll.SetDesiredValue(0.0);
            break;

        case 8:
            state = 1;
            break;

        case 9:
            resetValues();
            state = 0;
            break;

        default:
            break;
    }
}

void Drone::resetValues() {
    throttle = 1000;
    pid_roll.SetDesiredValue(0.0);
    pid_pitch.SetDesiredValue(0.0);
    pid_yaw.SetDesiredValue(0.0);
}