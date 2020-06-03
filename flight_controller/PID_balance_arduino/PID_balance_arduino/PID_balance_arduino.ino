#include <Wire.h>
#include <Servo.h>
#include <AccelGyro.h>
#include <PID.h>


Servo right_prop;
Servo left_prop;
Servo left_prop2;
Servo right_prop2;
AccelGyro accelGyro;
PID pid_pitch, pid_roll, pid_yaw;
float pid_pitch_output, pid_roll_output, pid_yaw_output;
int motorLeft, motorLeft2, motorRight, motorRight2;
Rotation rotation;

/////////////////PID CONSTANTS/////////////////
float kp_pitch=3;
float ki_pitch=0.0;
float kd_pitch=0.3;
double kp_roll=1;
double ki_roll=0;
double kd_roll=0;
double kp_yaw=1;
double ki_yaw=0;
double kd_yaw=0;
///////////////////////////////////////////////

int max_pitch=400;
int max_roll=400;
int max_yaw=400;
double throttle=1200;
float desired_angle = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  right_prop.attach(6); 
  right_prop2.attach(7);
  left_prop.attach(4);
  left_prop2.attach(5);
  accelGyro.Initialize();
  Serial.print("Is connected: ");
  Serial.println(accelGyro.IsConnected());
  if(!accelGyro.IsConnected()){
    Serial.println("Cannot get data from MPU6050");
    while(1);
  }
  //accelGyro.Calibrate();
  pid_pitch.SetConstants(kp_pitch, kd_pitch, ki_pitch);
  pid_roll.SetConstants(kp_roll, kd_roll, ki_roll);
  pid_yaw.SetConstants(kp_yaw, kd_yaw, ki_yaw);
  motorLeft = 1000;
  motorLeft2 = 1000;
  motorRight = 1000;
  motorRight2 = 1000;
  left_prop.writeMicroseconds(motorLeft); 
  right_prop.writeMicroseconds(motorRight);
  left_prop2.writeMicroseconds(motorLeft2); 
  right_prop2.writeMicroseconds(motorRight2);
  delay(7000); 
}

void loop() {
  rotation = accelGyro.GetRotation();
  pid_roll_output = 0;
  pid_yaw_output = 0;
  //pid_roll_output = pid_roll.Update(rotation.x);
  pid_pitch_output = pid_pitch.Update(rotation.y);
  //pid_yaw_output = pid_yaw.Update(rotation.z);

  //Serial.println(rotation.y);

  motorLeft = throttle + pid_roll_output + pid_pitch_output + pid_yaw_output;
  motorLeft2 = throttle + pid_roll_output - pid_pitch_output - pid_yaw_output;
  motorRight = throttle - pid_roll_output + pid_pitch_output - pid_yaw_output;
  motorRight2 = throttle - pid_roll_output - pid_pitch_output + pid_yaw_output;
  ClampValue(motorLeft);
  ClampValue(motorLeft2);
  ClampValue(motorRight);
  ClampValue(motorRight2);  
  
  left_prop.writeMicroseconds(motorLeft);
  left_prop2.writeMicroseconds(motorLeft2);
  right_prop.writeMicroseconds(motorRight);
  right_prop2.writeMicroseconds(motorRight2);

}

void ClampValue(int& value){
  if (value < 1000){
    value = 1000;
  }
  if (value > 1500){
    value = 1500;
  }
}
