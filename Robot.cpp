#include "Robot.h"

Robot::Robot()
	: lMotor(ev3::MotorPort::B, ev3::MotorType::Medium, true),
	  rMotor(ev3::MotorPort::C, ev3::MotorType::Medium, false),
	  lGrabberMotor(ev3::MotorPort::A, ev3::MotorType::Unregulated, false),
	  rGrabberMotor(ev3::MotorPort::D, ev3::MotorType::Unregulated, false),
	  //lColorSensor(ev3::SensorPort::S2),
	  rColorSensor(ev3::SensorPort::S3),
	  distanceSensor(ev3::SensorPort::S1),
	  gyroSensor(ev3::SensorPort::S4),
	  directionAngle(0),
	  totalAngle(0),
	  currentDirection(0, 1),
	  position(0, 0),
	  driver(gyroSensor, directionAngle, Robot::diffFunc, lMotor, rMotor)
{
	lGrabberMotor.rotate(180, GRABBER_MOTORS_POWER);
	rGrabberMotor.rotate(-135, GRABBER_MOTORS_POWER);
	while (!(distanceSensor.ready() && rColorSensor.ready() && gyroSensor.ready())) { ev3::Time::delay(10); }
	driver.setCoefficents(1.1f, 0.4f);
}

RubbishType Robot::grabAndIdentifyRubbish()
{	
	lMotor.setPower(DEFAULT_SPEED);
	rMotor.setPower(DEFAULT_SPEED);
	while (getDistFilterZero() > OBJECT_DISTANCE) { ev3::Time::delay(1); }
	lMotor.setPower(15);
	rMotor.setPower(15);

	rGrabberMotor.resetCounts();
	rGrabberMotor.setPower(-GRABBER_MOTORS_POWER);

	int8_t speed = 0;
	ev3::ColorDef color;
	do
	{
		ev3::Time::delay(1);
		speed = rGrabberMotor.getSpeed();
		color = rColorSensor.getColor();
	} while (!(speed < -6 && color != ev3::ColorDef::Yellow));
	
	while (speed < -4 && color != ev3::ColorDef::Yellow)
	{
		ev3::Time::delay(1);
		speed = rGrabberMotor.getSpeed();
		color = rColorSensor.getColor();
	}

	ev3::Time::delay(1);

	if (color != ev3::ColorDef::Yellow) { rGrabberMotor.rotate(-rGrabberMotor.getCounts() + 135, GRABBER_MOTORS_POWER); }
	else { rGrabberMotor.rotate(40, GRABBER_MOTORS_POWER); }

	RubbishType rubbish = RubbishType::Bottle;

	for (int i = 0; i < 2; i++)
	{
		lGrabberMotor.setPower(-GRABBER_MOTORS_POWER);
		while (lGrabberMotor.getSpeed() > -6) { ev3::Time::delay(1); }
		while (lGrabberMotor.getSpeed() < -4) { ev3::Time::delay(1); }
		lGrabberMotor.stop(false);
		ev3::Time::delay(100);
		lGrabberMotor.stop(true);
		int32_t sizeAngle = lGrabberMotor.getCounts();
		lGrabberMotor.setPower(-10);
		//bottles > 67 (70-95)
		//cans > 27 (30-55)
		//paper < 27 (10-20)
		ev3::Console::write("%d", sizeAngle);
		if (sizeAngle < 4) { rubbish = RubbishType::Bottle; break; } //bottle dropped and rolled away
		if (sizeAngle < 27) { rubbish = RubbishType::Paper; break; }
		if (sizeAngle < 67) { rubbish = RubbishType::Can; break; }

		if (i == 0) 
		{ 
			lGrabberMotor.rotate(150 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
			ev3::Time::delay(300);
		}
	}

	lMotor.stop(true); 
	rMotor.stop(true);
	return rubbish;
}

void Robot::placeRubbish()
{
	lMotor.setPower(-10);
	rMotor.setPower(-10);
	lGrabberMotor.rotate(180 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
	rGrabberMotor.setPower(GRABBER_MOTORS_POWER);
	ev3::Time::delay(500);
	lMotor.stop(true);
	rMotor.stop(true);
	while (rColorSensor.getColor() != ev3::ColorDef::Yellow) { ev3::Time::delay(1); }
	rGrabberMotor.rotate(-105, GRABBER_MOTORS_POWER);
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

void Robot::turnToDirection(ev3::Vector2c direction)
{
	if (currentDirection == direction) { return; }

	int16_t angle;
	if (direction.x == 0 && direction.y == 1) { angle = 0; }
	else if (direction.x == 1 && direction.y == 0) { angle = 90; }
	else if (direction.x == 0 && direction.y == -1) { angle = 180; }
	else { angle = -90; } // (direction.x == -1 && direction.y == 0)

	currentDirection = direction;

	turnToAngle(angle);
}

void Robot::turnToAngle(int16_t angle)
{
	ev3::Time::Clock clock;
	int16_t delta = angle - directionAngle;
	if (delta < -180) { delta += 360; }
	if (delta > 180) { delta -= 360; }
	directionAngle = angle;
	driver.setSpeed(0);
	if (std::abs(delta) == 180)
	{
		if (totalAngle > 0)
		{
			lMotor.setPower(-TURN_SPEED);
			rMotor.setPower(TURN_SPEED);
			totalAngle -= 180;
		}
		else
		{
			lMotor.setPower(TURN_SPEED);
			rMotor.setPower(-TURN_SPEED);
			totalAngle += 180;
		}

		while (clock.getElapsedTime() < NINETY_DEGREE_TURN_TIME / 2);
		while (clock.getElapsedTime() < NINETY_DEGREE_TURN_TIME * 2) { driver.drive(); }
	}
	else
	{
		totalAngle += delta;
		unsigned long turnTime = NINETY_DEGREE_TURN_TIME * delta / 90;
		while (clock.getElapsedTime() < turnTime) { driver.drive(); }
	}

	ev3::LCD::clear();
	char num[5];
	itoa(totalAngle, num, 10);
	ev3::LCD::drawString(num, 50, 20);
}

void Robot::driveForMotorCounts(int counts)
{
	lMotor.resetCounts();
	if (counts > 0)
	{
		driver.setSpeed(DEFAULT_SPEED);
		while (lMotor.getCounts() < counts) { driver.drive(); }
	}
	else if (counts < 0)
	{
		driver.setSpeed(-DEFAULT_SPEED);
		while (lMotor.getCounts() > counts) { driver.drive(); }
	}
}

void Robot::driveOneCellForward(bool driveToCenter)
{
	driver.setSpeed(DEFAULT_SPEED);
	while (rColorSensor.getColor() != ev3::ColorDef::Black) { driver.drive(); }
	if (driveToCenter) { driveForMotorCounts(DRIVE_TO_CENTER_COUNTS); }
	position += currentDirection;
}

void Robot::followPath(ev3::Vector2c* begin, ev3::Vector2c* end, bool driveToLastCellCenter)
{
	if (begin == end) { return; }
	end--;
	for (; begin != end; begin++)
	{
		ev3::Vector2c direction = *(begin + 1) - *begin;
		turnToDirection(direction);
		driveOneCellForward((begin != end - 1) || driveToLastCellCenter);
	}
	driver.stop();
}

void Robot::stop()
{
	driver.stop();
}

uint16_t Robot::getDistFilterZero()
{
	uint16_t dist = 0;
	do { dist = distanceSensor.getDistance(); } while (dist == 0);
	return dist;
}

float Robot::diffFunc(ev3::Tuple<ev3::GyroSensor*, int16_t*>& sensors)
{
	int16_t angle = (*sensors.getRest().current - sensors.current->getAngle()) % 360;
	if (angle < 0) { angle += 360; }
	if (angle > 180) { angle -= 360; }

	if (angle < -20) { angle = -20; }
	else if (angle > 20) { angle = 20; }

	return angle;
}
