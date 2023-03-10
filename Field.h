#pragma once
#include "Vector2.hpp"
#include "Console.hpp"
#include <cstdint>
#include <utility>

class Field
{
public:
	class Cell
	{
	public:
		enum class Type : uint8_t { Empty = 0, Rubbish, InterferingRubbish, Start, PaperStorage, CanStorage, BottleStorage, NonSortedRubbishStorage };
		enum class BorderType : uint8_t { None, Real, Imaginary };
	public:
		Cell();
		bool canGoInDirection(ev3::Vector2c direction) const;
		bool canDriveOnCenter() const;
		bool hasBorders() const;
		ev3::Vector2c getBorderDirection() const;
	public:
		Type type;
		BorderType topBorder : 2;
		BorderType bottomBorder : 2;
		BorderType leftBorder : 2;
		BorderType rightBorder : 2;
	};
public:
	Field();
	void flipBottomLine();
	void addBorder(ev3::Vector2c position, ev3::Vector2c direction);
	void removeBorders();
	Cell& at(uint8_t x, uint8_t y);
	Cell& at(ev3::Vector2c pos);
	const Cell& at(uint8_t x, uint8_t y) const;
	const Cell& at(ev3::Vector2c pos) const;
	void print();
public:
	static constexpr uint8_t sizeX = 5;
	static constexpr uint8_t sizeY = 3;
private:
	Cell field[sizeY * sizeX];
};

