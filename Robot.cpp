#include "Robot.h"

Robot::Robot()
	: lMotor(ev3::MotorPort::B, ev3::MotorType::Medium, true),
	  rMotor(ev3::MotorPort::C, ev3::MotorType::Medium, false),
	  lGrabberMotor(ev3::MotorPort::A, ev3::MotorType::Unregulated, false),
	  rGrabberMotor(ev3::MotorPort::D, ev3::MotorType::Unregulated, false),
	  //lColorSensor(ev3::SensorPort::S2),
	  distanceSensor(ev3::SensorPort::S1),
	  borderDistanceSensor(ev3::SensorPort::S2),
	  rColorSensor(ev3::SensorPort::S3),
	  gyroSensor(ev3::SensorPort::S4),
	  directionAngle(0),
	  totalAngle(0),
	  correctionAngle(0),
	  turnCount(0),
	  currentDirection(0, 1),
	  position(0, 0),
	  driver(gyroSensor, directionAngle, correctionAngle, Robot::diffFunc, lMotor, rMotor)
{
	while (!(distanceSensor.ready() && borderDistanceSensor.ready() && rColorSensor.ready() && gyroSensor.ready())) { ev3::Time::delay(10); }
	driver.setCoefficents(1.0f, 0.4f);
}

void Robot::openGrabbers(bool wait)
{
	lGrabberMotor.rotate(LEFT_GRABBER_MOTOR_OPEN_ANGLE - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER, false);
	rGrabberMotor.rotate(RIGHT_GRABBER_MOTOR_OPEN_ANGLE, GRABBER_MOTORS_POWER, false);
	if (wait) { ev3::Time::delay(700); }
}

void Robot::closeGrabbers(bool wait)
{
	lGrabberMotor.rotate(30 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER, false);
	rGrabberMotor.rotate(-RIGHT_GRABBER_MOTOR_OPEN_ANGLE, GRABBER_MOTORS_POWER, false);
	if (wait) { ev3::Time::delay(700); }
}

RubbishType Robot::grabAndIdentifyRubbish(int32_t* countsOut)
{	
	lMotor.resetCounts();
	lMotor.setPower(DEFAULT_SPEED);
	rMotor.setPower(DEFAULT_SPEED);
	while (getDistFilterZero() > OBJECT_DISTANCE) 
	{ 
		ev3::Time::delay(1); 
		if (borderDistanceSensor.getDistance() < MINIMAL_BORDER_DISTANCE || lMotor.getCounts() > (DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) * 2) { lMotor.stop(true); rMotor.stop(true); break; }
	}
	//if (countsOut != nullptr) { *countsOut = lMotor.getCounts(); }
	//lMotor.resetCounts();
	lMotor.setPower(15);
	rMotor.setPower(15);

	rGrabberMotor.resetCounts();
	rGrabberMotor.setPower(-GRABBER_MOTORS_POWER);
	
	int8_t speed = 0;
	ev3::ColorDef color;
	do
	{
		ev3::Time::delay(1);
		if (borderDistanceSensor.getDistance() < MINIMAL_BORDER_DISTANCE || lMotor.getCounts() > (DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) * 2) { lMotor.stop(true); rMotor.stop(true); }
		speed = rGrabberMotor.getSpeed();
		color = rColorSensor.getColor();
	} while (!(speed < -6 && color != ev3::ColorDef::Yellow));
	
	while (speed < -4 && color != ev3::ColorDef::Yellow)
	{
		ev3::Time::delay(1);
		if (borderDistanceSensor.getDistance() < MINIMAL_BORDER_DISTANCE || lMotor.getCounts() > (DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) * 2) { lMotor.stop(true); rMotor.stop(true); }
		speed = rGrabberMotor.getSpeed();
		color = rColorSensor.getColor();
	}

	ev3::Time::delay(1);

	if (color != ev3::ColorDef::Yellow) 
	{ 
		lMotor.stop(true);
		rMotor.stop(true);
		rGrabberMotor.rotate(-rGrabberMotor.getCounts() - RIGHT_GRABBER_MOTOR_OPEN_ANGLE, GRABBER_MOTORS_POWER);
	}
	else { rGrabberMotor.rotate(40, GRABBER_MOTORS_POWER); }

	if (borderDistanceSensor.getDistance() < MINIMAL_BORDER_DISTANCE || lMotor.getCounts() > (DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) * 2) { lMotor.stop(true); rMotor.stop(true); }

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
		//bottles > 67
		//cans > 34 old 27
		//paper < 34
		ev3::Console::write("%d", sizeAngle);
		if (sizeAngle < 4) { rubbish = RubbishType::None; ev3::Console::write("empty"); break; }
		if (sizeAngle < 34) { rubbish = RubbishType::Paper; ev3::Console::write("paper"); break; }
		if (sizeAngle < 67) { rubbish = RubbishType::Can; ev3::Console::write("can"); break; }

		if (i == 0) 
		{ 
			lMotor.setPower(-DEFAULT_SPEED);
			rMotor.setPower(-DEFAULT_SPEED);
			while (lMotor.getCounts() > DRIVE_TO_LINE_COUNTS);
			lMotor.setPower(15);
			rMotor.setPower(15);
			lGrabberMotor.rotate(150 - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
			ev3::Time::delay(300);
		}
	}

	lMotor.stop(true); 
	rMotor.stop(true);

	if (countsOut != nullptr) { *countsOut = lMotor.getCounts(); }

	return rubbish;
}

