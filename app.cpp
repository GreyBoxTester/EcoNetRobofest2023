#include "app.h"
#include "Robot.h"

void mainTask(intptr_t unused)
{
	Robot robot;
	std::vector<ev3::Vector2c> path = { { 0, 0 }, { 0, 1 }, { 1, 1 } };
	robot.drivePath(path.begin(), path.end(), false);
	robot.grabAndIdentifyRubbish();
}
