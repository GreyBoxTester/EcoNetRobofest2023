#pragma once
#include "Robot.h"
#include "Field.h"
#include "PathGen.h"

class Application : NonCopyable
{
public:
	Application();
	void identifyFieldSide();
	void sortFirstThree();
	void sortRubbish();
	void goToFinish();
	void goTo(Field::Cell::Type destination, bool driveToLastCellCenter);
	void goToZigZag(Field::Cell::Type destination, bool startInCenter);
	Field::Cell::Type getDestinationCellType(RubbishType rubbish);
private:
	Robot robot;
	Field field;
	PathGen pathGen;
};
