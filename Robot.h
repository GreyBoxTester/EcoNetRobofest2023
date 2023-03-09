#pragma once
#include "ev3lib.h"
#include "Util.h"

#define GRABBER_MOTORS_POWER 40
#define LEFT_GRABBER_MOTOR_OPEN_ANGLE 180
#define RIGHT_GRABBER_MOTOR_OPEN_ANGLE (-140)
#define DEFAULT_SPEED 30
#define TURN_SPEED 20
#define OBJECT_DISTANCE 20
#define DRIVE_TO_CENTER_COUNTS 640
#define DRIVE_TO_LINE_COUNTS 180
#define NINETY_DEGREE_TURN_TIME 600
#define TURN_BALANCE_TIME 700
#define WALL_DISTANCE 70 

enum class RubbishType : uint8_t { Paper, Can, Bottle };

class Robot : NonCopyable
{
public:
	Robot();
	void openGrabbers();
	void closeGrabbers();
	RubbishType grabAndIdentifyRubbish(int32_t* countsOut = nullptr);
	void placeRubbish();
	void emergencyStop();
	void turnToDirection(ev3::Vector2c direction);
	void turnToAngle(int16_t angle);
	void driveForMotorCounts(int32_t counts);
	void driveToLine();
	void driveOneCellForward(int32_t additionalCounts = DRIVE_TO_CENTER_COUNTS);
	void driveAroundForward(bool right);
	void driveAroundTurnLeft();
	void driveAroundTurnRight();
	bool checkWall();
	void stop();
	ev3::Vector2c getPosition() const;
	void setPosition(ev3::Vector2c pos);
	ev3::Vector2c getDirection() const;
private:
	uint16_t getDistFilterZero();
	static float diffFunc(ev3::Tuple<ev3::GyroSensor*, int16_t*>& sensors);
private:
	ev3::Motor lMotor;
	ev3::Motor rMotor;
	ev3::Motor lGrabberMotor;
	ev3::Motor rGrabberMotor;
	ev3::UltrasonicSensor distanceSensor;
	ev3::IRSensor wallDistanceSensor;
	ev3::ColorSensor rColorSensor;
	ev3::GyroSensor gyroSensor;
	int16_t directionAngle;
	int16_t totalAngle;
	ev3::Vector2c currentDirection;
	ev3::Vector2c position;
	ev3::PDRegulator<ev3::GyroSensor, int16_t> driver;
};
