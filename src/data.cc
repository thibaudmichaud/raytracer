#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>

#include "data.hh"
#include "raytrace.hh"

#define EPSILON 0.01
#define LA 0.2

collision sphere::intersect(const ray& r) const
{
  float a = r.dir * r.dir;
  float b = (2 * r.dir) * (r.pos - pos_);
  vector3D n = r.pos - pos_;
  float c = n * n - radius_ * radius_;
  float delta = b * b - 4 * a * c;
  collision col;
  if (delta >= 0)
  {
    float t0 = (- b + sqrt(delta)) / (2 * a);
    float t1 = (- b - sqrt(delta)) / (2 * a);
    float t = (t0 < t1 && t0 > EPSILON) ? t0 :
      (t1 > EPSILON ? t1 : t0);
    if (t > EPSILON)
    {
      col = collision{
        r.pos + t * r.dir,
        normalize((r.pos + t * r.dir) - pos_),
        texture_
      };
    }
  }
  return col;
}

collision plane::intersect(const ray& r) const
{
  float denom = n * r.dir;
  collision col;
  if (denom != 0)
  {
    float t = - (n * r.pos + d) / (n * r.dir);
    if (t > EPSILON)
    {
      col = collision{
        r.pos + t * r.dir,
        n,
        texture_
      };
    }
  }
  return col;
}

collision triangle::intersect(const ray& r) const
{
  vector3D ab = b - a;
  vector3D ac = c - a;
  vector3D norm = cross(ab, ac);
  float d = norm.x * a.x
             - norm.y * a.y
             - norm.z * a.z;
  plane test(0, 0, 0, 0, {0, 0, 0}, 0, 0, norm, d);
  collision col = test.intersect(r);
  if (col)
  {
    col.texture = texture_;
    vector3D m = col.pos;
    vector3D am = m - a;
    vector3D bm = m - b;
    vector3D cm = m - c;
    if (!((cross(am, bm).z >= 0
            && cross(bm, cm).z >= 0
            && cross(cm, am).z >= 0)
          || (cross(am, bm).z <= 0
            && cross(bm, cm).z <= 0
            && cross(cm, am).z <= 0)))
    {
      col = {};
    }
  }
  return col;
}

void light::apply(const data& da, color_t& c, const collision& col) const
{
  if (!in_shadow(da, col))
    {
      switch (kind)
      {
        case a:
          c = c + (color * col.texture.color) * factor(col);
          break;
        case d:
        case p:
          float ld = factor(col);
          float ls = specular(da, col);
          c = c + (color * col.texture.color) * ld
                + color * ls
                + col.texture.color * color * (ld + 0.05) + color_t(ls, ls, ls);
          break;
      }
    }
}

float alight::factor(const collision&) const
{
  return LA;
}

bool dlight::in_shadow(const data& d, const collision& col) const
{
  return collide(d, {col.pos, -1 * dir}, true);
}

bool plight::in_shadow(const data& d, const collision& col) const
{
  collision col2 = collide(d, {col.pos, normalize(pos - col.pos)}, true);
  return col2 && norm(col2.pos - col.pos) < norm(pos - col.pos);
}

float dlight::factor(const collision& col) const
{
  float ld = col.texture.diff * (-1 * dir) * col.normal;
  if (ld < 0)
    ld = 0;
  return ld;
}

float plight::factor(const collision& col) const
{
  vector3D dir = col.pos - pos;
  float ld = col.texture.diff * (1 / norm(dir)) * normalize(-1 * dir) * col.normal;
  if (ld < 0)
    ld = 0;
  return ld;
}

float dlight::specular(const data& d, const collision& col) const
{
  vector3D i = -1 * dir;
  vector3D n = normalize(col.normal);
  return i * n < 0 ? 0 :
    pow(normalize(i - (2 * (n * i) * n))
        * normalize(d.cam.pos - col.pos), col.texture.shin)
    * col.texture.spec;
}

float plight::specular(const data& d, const collision& col) const
{
  vector3D i = -1 * normalize(col.pos - pos);
  vector3D n = col.normal;
  return i * n < 0 ? 0 :
    pow(normalize(i - (2 * (n * i) * n))
        * normalize(d.cam.pos - col.pos), col.texture.shin)
    * col.texture.spec;
}
