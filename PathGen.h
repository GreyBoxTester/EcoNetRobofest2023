#pragma once
#include "ArrayFixed.hpp"
#include "Field.h"
#include "Util.h"

#define MAX_PATH_LENGTH 9

typedef ev3::ArrayFixed<ev3::Vector2c, MAX_PATH_LENGTH + 1> Path;

class PathGen
{
public:
	PathGen();
	void generatePath(const Field& field, ev3::Vector2c start, Field::Cell::Type end, bool ignoreRubbish, Path* bestPathOut);
private:
	void generatePath(const Field& field, Path& path);
private:
	Path* bestPathPtr;
	bool ignoreRubbish;
	ev3::Vector2c direction;
	Field::Cell::Type destination;
};
