#pragma once

#include "texture.hh"

class collision
{
public:
  collision(vector3D p, vector3D n, texture_t t)
  : pos(p), normal(n), texture(t), valid_(true)
  {}

  collision()
  : valid_(false)
  {}

  operator bool()
  {
    return valid_;
  }

  vector3D pos;
  vector3D normal;
  texture_t texture;

private:
  bool valid_;
};
