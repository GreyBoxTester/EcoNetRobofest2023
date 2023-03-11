#include "app.h"
#include "Application.h"

static Application app;

void mainTask(intptr_t unused)
{
	app.waitInit();
	app.identifyFieldSide();
	app.sortFirstThree();
	app.sortRubbish();
	app.goToFinish();
}
