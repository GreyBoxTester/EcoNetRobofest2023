#include "Path.h"

void printPath(const Path& path)
{
	ev3::Console::write("size: %d", path.size());
	for (auto p : path)
	{
		ev3::Console::write("%d %d", p.x, p.y);
	}
}
