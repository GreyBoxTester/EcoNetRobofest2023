#pragma once
#include "Robot.h"
#include "Field.h"
#include "PathGen.h"

class Application : NonCopyable
{
public:
	Application();
	void waitInit() const;
	void identifyFieldSide();
	void sortFirstThree();
	void sortRubbish();
	void goToFinish();
	void goTo(Field::Cell::Type destination, bool driveToLastCellCenter, bool openGrabbers);
	void goToZigZag(Field::Cell::Type destination, bool startInCenter);
	static Field::Cell::Type getDestinationCellType(RubbishType rubbish);
	static ev3::ColorDef getDestinationCellColor(Field::Cell::Type destination);
	static void emergencyButtonHandler(intptr_t obj);
private:
	Robot robot;
	Field field;
	PathGen pathGen;
	bool initialized = false;
};
