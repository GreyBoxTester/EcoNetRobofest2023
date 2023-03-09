#include "Application.h"

Application::Application() 
{
	//field.at(1, 1).type = field.at(2, 1).type = field.at(3, 1).type = Field::Cell::Type::Empty;
}

void Application::sortFirstThree()
{
	robot.driveOneCellForward(DRIVE_TO_LINE_COUNTS);
	for (int i = 0; i < 3; i++)
	{
		goToZigZag(Field::Cell::Type::InterferingRubbish, false);

		robot.openGrabbers();
		int32_t movedBy = 0;
		auto destination = getDestinationCellType(robot.grabAndIdentifyRubbish(&movedBy));
		field.at(robot.getPosition()).type = Field::Cell::Type::Empty;
		robot.driveForMotorCounts((DRIVE_TO_CENTER_COUNTS - DRIVE_TO_LINE_COUNTS) - movedBy);

		goToZigZag(destination, true);

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
	Path path;
	for (int rubbishLeft = 7; rubbishLeft > 0; rubbishLeft--)
	{
		field.print();
		goTo(Field::Cell::Type::Rubbish, false);

		int32_t movedBy = 0;
		auto destination = getDestinationCellType(robot.grabAndIdentifyRubbish(&movedBy));
		robot.driveForMotorCounts(DRIVE_TO_CENTER_COUNTS - movedBy);
		field.at(robot.getPosition()).type = Field::Cell::Type::Empty;

		ev3::Console::write("u:%d d:%d l:%d r:%d", field.at(robot.getPosition()).topBorder, field.at(robot.getPosition()).bottomBorder, field.at(robot.getPosition()).leftBorder, field.at(robot.getPosition()).rightBorder);
		field.print();
		goTo(destination, false);

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
		pathGen.generatePath(field, robot.getPosition(), destination, false, &path);
		printPath(path);
		for (size_t i = 0; i < path.size() - 1; i++)
		{
			ev3::Vector2c direction = path[i + 1] - path[i];
			robot.turnToDirection(direction);
			if (robot.checkWall()) { field.addBorder(robot.getPosition(), robot.getDirection()); break; }
			if (i < path.size() - 2)
			{
				robot.driveOneCellForward();
				if (robot.checkWall()) { field.addBorder(robot.getPosition(), robot.getDirection()); break; }
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
	pathGen.generatePath(field, robot.getPosition(), destination, true, &path);
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
	}
	return Field::Cell::Type::Empty;
}
