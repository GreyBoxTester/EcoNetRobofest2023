#pragma once
#include "Field.h"
#include "Path.h"
#include <cstring>

class PathGen
{
public:
	PathGen();
	void generatePath(const Field& field, ev3::Vector2c start, Field::Cell::Type end, Path* bestPathOut);
private:
	void generatePath(const Field& field, Path& path);
	void turnLeft();
	void turnRight();
private:
	Path* bestPathPtr;
	bool pathFound;
	ev3::Vector2c direction;
	Field::Cell::Type destination;
};
