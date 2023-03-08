#include "Path.h"

Path::Path(size_t size)
    : sz(size)
{}

Path::Path(const Path & other)
    : sz(other.sz)
{
    memcpy(path, other.path, sz * sizeof(ev3::Vector2c));
}

Path& Path::operator=(const Path& other)
{
    sz = other.sz;
    memcpy(path, other.path, sz * sizeof(ev3::Vector2c));
    return *this;
}

ev3::Vector2c& Path::operator[](size_t index)
{
    return path[index];
}

const ev3::Vector2c& Path::operator[](size_t index) const
{
    return path[index];
}

void Path::push(ev3::Vector2c pos)
{
    if (sz == maxSize) { return; }
    path[sz] = pos;
    sz++;
}

void Path::pop()
{
    if (sz == 0) { return; }
    sz--;
}

void Path::resize(size_t size)
{
    sz = std::min(size, maxSize);
}

void Path::clear()
{
    sz = 0;
}

size_t Path::size() const
{
    return sz;
}

ev3::Vector2c* Path::begin()
{
    return path;
}

ev3::Vector2c* Path::end()
{
    return path + sz;
}

const ev3::Vector2c* Path::begin() const
{
    return path;
}

const ev3::Vector2c* Path::end() const
{
    return path + sz;
}

ev3::Vector2c& Path::front()
{
    return path[0];
}

ev3::Vector2c& Path::back()
{
    return path[sz - 1];
}

const ev3::Vector2c& Path::front() const
{
    return path[0];
}

const ev3::Vector2c& Path::back() const
{
    return path[sz - 1];
}
