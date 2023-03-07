#pragma once
#include "ev3lib.h"

#define GRABBER_MOTORS_POWER 40
#define DEFAULT_POWER 30
#define TURN_POWER 20
#define OBJECT_DISTANCE 20

enum class RubbishType : uint8_t { Paper, Can, Bottle };

class Robot : NonCopyable
{
public:
	Robot();
	RubbishType grabAndIdentifyRubbish();
	void placeRubbish();
	void emergencyStop();
private:
	uint16_t getDistFilterZero();
private:
	ev3::Motor lMotor;
	ev3::Motor rMotor;
	ev3::Motor lGrabberMotor;
	ev3::Motor rGrabberMotor;
	//ev3::ColorSensor lColorSensor;
	ev3::ColorSensor rColorSensor;
	ev3::UltrasonicSensor distanceSensor;
};
