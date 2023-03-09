#include "PathGen.h"

PathGen::PathGen()
	: bestPathPtr(nullptr), ignoreRubbish(false), direction(0, 0), destination(Field::Cell::Type::Empty)
{}

void PathGen::generatePath(const Field& field, ev3::Vector2c start, Field::Cell::Type end, bool ignoreRubbish, Path* bestPathOut)
{
	if (field.at(start).type == end)
	{
		bestPathOut->resize(1);
		(*bestPathOut)[0] = start;
		return;
	}

	bestPathOut->resize(bestPathOut->capacity());
	bestPathPtr = bestPathOut;
	this->ignoreRubbish = ignoreRubbish;
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
		direction = turnRight(direction);
	}
}

void PathGen::generatePath(const Field& field, Path& path)
{
	ev3::Vector2c currentPos = path.back();
	if (path.size() >= bestPathPtr->size()) { return; }
	//if (currentPos.x < 0 || currentPos.x >= Field::sizeX || currentPos.y < 0 || currentPos.y >= Field::sizeY) { return; }
	if (field.at(currentPos).type == destination)
	{
		*bestPathPtr = path;
		return;
	}
	if (!ignoreRubbish && field.at(currentPos).type == Field::Cell::Type::Rubbish) { return; }
	if (currentPos == path[path.size() - 5]) { return; }

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
