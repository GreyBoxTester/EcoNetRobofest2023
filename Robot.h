#pragma once
#include "ev3lib.h"
#include <vector>

#define GRABBER_MOTORS_POWER 40
#define DEFAULT_SPEED 30
#define TURN_SPEED 20
#define OBJECT_DISTANCE 20
#define DRIVE_TO_CENTER_COUNTS 640
#define NINETY_DEGREE_TURN_TIME 1300

enum class RubbishType : uint8_t { Paper, Can, Bottle };

class Robot : NonCopyable
{
public:
	Robot();
	RubbishType grabAndIdentifyRubbish();
	void placeRubbish();
	void emergencyStop();
	void turnToDirection(ev3::Vector2c direction);
	void turnToAngle(int16_t angle);
	void driveForMotorCounts(int counts);
	void driveOneCellForward(bool driveToCenter);
	void followPath(ev3::Vector2c* begin, ev3::Vector2c* end, bool driveToLastCellCenter);
	void stop();
private:
	uint16_t getDistFilterZero();
	static float diffFunc(ev3::Tuple<ev3::GyroSensor*, int16_t*>& sensors);
private:
	ev3::Motor lMotor;
	ev3::Motor rMotor;
	ev3::Motor lGrabberMotor;
	ev3::Motor rGrabberMotor;
	//ev3::ColorSensor lColorSensor;
	ev3::ColorSensor rColorSensor;
	ev3::UltrasonicSensor distanceSensor;
	ev3::GyroSensor gyroSensor;
	int16_t directionAngle;
	int16_t totalAngle;
	ev3::Vector2c currentDirection;
	ev3::Vector2c position;
	ev3::PDRegulator<ev3::GyroSensor, int16_t> driver;
};
