#include "app.h"
#include "Application.h"

void mainTask(intptr_t unused)
{
	Application app;
	app.sortRubbish();
	app.goToFinish();
}
