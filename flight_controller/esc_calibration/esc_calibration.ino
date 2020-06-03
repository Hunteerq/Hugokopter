#include <Wire.h>
#include <Servo.h>

Servo motor1, motor2, motor3, motor4;
int state = 0;
int motorsSpeed = 1000;

void setup() {
  Serial.begin(9600);
  motor1.attach(4);
  motor2.attach(5);
  motor3.attach(6);
  motor4.attach(7);
  setMotorsSpeed(1000);
  Serial.println("Disconnect battery and send something on Serial");
  waitForSerialData();
  setMotorsSpeed(2000);
  Serial.println("Connect battery, wait for proper signal and send something on Serial");
  waitForSerialData();
  setMotorsSpeed(1000);
  Serial.println("After proper signal send something on Serial");
  waitForSerialData();
  Serial.println("Your motors are calibrated :) You can send motors speed over Serial to test calibration. Remeber to turn off motors after testing.");
}

void loop() {
  motorsSpeed = Serial.parseInt();
  if(motorsSpeed >= 1000 && motorsSpeed <= 2000){
    setMotorsSpeed(motorsSpeed);
  }
}

void setMotorsSpeed(int speed){
  Serial.print("Setting motors speed to ");
  Serial.println(speed);
  motor1.writeMicroseconds(speed);
  motor2.writeMicroseconds(speed);
  motor3.writeMicroseconds(speed);
  motor4.writeMicroseconds(speed);
}

void waitForSerialData(){
  while(Serial.available() > 0) Serial.read();
  delay(100);
  while(Serial.available() == 0) {};
  delay(100);
  while(Serial.available() > 0) Serial.read();
  delay(100);
}
