#include "app.h"
#include "Application.h"

void mainTask(intptr_t unused)
{
	Application app;
	app.identifyFieldSide();
	app.sortFirstThree();
	app.sortRubbish();
	app.goToFinish();
}
