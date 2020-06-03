#include "AccelGyro.h"
#include "Arduino.h"
#include "math.h"
#include "Wire.h"

AccelGyro::AccelGyro(void) {
	_address = 0x68;
}

AccelGyro::AccelGyro(int address) {
	_address = address;
}

Rotation::Rotation(void) {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Correction::Correction(void) {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

SensorData::SensorData(void) {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

RawSensorData::RawSensorData(void) {
	x = 0;
	y = 0;
	z = 0;
}

Rotation::Rotation(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Correction::Correction(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

SensorData::SensorData(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

RawSensorData::RawSensorData(int16_t x, int16_t y, int16_t z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void SensorData::Correct(Correction correction) {
	x -= correction.x;
	y -= correction.y;
	z -= correction.z;
}

SensorData RawSensorData::ToSensorData(const float* scaleModifiers, int sensivity) {
	SensorData sensorData;
	sensorData.x = x / scaleModifiers[sensivity];
	sensorData.y = y / scaleModifiers[sensivity];
	sensorData.z = z / scaleModifiers[sensivity];
	return sensorData;
}

void SensorData::Add(SensorData sensorData) {
	x += sensorData.x;
	y += sensorData.y;
	z += sensorData.z;
}

void AccelGyro::Initialize() {
	_accelCorrection = Correction(0.02, 0.0, 0.09);
	_gyroCorrection = Correction(0.0, 0.0, 0.0);
	_compFilterAlpha = 0.04;
	_prevMeasurementTime = micros();
	I2cWriteData(POWER_REG, 0x00);
	delay(100);
	SetAccelSensitivity(ACCEL_4G);
	SetGyroSensitivity(GYRO_500_DEG);
	_rotation = GetRotationAccel();
}

void AccelGyro::I2cWriteData(int address, byte data) {
	Wire.beginTransmission(_address);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission(true);
}

void AccelGyro::I2cReadData(int address, byte* output, int numberOfBytes) {
	Wire.beginTransmission(_address);
	Wire.write(address);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, numberOfBytes, true);

	for (int i = 0; i < numberOfBytes; i++) {
		output[i] = Wire.read();
	}
}

byte AccelGyro::I2cReadOneByte(int address) {
	Wire.beginTransmission(_address);
	Wire.write(address);
	Wire.endTransmission(false);
	Wire.requestFrom(_address, 1, true);
	return Wire.read();
}

bool AccelGyro::IsConnected() {
	byte checkConnValue = I2cReadOneByte(CHECK_CONN_REG);
	return checkConnValue == _address;
}

RawSensorData AccelGyro::GetRawData(int dataRegister) {
	byte rawData[6];
	RawSensorData rawSensorData;
	I2cReadData(dataRegister, rawData, 6);
	rawSensorData.x = (rawData[0] << 8 | rawData[1]);
	rawSensorData.y = (rawData[2] << 8 | rawData[3]);
	rawSensorData.z = (rawData[4] << 8 | rawData[5]);
	return rawSensorData;
}

SensorData AccelGyro::GetData(int dataRegister) {
	SensorData sensorData;
	const float* scaleModifiers = dataRegister == ACCEL_DATA_REG ? _accelScaleModifiers : _gyroScaleModifiers;
	int sensitivityLevel = dataRegister == ACCEL_DATA_REG ? _accelSensitivityLevel : _gyroSensitivityLevel;
	RawSensorData rawSensorData = GetRawData(dataRegister);
	sensorData = rawSensorData.ToSensorData(scaleModifiers, sensitivityLevel);
	return sensorData;
}

SensorData AccelGyro::CorrectSensorData(SensorData sensorData, Correction correction){
	SensorData correctedSensorData;
	correctedSensorData.x = sensorData.x - correction.x;
	correctedSensorData.y = sensorData.y - correction.y;
	correctedSensorData.z = sensorData.z - correction.z;
	return correctedSensorData;
}

Rotation AccelGyro::GetRotationAccel() {
	Rotation accelRotation;
	SensorData accelData = GetData(ACCEL_DATA_REG);
	accelData.Correct(_accelCorrection);
	accelRotation.x = atan2(accelData.y, sqrt(pow(accelData.x, 2) + pow(accelData.z, 2))) * RAD_TO_DEG;
	accelRotation.y = atan2(-accelData.x, sqrt(pow(accelData.y, 2) + pow(accelData.z, 2))) * RAD_TO_DEG;
	accelRotation.z = 0;
	return accelRotation;
}

Rotation AccelGyro::GetRotationGyro() {
	float microsecondsInSecond = 1000000.0;
	Rotation gyroRotation;
	unsigned long currentTime;
	double elapsedTime;
	SensorData gyroData = GetData(GYRO_DATA_REG);
	gyroData.Correct(_gyroCorrection);
	currentTime = micros();
	elapsedTime = (currentTime - _prevMeasurementTime) / microsecondsInSecond;
	gyroRotation.x = gyroData.x * elapsedTime;
	gyroRotation.y = gyroData.y * elapsedTime;
	gyroRotation.z = gyroData.z * elapsedTime;
	_prevMeasurementTime = currentTime;
	return gyroRotation;
}

Rotation AccelGyro::GetRotation() {
	Rotation rotationAccel, rotationGyro;
	rotationAccel = GetRotationAccel();
	rotationGyro = GetRotationGyro();
	_rotation.x = (1 - _compFilterAlpha) * (_rotation.x + rotationGyro.x) + _compFilterAlpha * rotationAccel.x;
	_rotation.y = (1 - _compFilterAlpha) * (_rotation.y + rotationGyro.y) + _compFilterAlpha * rotationAccel.y;
	_rotation.z += rotationGyro.z;
	if(_rotation.z > 360){
		_rotation.z = _rotation.z - 360;
	}else if(_rotation.z < -360){
		_rotation.z = _rotation.z + 360;
	}
	return _rotation;
}

void AccelGyro::SetGyroSensitivity(int sensitivityLevel) {
	_gyroSensitivityLevel = sensitivityLevel;
	I2cWriteData(GYRO_CONF_REG, _gyroSensitivityLevel << 3);
	delay(50);
}

void AccelGyro::SetAccelSensitivity(int sensitivityLevel) {
	_accelSensitivityLevel = sensitivityLevel;
	I2cWriteData(ACCEL_CONF_REG, _accelSensitivityLevel << 3);
	delay(50);
}

void AccelGyro::SetAccelCorrection(Correction correction) {
	_accelCorrection = correction;
}

void AccelGyro::SetGyroCorrection(Correction correction) {
	_gyroCorrection = correction;
}

Correction AccelGyro::GetAccelCorrection() {
	return _accelCorrection;
}

Correction AccelGyro::GetGyroCorrection() {
	return _gyroCorrection;
}

void AccelGyro::Calibrate() {
	SensorData accelData, gyroData, totalAccelData, totalGyroData;
	totalAccelData = SensorData(0.0, 0.0, 0.0);
	totalGyroData = SensorData(0.0, 0.0, 0.0);
	_rotation = Rotation(0.0, 0.0, 0.0);
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		accelData = GetData(ACCEL_DATA_REG);
		gyroData = GetData(GYRO_DATA_REG);
		totalAccelData.Add(accelData);
		totalGyroData.Add(gyroData);
	}
	_accelCorrection.x = totalAccelData.x / CALIBRATION_SAMPLES;
	_accelCorrection.y = totalAccelData.y / CALIBRATION_SAMPLES;
	_accelCorrection.z = (totalAccelData.z / CALIBRATION_SAMPLES) - 1;
	_gyroCorrection.x = totalGyroData.x / CALIBRATION_SAMPLES;
	_gyroCorrection.y = totalGyroData.y / CALIBRATION_SAMPLES;
	_gyroCorrection.z = totalGyroData.z / CALIBRATION_SAMPLES;
	_prevMeasurementTime = micros();
}

void AccelGyro::CalibrateGyro() {
	SensorData gyroData, totalGyroData;
	totalGyroData = SensorData(0.0, 0.0, 0.0);
	_rotation = Rotation(0.0, 0.0, 0.0);
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		gyroData = GetData(GYRO_DATA_REG);
		totalGyroData.Add(gyroData);
	}
	_gyroCorrection.x = totalGyroData.x / CALIBRATION_SAMPLES;
	_gyroCorrection.y = totalGyroData.y / CALIBRATION_SAMPLES;
	_gyroCorrection.z = totalGyroData.z / CALIBRATION_SAMPLES;
	_prevMeasurementTime = micros();
}