#include "Field.h"

Field::Cell::Cell()
	: type(Type::Empty), topBorder(false), bottomBorder(false), leftBorder(false), rightBorder(false) 
{}

bool Field::Cell::canGoInDirection(uint8_t x, uint8_t y) const
{
	if (x == 0 && y == 1) { return !topBorder; }
	else if (x == 1 && y == 0) { return !rightBorder; }
	else if (x == 0 && y == -1) { return !bottomBorder; }
	else { return !leftBorder; }
}

bool Field::Cell::canGoInDirection(ev3::Vector2c direction) const
{
	return canGoInDirection(direction.x, direction.y);
}

Field::Field(bool mirror)
{
	at(0, 0).type = mirror ? Cell::Type::NonSortedRubbishStorage : Cell::Type::Start;
	at(1, 0).type = mirror ? Cell::Type::CanStorage : Cell::Type::PaperStorage;
	at(2, 0).type = Cell::Type::BottleStorage;
	at(3, 0).type = mirror ? Cell::Type::PaperStorage : Cell::Type::CanStorage;
	at(4, 0).type = mirror ? Cell::Type::Start : Cell::Type::NonSortedRubbishStorage;

	for (uint8_t y = 0; y < sizeY; y++)
	{
		at(0, y).leftBorder = true;
		at(sizeX - 1, y).rightBorder = true;
	}

	for (uint8_t x = 0; x < sizeX; x++)
	{
		at(x, 0).bottomBorder = at(x, 0).leftBorder = at(x, 0).rightBorder = true;
		at(x, sizeY - 1).topBorder = true;
	}
	
	for (uint8_t y = 1; y < sizeY; y++)
	{
		for (uint8_t x = 0; x < sizeX; x++)
		{
			at(x, y).type = Cell::Type::Rubbish;
		}
	}
}

Field::Cell& Field::at(uint8_t x, uint8_t y)
{
	return field[y * sizeX + x];
}

Field::Cell& Field::at(ev3::Vector2c pos)
{
	return field[pos.y * sizeX + pos.x];
}

const Field::Cell& Field::at(uint8_t x, uint8_t y) const
{
	return field[y * sizeX + x];
}

const Field::Cell& Field::at(ev3::Vector2c pos) const
{
	return field[pos.y * sizeX + pos.x];
}