void Robot::placeRubbish()
{
	/*lGrabberMotor.rotate(LEFT_GRABBER_MOTOR_OPEN_ANGLE - lGrabberMotor.getCounts(), GRABBER_MOTORS_POWER);
	rGrabberMotor.setPower(GRABBER_MOTORS_POWER);
	ev3::Time::delay(500);
	while (rColorSensor.getColor() != ev3::ColorDef::Yellow) { ev3::Time::delay(1); }
	rGrabberMotor.rotate(-105, GRABBER_MOTORS_POWER);*/
	//openGrabbers();
	driveForMotorCounts(360);
	driveForMotorCounts(-360);
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
	else if (direction.x == -1 && direction.y == 0) { angle = -90; }
	else { return; }

	currentDirection = direction;

	turnToAngle(angle);
}

void Robot::turnToAngle(int16_t angle)
{
	turnCount++;
	correctionAngle = turnCount / 4;
	ev3::Time::Clock clock;
	int16_t delta = angle - directionAngle;
	if (delta < -180) { delta += 360; }
	if (delta > 180) { delta -= 360; }
	//ev3::Console::write("a %d", delta);
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
		while (clock.getElapsedTime() < NINETY_DEGREE_TURN_TIME * 2 + TURN_BALANCE_TIME) { driver.drive(); }
	}
	else
	{
		totalAngle += delta;
		unsigned long turnTime = NINETY_DEGREE_TURN_TIME * std::abs(delta) / 90 + TURN_BALANCE_TIME;
		while (clock.getElapsedTime() < turnTime) { driver.drive(); }
	}

	driver.stop();

	ev3::Console::write("total: %d turnCount: %d correction: %d", totalAngle, turnCount, correctionAngle);
}

void Robot::driveForMotorCounts(int32_t counts)
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
	driver.stop();
}

void Robot::driveToLine()
{
	driver.setSpeed(DEFAULT_SPEED);
	while (rColorSensor.getColor() != ev3::ColorDef::Black) { driver.drive(); }
	driver.stop();
}

void Robot::driveOneCellForward(int32_t additionalCounts)
{
	driveToLine();
	driveForMotorCounts(additionalCounts);
	position += currentDirection;
}

void Robot::driveAroundForward(bool right)
{
	if (right)
	{
		turnToAngle(directionAngle + 45);
		driveForMotorCounts(180);
		driveToLine();
		driveForMotorCounts(270);
		turnToAngle(directionAngle - 90);
		driveForMotorCounts(180);
		driveToLine();
		driveForMotorCounts(270);
		turnToAngle(directionAngle + 45);
	}
	else
	{
		turnToAngle(directionAngle - 45);
		driveToLine();
		turnToAngle(directionAngle + 90);
		driveToLine();
		turnToAngle(directionAngle - 45);
	}
	driver.stop();
	position += currentDirection;
}

void Robot::driveAroundTurnLeft()
{
	currentDirection = turnLeft(currentDirection);
	position += currentDirection;

	turnToAngle(directionAngle - 45);
	driveToLine();
	turnToAngle(directionAngle - 45);
	driver.stop();
}

void Robot::driveAroundTurnRight()
{
	currentDirection = turnRight(currentDirection);
	position += currentDirection;

	turnToAngle(directionAngle + 45);
	driveForMotorCounts(180);
	driveToLine();
	driveForMotorCounts(270);
	turnToAngle(directionAngle + 45);
	driver.stop();
}

bool Robot::checkBorder(bool onCenter)
{
	/*uint16_t dist = 0; 
	for (int i = 0; i < 20; i++)
	{
		dist += borderDistanceSensor.getDistance();
		ev3::Time::delay(5);
	}

	dist /= 20;*/
	uint16_t dist = borderDistanceSensor.getDistance();
	ev3::Console::write("border: %d", dist);
	return dist < (onCenter ? BORDER_DISTANCE : BORDER_DISTANCE_FROM_LINE);
}

void Robot::stop()
{
	driver.stop();
}

/*void Robot::calibrateGyroSensor()
{
	lMotor.setPower(15);
	rMotor.setPower(15);
	ev3::Time::delay(3000);
	lMotor.stop(false);
	rMotor.stop(false);
	ev3::Time::delay(200);
	int16_t delta = gyroSensor.getAngle() - totalAngle;
	ev3::Console::write("delta: %d", delta);
	lMotor.setPower(-15);
	rMotor.setPower(-15);
	ev3::Time::delay(500);
	turnToAngle(delta);
	driveForMotorCounts(-200);
	ev3::Time::delay(500);
	gyroSensor.reset();
	ev3::Time::delay(500);
}*/

ev3::Vector2c Robot::getPosition() const
{
	return position;
}

void Robot::setPosition(ev3::Vector2c pos)
{
	position = pos;
}

ev3::Vector2c Robot::getDirection() const
{
	return currentDirection;
}

void Robot::setCorrectionAngle(int16_t angle)
{
	correctionAngle = angle;
}

uint16_t Robot::getDistFilterZero()
{
	uint16_t dist = 0;
	do { dist = distanceSensor.getDistance(); } while (dist == 0);
	return dist;
}

float Robot::diffFunc(ev3::Tuple<ev3::GyroSensor*, int16_t*, int16_t*>& sensors)
{
	int16_t angle = (*sensors.get<1>() - sensors.get<0>()->getAngle() + *sensors.get<2>()) % 360;
	if (angle < 0) { angle += 360; }
	if (angle > 180) { angle -= 360; }

	if (angle < -20) { angle = -20; }
	else if (angle > 20) { angle = 20; }

	return angle;
}
