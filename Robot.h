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
	void driveForward();
	void driveBackward();
	void turnLeft();
	void turnRight();
	void stop();
	RubbishType grabAndIdentifyRubbish();
	void placeRubbish();
	void driveBack();
	void emergencyStop();
	void setDrivePower(uint8_t power);
private:
	uint16_t getDistFilterZero();
	void drive();
private:
	ev3::Motor lMotor;
	ev3::Motor rMotor;
	ev3::Motor lGrabberMotor;
	ev3::Motor rGrabberMotor;
	//ev3::ColorSensor lColorSensor;
	ev3::ColorSensor rColorSensor;
	ev3::UltrasonicSensor distanceSensor;
	//=======
	int8_t lDrivePower = 0;
	int8_t rDrivePower = 0;
	uint8_t powerPercent = 100;
};
