#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "vector3d.hh"
#include "collision.hh"
#include "texture.hh"
#include "color.hh"

class ray;
class data;

class camera
{
public:
  camera()
  {}

  camera(float pos_x, float pos_y, float pos_z,
         float u_x, float u_y, float u_z,
         float v_x, float v_y, float v_z)
    : pos(pos_x, pos_y, pos_z), u(u_x, u_y, u_z), v(v_x, v_y, v_z)
  {}

  vector3D pos;
  vector3D u;
  vector3D v;
};


class object
{
public:
  object(float diff, float refl, float spec, unsigned int shin, color_t c,
         float refr, float opac)
    : texture_(diff, refl, spec, shin, c, refr, opac)
  {}

  virtual ~object()
  {}

  // nvcc doesn't seem to like pure virtual methods, so provide a default
  // implementation.
  virtual collision intersect(const ray& r) const = 0;

  texture_t texture_;

  enum
  {
    s, // sphere
    p, // plane
    t  // triangle
  } kind;
};

class sphere : public object
{
public:
  sphere(float diff, float refl, float spec, unsigned int shin, color_t c,
         float refr, float opac, float radius, float pos_x, float pos_y,
         float pos_z)
    : object(diff, refl, spec, shin, c, refr, opac),
      radius_(radius), pos_(pos_x, pos_y, pos_z)
  {
    kind = s;
  }

  virtual collision intersect(const ray& r) const override;

  float radius_;
  vector3D pos_;
};

class plane : public object
{
public:
  plane(float diff, float refl, float spec, unsigned int shin, color_t c,
        float refr, float opac, vector3D n, float d)
    : object(diff, refl, spec, shin, c, refr, opac),
      n(n), d(d)
  {
    kind = p;
  }

  virtual collision intersect(const ray& r) const override;

  vector3D n;
  float d;
};

class triangle : public object
{
public:
  triangle(float diff, float refl, float spec, unsigned int shin, color_t c,
           float refr, float opac, float a_x, float a_y, float a_z,
           float b_x, float b_y, float b_z, float c_x, float c_y,
           float c_z)
    : object(diff, refl, spec, shin, c, refr, opac),
      a(a_x, a_y, a_z), b(b_x, b_y, b_z), c(c_x, c_y, c_z)
  {
    kind = t;
  }

  virtual collision intersect(const ray& r) const override;

  vector3D a;
  vector3D b;
  vector3D c;
};

class light
{
public:
  light(color_t c)
    : color(c)
  {}

  virtual ~light()
  {}

  virtual float specular(const data&, const collision&) const = 0;

  virtual float factor(const collision&) const = 0;

  virtual bool in_shadow(const data& d, const collision&) const = 0;

  void apply(const data& data, color_t& c, const collision& col) const;

  color_t color;

  enum
  {
    a, // alight
    d, // dlight
    p  // plight
  } kind;
};

class alight : public light
{
public:
  alight(color_t c)
    : light(c)
  {
    kind = a;
  }

  float specular(const data&, const collision&) const override
  {
    return 0;
  }

  float factor(const collision& col) const override;

  bool in_shadow(const data&, const collision&) const override
  {
    return false;
  }
};

class dlight : public light
{
public:
  dlight(float dir_x, float dir_y, float dir_z, color_t c)
    : light(c), dir(dir_x, dir_y, dir_z)
  {
    kind = d;
  }

  float specular(const data&, const collision&) const override;

  float factor(const collision& col) const override;

  bool in_shadow(const data& d, const collision& col) const override;

  vector3D dir;
};

class plight : public light
{
public:
  plight(float pos_x, float pos_y, float pos_z, color_t c)
    : light(c), pos(pos_x, pos_y, pos_z)
  {
    kind = p;
  }

  float specular(const data&, const collision&) const override;

  float factor(const collision& col) const override;

  bool in_shadow(const data& d, const collision& col) const override;

  vector3D pos;
};

class data
{
public:
  data() : width(0), height(0)
  {}

  ~data()
  {
    for (unsigned i = 0; i < objs.size(); ++i)
      delete objs[i];
    for (unsigned i = 0; i < lights.size(); ++i)
      delete lights[i];
  }

  unsigned width;
  unsigned height;
  camera cam;
  std::vector<object*> objs;
  std::vector<light*> lights;
};
