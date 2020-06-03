#ifndef PID_h
#define PID_h

#include "Arduino.h"

class PID {
public:
	PID();
	PID(float kp, float kd, float ki, float desiredValue, unsigned long sampleTime, float maxOutput, float minOutput);
	PID(float kp, float kd, float ki, float desiredValue, float sampleTime, float maxOutput, float minOutput);
	PID(float kp, float kd, float ki);
	float Update(float actualValue);
	void SetDesiredValue(float desiredValue);
	void SetSampleTime(unsigned long sampleTime);
	void SetSampleTime(float sampleTime);
	void SetOutputLimits(float maxOutput, float minOutput);
	void SetConstants(float kp, float kd, float ki);
private:
	float _kp;
	float _kd;
	float _ki;
	float _desiredValue;
	float _integral;
	unsigned long _sampleTime;
	float _maxOutput;
	float _minOutput;
	float _lastError;
	float _lastOutput;
	unsigned long _lastUpdateTime;
	bool _outputCalculated;
	bool _derivativeCalculated;
	float ClampValue(float value, float min, float max);
	void ResetValues();
};
#endif