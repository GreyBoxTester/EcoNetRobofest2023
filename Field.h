#pragma once
#include "Vector2.hpp"
#include <cstdint>
#include <utility>

class Field
{
public:
	class Cell
	{
	public:
		enum class Type : uint8_t { Empty = 0, Rubbish, InterferingRubbish, Start, PaperStorage, CanStorage, BottleStorage, NonSortedRubbishStorage };
	public:
		Cell();
		bool canGoInDirection(ev3::Vector2c direction) const;
		bool canDriveOnCenter() const;
	public:
		Type type;
		bool topBorder : 1;
		bool bottomBorder : 1;
		bool leftBorder : 1;
		bool rightBorder : 1;
	};
public:
	Field();
	void flipBottomLine();
	void addBorder(ev3::Vector2c position, ev3::Vector2c direction);
	Cell& at(uint8_t x, uint8_t y);
	Cell& at(ev3::Vector2c pos);
	const Cell& at(uint8_t x, uint8_t y) const;
	const Cell& at(ev3::Vector2c pos) const;
public:
	static constexpr uint8_t sizeX = 5;
	static constexpr uint8_t sizeY = 3;
private:
	Cell field[sizeY * sizeX];
};

