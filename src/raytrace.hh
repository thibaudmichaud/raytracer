#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>

#include "data.hh"
#include "vector3d.hh"
#include "collision.hh"

#define FOV (45 * 3.14 / 180)

class ray
{
public:
  ray(const vector3D& pos, const vector3D& dir)
    : pos(pos), dir(dir)
  {}

  vector3D pos;
  vector3D dir;
};

collision collide(const data& data, const ray& r, bool stopfirst);

void raytrace(const data& data, sf::Uint8 *pixels);
