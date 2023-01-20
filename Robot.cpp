#include "Robot.h"

Robot::Robot()
	: lMotor(ev3::MotorPort::B, ev3::MotorType::Medium, true),
	  rMotor(ev3::MotorPort::C, ev3::MotorType::Medium, false),
	  lGrabberMotor(ev3::MotorPort::A, ev3::MotorType::Unregulated, false),
	  rGrabberMotor(ev3::MotorPort::D, ev3::MotorType::Unregulated, false),
	  //lColorSensor(ev3::SensorPort::S2),
	  rColorSensor(ev3::SensorPort::S3),
	  distanceSensor(ev3::SensorPort::S1)
{
	lGrabberMotor.rotate(180, GRABBER_MOTORS_POWER);
	rGrabberMotor.rotate(-135, GRABBER_MOTORS_POWER);
	while (!(distanceSensor.ready() && rColorSensor.ready())) { ev3::Time::delay(10); }
}

void Robot::driveForward()
{
	lDrivePower = DEFAULT_POWER;
	rDrivePower = DEFAULT_POWER;
	drive();
}

void Robot::driveBackward()
{
	lDrivePower = -DEFAULT_POWER;
	rDrivePower = -DEFAULT_POWER;
	drive();
}

void Robot::turnLeft()
{
	lDrivePower = 0;
	rDrivePower = DEFAULT_POWER;
	drive();
}

void Robot::turnRight()
{
	lDrivePower = DEFAULT_POWER;
	rDrivePower = 0;
	drive();
}

void Robot::stop()
{
	lDrivePower = 0;
	rDrivePower = 0;
	drive();
}


RubbishType Robot::grabAndIdentifyRubbish()
{	
	lMotor.setPower(DEFAULT_POWER);
	rMotor.setPower(DEFAULT_POWER);
	while (getDistFilterZero() > OBJECT_DISTANCE) { ev3::Time::delay(1); }
	lMotor.setPower(15);
	rMotor.setPower(15);

	//=====================

	rGrabberMotor.resetCounts();
	rGrabberMotor.setPower(-GRABBER_MOTORS_POWER);

	int8_t speed = 0;
	uint8_t reflect = 0;
	do
	{
		speed = rGrabberMotor.getSpeed();
		reflect = rColorSensor.getReflect();
	} while (speed >= -4 && reflect > RIGHT_ARM_REFLECT);
	
	ev3::Time::delay(1);
	
	while (speed < -4 && reflect > RIGHT_ARM_REFLECT)
	{
		speed = rGrabberMotor.getSpeed();
		reflect = rColorSensor.getReflect();
	}

	ev3::Time::delay(1);

	if (reflect > RIGHT_ARM_REFLECT) { rGrabberMotor.rotate(-rGrabberMotor.getCounts() + 135, GRABBER_MOTORS_POWER); }
	else { rGrabberMotor.rotate(30, GRABBER_MOTORS_POWER); }

	RubbishType rubbish = RubbishType::Bottle;

	for (int i = 0; i < 2; i++)
	{
		lGrabberMotor.setPower(-GRABBER_MOTORS_POWER);
		while (lGrabberMotor.getSpeed() >= -4) { ev3::Time::delay(1); }
		while (lGrabberMotor.getSpeed() < -4) { ev3::Time::delay(1); }
		lGrabberMotor.stop(false);
		ev3::Time::delay(100);
		lGrabberMotor.stop(true);
		int32_t sizeAngle = lGrabberMotor.getCounts();
		//bottles > 60 (70-95)
		//cans > 27 (30-50)
		//paper < 27 (10-20)
		ev3::Console::write("%d", sizeAngle);
		if (sizeAngle < 27) { rubbish = RubbishType::Paper; break; }
		if (sizeAngle < 60) { rubbish = RubbishType::Can; break; }

		if (i == 0) 
		{ 
			lGrabberMotor.rotate(150 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
			ev3::Time::delay(300);
		}
	}

	lGrabberMotor.stop(true);
	lMotor.stop(true); 
	rMotor.stop(true);
	return rubbish;
}

void Robot::placeRubbish()
{
	lGrabberMotor.rotate(180 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
	rGrabberMotor.setPower(GRABBER_MOTORS_POWER);
	lMotor.setPower(-10);
	rMotor.setPower(-10);
	ev3::Time::delay(500);
	lMotor.stop(true);
	rMotor.stop(true);
	while (rColorSensor.getReflect() > 15) { ev3::Time::delay(1); }
	rGrabberMotor.rotate(-105, GRABBER_MOTORS_POWER);
}

void Robot::driveBack()
{
	lMotor.setPower(-DEFAULT_POWER);
	rMotor.setPower(-DEFAULT_POWER);
	while (lMotor.getCounts() > 0);
	lMotor.stop(true);
	rMotor.stop(true);
}

void Robot::emergencyStop()
{
	lMotor.stop(false);
	rMotor.stop(false);
	lGrabberMotor.stop(false);
	rGrabberMotor.stop(false);

	for (int i = 0; i < 2; i++)
	{
		ev3::Brick::setLEDColor(ev3::LEDColor::Red);
		ev3::Time::delay(300);
		ev3::Brick::setLEDColor(ev3::LEDColor::Off);
		ev3::Time::delay(300);
	}
	ev3::Brick::setLEDColor(ev3::LEDColor::Red);
}

void Robot::setDrivePower(uint8_t power)
{
	powerPercent = power;
	drive();
}

uint16_t Robot::getDistFilterZero()
{
	uint16_t dist = 0;
	do { dist = distanceSensor.getDistance(); } while (dist == 0);
	return dist;
}

void Robot::drive()
{
	lMotor.setPower((int)lDrivePower * (int)powerPercent / 100);
	rMotor.setPower((int)rDrivePower * (int)powerPercent / 100);
}
