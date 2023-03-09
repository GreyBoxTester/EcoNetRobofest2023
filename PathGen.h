#pragma once
#include "ArrayFixed.hpp"
#include "Field.h"
#include "Util.h"
#include "Path.h"

class PathGen
{
public:
	PathGen();
	void generatePath(const Field& field, ev3::Vector2c start, Field::Cell::Type end, bool ignoreRubbish, Path* bestPathOut);
private:
	void generatePath(const Field& field, Path& path);
private:
	Path* bestPathPtr;
	bool foundPath;
	bool ignoreRubbish;
	ev3::Vector2c direction;
	Field::Cell::Type destination;
};
