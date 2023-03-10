#include "Field.h"

Field::Cell::Cell()
	: type(Type::Empty), topBorder(BorderType::None), bottomBorder(BorderType::None), leftBorder(BorderType::None), rightBorder(BorderType::None)
{}

bool Field::Cell::canGoInDirection(ev3::Vector2c direction) const
{
	if (direction == ev3::Vector2c(0, 1)) { return topBorder == BorderType::None; }
	if (direction == ev3::Vector2c(1, 0)) { return rightBorder == BorderType::None; }
	if (direction == ev3::Vector2c(0, -1)) { return bottomBorder == BorderType::None; }
	if (direction == ev3::Vector2c(-1, 0)) { return leftBorder == BorderType::None; }
	return false;
}

bool Field::Cell::canDriveOnCenter() const
{
	return type != Cell::Type::Rubbish && type != Cell::Type::InterferingRubbish;
}

bool Field::Cell::hasBorders() const
{
	return topBorder == BorderType::Real || bottomBorder == BorderType::Real || leftBorder == BorderType::Real || rightBorder == BorderType::Real;
}

ev3::Vector2c Field::Cell::getBorderDirection() const
{
	if (topBorder == BorderType::Real) { return { 0, 1 }; }
	if (bottomBorder == BorderType::Real) { return { 0, -1 }; }
	if (leftBorder == BorderType::Real) { return { -1, 0 }; }
	if (rightBorder == BorderType::Real) { return { 1, 0 }; }
	return { 0, 0 };
}

Field::Field()
{
	at(0, 0).type = Cell::Type::Start;
	at(1, 0).type = Cell::Type::PaperStorage;
	at(2, 0).type = Cell::Type::BottleStorage;
	at(3, 0).type = Cell::Type::CanStorage;
	at(4, 0).type = Cell::Type::NonSortedRubbishStorage;

	for (uint8_t y = 0; y < sizeY; y++)
	{
		at(0, y).leftBorder = Cell::BorderType::Imaginary;
		at(sizeX - 1, y).rightBorder = Cell::BorderType::Imaginary;
	}

	for (uint8_t x = 0; x < sizeX; x++)
	{
		at(x, 0).bottomBorder = at(x, 0).leftBorder = at(x, 0).rightBorder = Cell::BorderType::Imaginary;
		at(x, sizeY - 1).topBorder = Cell::BorderType::Imaginary;
	}
	

	for (uint8_t x = 0; x < sizeX; x++)
	{
		at(x, 2).type = Cell::Type::Rubbish;
	}
	at(0, 1).type = at(4, 1).type = Cell::Type::Rubbish;
	at(1, 1).type = at(2, 1).type = at(3, 1).type = Cell::Type::InterferingRubbish;
}

void Field::flipBottomLine()
{
	std::swap(at(0, 0), at(4, 0));
	std::swap(at(1, 0), at(3, 0));
}

void Field::addBorder(ev3::Vector2c position, ev3::Vector2c direction)
{
	if (direction == ev3::Vector2c(0, 1) && at(position).topBorder == Cell::BorderType::None) { at(position).topBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(1, 0) && at(position).rightBorder == Cell::BorderType::None) { at(position).rightBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(0, -1) && at(position).bottomBorder == Cell::BorderType::None) { at(position).bottomBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(-1, 0) && at(position).leftBorder == Cell::BorderType::None) { at(position).leftBorder = Cell::BorderType::Real; }
	else { return; }

	position += direction;
	direction = -direction;

	if (position.x < 0 || position.x >= sizeX || position.y < 0 || position.y >= sizeY) { return; }

	if (direction == ev3::Vector2c(0, 1) && at(position).topBorder == Cell::BorderType::None) { at(position).topBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(1, 0) && at(position).rightBorder == Cell::BorderType::None) { at(position).rightBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(0, -1) && at(position).bottomBorder == Cell::BorderType::None) { at(position).bottomBorder = Cell::BorderType::Real; }
	else if (direction == ev3::Vector2c(-1, 0) && at(position).leftBorder == Cell::BorderType::None) { at(position).leftBorder = Cell::BorderType::Real; }
}

void Field::removeBorders()
{
	for (uint8_t y = 0; y < sizeY; y++)
	{
		for (uint8_t x = 0; x < sizeX; x++)
		{
			if (at(x, y).topBorder == Cell::BorderType::Real) { at(x, y).topBorder = Cell::BorderType::None; }
			if (at(x, y).bottomBorder == Cell::BorderType::Real) { at(x, y).bottomBorder = Cell::BorderType::None; }
			if (at(x, y).leftBorder == Cell::BorderType::Real) { at(x, y).leftBorder = Cell::BorderType::None; }
			if (at(x, y).rightBorder == Cell::BorderType::Real) { at(x, y).rightBorder = Cell::BorderType::None; }
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

void Field::print()
{
	ev3::Console::write("=====");
	for (uint8_t x = 0; x < sizeX; x++)
	{
		ev3::Console::write("%d %d %d", at(x, 0).type, at(x, 1).type, at(x, 2).type);
	}
	ev3::Console::write("=====");
}
