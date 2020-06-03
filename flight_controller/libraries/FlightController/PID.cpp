#include <Arduino.h>
#include "PID.h"

PID::PID() {
	SetConstants(1.0, 0.0, 0.0);
	SetSampleTime(0ul);
	SetDesiredValue(0.0);
	SetOutputLimits(500.0, -500.0);
	ResetValues();
}

PID::PID(float kp, float kd, float ki, float desiredValue, unsigned long sampleTime, float maxOutput, float minOutput) {
	SetConstants(kp, kd, ki);
	SetDesiredValue(desiredValue);
	SetSampleTime(sampleTime);
	SetOutputLimits(maxOutput, minOutput);
	ResetValues();
}

PID::PID(float kp, float kd, float ki) {
	SetConstants(kp, kd, ki);
	SetSampleTime(0ul);
	SetDesiredValue(0.0);
	SetOutputLimits(500.0, -500.0);
	ResetValues();
}

PID::PID(float kp, float kd, float ki, float desiredValue, float sampleTime, float maxOutput, float minOutput) {
	SetConstants(kp, kd, ki);
	SetDesiredValue(desiredValue);
	SetSampleTime(sampleTime);
	SetOutputLimits(maxOutput, minOutput);
}

void PID::SetSampleTime(float sampleTime) {
	float microsecondsInSecond = 1000000.0;
	_sampleTime = sampleTime * microsecondsInSecond;
}

void PID::ResetValues() {
	_integral = 0.0;
	_lastError = 0.0;
	_lastOutput = 0.0;
	_lastUpdateTime = 0;
	_outputCalculated = false;
	_derivativeCalculated = false;
}

void PID::SetConstants(float kp, float kd, float ki) {
	_kp = kp;
	_kd = kd;
	_ki = ki;
}

float PID::ClampValue(float value, float min, float max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}

float PID::Update(float actualValue) {
	float microsecondsInSecond = 1000000.0;
	float output, error, derror, pid_p, pid_d, pid_i;
	unsigned long actualTime, elapsedTime;
	float elapsedTimeInSeconds;
	actualTime = micros();
	elapsedTime = actualTime - _lastUpdateTime;
	if (elapsedTime <= _sampleTime && _outputCalculated) {
		return _lastOutput;
	}
	elapsedTimeInSeconds = elapsedTime / microsecondsInSecond;
	error = _desiredValue - actualValue;
	derror = error - _lastError;
	pid_p = _kp * error;
	if (_derivativeCalculated) {
		// pid_d = _kd * (derror / elapsedTimeInSeconds);
		pid_d = _kd * derror;
	} else {
		pid_d = 0;
		_derivativeCalculated = true;
	}
	_integral += _ki * (error * elapsedTimeInSeconds);
	_integral = ClampValue(_integral, _minOutput, _maxOutput);
	pid_i = _integral;
	output = pid_p + pid_d + pid_i;
	output = ClampValue(output, _minOutput, _maxOutput);
	_outputCalculated = true;
	_lastUpdateTime = actualTime;
	_lastError = error;
	_lastOutput = output;
	return output;
}

void PID::SetDesiredValue(float desiredValue) {
	_desiredValue = desiredValue;
	_outputCalculated = false;
}

void PID::SetSampleTime(unsigned long sampleTime) {
	_sampleTime = sampleTime;
}

void PID::SetOutputLimits(float maxOutput, float minOutput) {
	_maxOutput = maxOutput;
	_minOutput = minOutput;
}