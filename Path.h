#pragma once
#include "Field.h"
#include <algorithm>
#include <cstring>

class Path
{
public:
	Path(size_t size = 0);
	Path(const Path& other);
	Path& operator=(const Path& other);
	ev3::Vector2c& operator[](size_t index);
	const ev3::Vector2c& operator[](size_t index) const;
	void push(ev3::Vector2c pos);
	void pop();
	void resize(size_t size);
	void clear();
	size_t size() const;
	ev3::Vector2c* begin();
	ev3::Vector2c* end();
	const ev3::Vector2c* begin() const;
	const ev3::Vector2c* end() const;
	ev3::Vector2c& front();
	ev3::Vector2c& back();
	const ev3::Vector2c& front() const;
	const ev3::Vector2c& back() const;
public:
	static constexpr size_t maxSize = ((Field::sizeX > Field::sizeY ? Field::sizeX : Field::sizeY) + 1) * ((Field::sizeX < Field::sizeY ? Field::sizeX : Field::sizeY) / 2);
private:
	ev3::Vector2c path[maxSize];
	size_t sz;
};
