#include "app.h"
#include "Application.h"

void mainTask(intptr_t unused)
{
	Application app;
	app.sortFirstThree();
	//app.goTo(Field::Cell::Type::PaperStorage, false);
	//robot.grabAndIdentifyRubbish();
}
