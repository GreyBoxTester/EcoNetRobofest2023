#include "app.h"
#include "Robot.h"
#include "Field.h"
#include "Path.h"
#include "PathGen.h"

void mainTask(intptr_t unused)
{
	Robot robot;
	Field field(false);
	field.at(0, 1).type = field.at(1, 1).type = Field::Cell::Type::Empty;
	PathGen pathGen;
	Path path;
	pathGen.generatePath(field, { 0, 0 }, Field::Cell::Type::PaperStorage, &path);
	for (auto p : path)
	{
		ev3::Console::write("%d %d", p.x, p.y);
	}
	robot.followPath(path.begin(), path.end(), false);
	//robot.grabAndIdentifyRubbish();
}
