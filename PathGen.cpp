#include "PathGen.h"

PathGen::PathGen()
	: bestPathPtr(nullptr), pathFound(false), direction(0, 0), destination(Field::Cell::Type::Empty)
{}

void PathGen::generatePath(const Field& field, ev3::Vector2c start, Field::Cell::Type end, Path* bestPathOut)
{
	bestPathOut->resize(Path::maxSize);
	bestPathPtr = bestPathOut;
	pathFound = false;
	destination = end;

	Path pathBuffer(1);
	pathBuffer[0] = start;

	direction = { 1, 0 };
	for (int i = 0; i < 4; i++)
	{
		if (field.at(start).canGoInDirection(direction))
		{
			pathBuffer.push(start + direction);
			generatePath(field, pathBuffer);
			pathBuffer.pop();
		}
		turnRight();
	}

	if (!pathFound) { bestPathOut->clear(); }
}

void PathGen::generatePath(const Field& field, Path& path)
{
	ev3::Vector2c currentPos = path.back();
	if (path.size() >= bestPathPtr->size() + !pathFound) { return; }
	if (field.at(currentPos).type == destination)
	{
		pathFound = true;
		*bestPathPtr = path;
		return;
	}
	if (field.at(currentPos).type == Field::Cell::Type::Rubbish) { return; }
	if (currentPos == path[path.size() - 5]) { return; }

	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //forward
		path.pop();
	}

	turnLeft();
	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //left
		path.pop();
	}
	turnRight();

	turnRight();
	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //right
		path.pop();
	}
	turnLeft();
}

void PathGen::turnLeft()
{
	int8_t newX = -direction.y;
	direction.y = direction.x;
	direction.x = newX;
}

void PathGen::turnRight()
{
	int8_t newX = direction.y;
	direction.y = -direction.x;
	direction.x = newX;
}
