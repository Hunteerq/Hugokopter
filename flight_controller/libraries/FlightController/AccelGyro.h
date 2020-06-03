#ifndef AccelGyro_h
#define AccelGyro_h

#include "Wire.h"
#include "Arduino.h"
#include "math.h"

const int GYRO_250_DEG = 0;
const int GYRO_500_DEG = 1;
const int GYRO_1000_DEG = 2;
const int GYRO_2000_DEG = 3;
const int ACCEL_2G = 0;
const int ACCEL_4G = 1;
const int ACCEL_8G = 2;
const int ACCEL_16G = 3;

const int POWER_REG = 0x6B;
const int ACCEL_CONF_REG = 0x1C;
const int GYRO_CONF_REG = 0x1B;

const int ACCEL_DATA_REG = 0x3B;
const int GYRO_DATA_REG = 0x43;

const int CHECK_CONN_REG = 0x75;

const int CALIBRATION_SAMPLES = 600;

class Rotation {
public:
	Rotation();
	Rotation(double x, double y, double z);
	double x;
	double y;
	double z;
};

class Correction {
public:
	Correction();
	Correction(double x, double y, double z);
	double x;
	double y;
	double z;
};

class SensorData {
public:
	SensorData();
	SensorData(double x, double y, double z);
	double x;
	double y;
	double z;
	void Correct(Correction correction);
	void Add(SensorData sensorData);
};

class RawSensorData {
public:
	RawSensorData();
	RawSensorData(int16_t x, int16_t y, int16_t z);
	int16_t x;
	int16_t y;
	int16_t z;
	SensorData ToSensorData(const float* scaleModifiers, int sensivity);
};

class AccelGyro {
public:
	AccelGyro();
	AccelGyro(int address);
	void Calibrate();
	void CalibrateGyro();
	SensorData GetData(int dataRegister);
	Rotation GetRotationAccel();
	Rotation GetRotationGyro();
	Rotation GetRotation();
	void SetGyroSensitivity(int sensitivityLevel);
	void SetAccelSensitivity(int sensitivityLevel);
	void SetAccelCorrection(Correction correction);
	void SetGyroCorrection(Correction correction);
	Correction GetAccelCorrection();
	Correction GetGyroCorrection();
	void Initialize();
	bool IsConnected();
private:
	int _address;
	int _gyroSensitivityLevel;
	int _accelSensitivityLevel;
	Correction _accelCorrection;
	Correction _gyroCorrection;
	float _compFilterAlpha;
	unsigned long _prevMeasurementTime;
	Rotation _rotation;
	void I2cWriteData(int address, byte data);
	void I2cReadData(int address, byte* output, int numberOfBytes);
	RawSensorData GetRawData(int dataRegister);
	byte I2cReadOneByte(int address);
	SensorData CorrectSensorData(SensorData sensorData, Correction correction);
	const float _gyroScaleModifiers[4] = {
		131.0,
		65.5,
		32.8,
		16.4
	};
	const float _accelScaleModifiers[4] = {
		16384.0,
		8192.0,
		4096.0,
		2048.0
	};
};
#endif