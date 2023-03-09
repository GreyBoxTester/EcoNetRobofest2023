#pragma once
#include "ArrayFixed.hpp"
#include "Console.hpp"

#define MAX_PATH_LENGTH 9

typedef ev3::ArrayFixed<ev3::Vector2c, MAX_PATH_LENGTH + 1> Path;

void printPath(const Path& path);
