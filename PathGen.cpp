#include "PathGen.h"

PathGen::PathGen()
	: bestPathPtr(nullptr), foundPath(false), ignoreRubbish(false), direction(0, 0), destination(Field::Cell::Type::Empty)
{}

void PathGen::generatePath(const Field& field, ev3::Vector2c start, ev3::Vector2c dir, Field::Cell::Type end, bool ignoreRubbish, Path* bestPathOut)
{
	if (field.at(start).type == end)
	{
		bestPathOut->resize(1);
		(*bestPathOut)[0] = start;
		return;
	}

	bestPathOut->resize(bestPathOut->capacity());
	bestPathPtr = bestPathOut;
	foundPath = false;
	this->ignoreRubbish = ignoreRubbish;
	destination = end;

	Path pathBuffer(1);
	pathBuffer[0] = start;

	direction = dir;
	for (int i = 0; i < 4; i++)
	{
		if (field.at(start).canGoInDirection(direction))
		{
			pathBuffer.push(start + direction);
			generatePath(field, pathBuffer);
			pathBuffer.pop();
		}
		direction = turnRight(direction);
	}

	if (!foundPath) { bestPathPtr->clear(); }
}

void PathGen::generatePath(const Field& field, Path& path)
{
	ev3::Vector2c currentPos = path.back();
	if (path.size() >= bestPathPtr->size()) { return; }
	if (field.at(currentPos).type == destination)
	{
		foundPath = true;
		*bestPathPtr = path;
		return;
	}
	if (!ignoreRubbish && field.at(currentPos).type == Field::Cell::Type::Rubbish) { return; }
	if (path.size() >= 5) { if (currentPos == path[path.size() - 5]) { return; } }

	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //forward
		path.pop();
	}

	direction = turnLeft(direction);
	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //left
		path.pop();
	}
	direction = turnRight(direction);

	direction = turnRight(direction);
	if (field.at(currentPos).canGoInDirection(direction))
	{
		path.push(currentPos + direction);
		generatePath(field, path); //right
		path.pop();
	}
	direction = turnLeft(direction);
}
