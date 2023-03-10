#include "Application.h"

Application::Application() {}

void Application::identifyFieldSide()
{
	robot.turnToDirection({ 1, 0 });
	if (!robot.checkBorder()) 
	{ 
		robot.setPosition({ 4, 0 });
		field.flipBottomLine(); 
	}
	robot.turnToDirection({ 0, 1 });
}

void Application::sortFirstThree()
{
	robot.driveOneCellForward(DRIVE_TO_LINE_COUNTS);
	for (int i = 0; i < 3; i++)
	{
		goToZigZag(Field::Cell::Type::InterferingRubbish, false);

		robot.openGrabbers();
		int32_t movedBy = 0;
		RubbishType rubbish = robot.grabAndIdentifyRubbish(&movedBy);
		field.at(robot.getPosition()).type = Field::Cell::Type::Empty;
		robot.driveForMotorCounts((DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) - movedBy);
		if (rubbish == RubbishType::None)
		{
			continue;
		}

		goToZigZag(getDestinationCellType(rubbish), true);

		robot.placeRubbish();
		robot.closeGrabbers();
		robot.turnToDirection({ 0, 1 });
		robot.setPosition(robot.getPosition() + robot.getDirection());
	}
	robot.driveForMotorCounts(DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS);
}

void Application::sortRubbish()
{
	robot.openGrabbers();
	for (int rubbishLeft = 7; rubbishLeft > 0; rubbishLeft--)
	{
		field.print();
		goTo(Field::Cell::Type::Rubbish, false);
		int32_t movedBy = 0;
		RubbishType rubbish = robot.grabAndIdentifyRubbish(&movedBy);
		robot.driveForMotorCounts(DRIVE_TO_CENTER_COUNTS - movedBy);
		field.at(robot.getPosition()).type = Field::Cell::Type::Empty;
		if (rubbish == RubbishType::None) 
		{
			robot.openGrabbers();
			continue; 
		}

		ev3::Console::write(
			"u:%d d:%d l:%d r:%d", 
			field.at(robot.getPosition()).topBorder, 
			field.at(robot.getPosition()).bottomBorder, 
			field.at(robot.getPosition()).leftBorder, 
			field.at(robot.getPosition()).rightBorder
		);
		field.print();
		goTo(getDestinationCellType(rubbish), false);

		robot.placeRubbish();
		robot.turnToDirection({ 0, 1 });
		robot.driveForMotorCounts(DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS * 2);
		robot.setPosition(robot.getPosition() + robot.getDirection());
	}
}

void Application::goToFinish()
{
	robot.closeGrabbers();
	goTo(Field::Cell::Type::Start, true);
}

void Application::goTo(Field::Cell::Type destination, bool driveToLastCellCenter)
{
	bool reachedDestination = false;
	Path path;
	while (!reachedDestination)
	{
		ev3::Console::write("start: %d %d", robot.getPosition().x, robot.getPosition().y);
		pathGen.generatePath(field, robot.getPosition(), robot.getDirection(), destination, false, &path);
		printPath(path);
		if (path.size() == 0)
		{
			field.removeBorders();
			ev3::Console::write("!!!BORDER RESET!!!");
			continue;
		}

		for (size_t i = 0; i < path.size() - 1; i++)
		{
			ev3::Vector2c direction = path[i + 1] - path[i];
			
			if (!(robot.getPosition().y == 1 && robot.getDirection() != ev3::Vector2c(0, 1)))
			{
				if (robot.checkBorder()) { field.addBorder(robot.getPosition(), robot.getDirection()); }
			}

			robot.turnToDirection(direction);
			
			if (!(robot.getPosition().y == 1 && robot.getDirection() != ev3::Vector2c(0, 1)))
			{
				if (robot.checkBorder()) { field.addBorder(robot.getPosition(), robot.getDirection()); break; }
			}

			if (i < path.size() - 2)
			{
				robot.driveOneCellForward();
			}
			else
			{
				robot.driveOneCellForward(driveToLastCellCenter ? DRIVE_TO_CENTER_COUNTS : 0);
				reachedDestination = true;
			}
		}
	}
}

void Application::goToZigZag(Field::Cell::Type destination, bool startInCenter)
{
	Path path;
	pathGen.generatePath(field, robot.getPosition(), robot.getDirection(), destination, true, &path);
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		ev3::Vector2c direction = path[i + 1] - path[i];
		if (field.at(robot.getPosition()).canDriveOnCenter())
		{
			if (i > 0 || !startInCenter) { robot.driveForMotorCounts(DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS); }
			robot.turnToDirection(direction);
			robot.driveOneCellForward(DRIVE_TO_LINE_COUNTS);
		}
		else
		{
			if (direction != robot.getDirection())
			{
				if (direction == turnRight(robot.getDirection())) { robot.driveAroundTurnRight(); }
				else if (direction == turnLeft(robot.getDirection())) { robot.driveAroundTurnLeft(); }
			}
			else
			{
				robot.driveAroundForward(direction.x == 1);
			}
		}
	}
}

Field::Cell::Type Application::getDestinationCellType(RubbishType rubbish)
{
	switch (rubbish)
	{
	case RubbishType::Paper: return Field::Cell::Type::PaperStorage;
	case RubbishType::Can: return Field::Cell::Type::CanStorage;
	case RubbishType::Bottle: return Field::Cell::Type::BottleStorage;
	default: return Field::Cell::Type::Empty;
	}
}
