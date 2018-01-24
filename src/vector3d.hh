#pragma once

#include <cmath>

class vector3D
{
public:
  vector3D() : x(0), y(0), z(0)
  {}

  vector3D(float x, float y, float z) : x(x), y(y), z(z)
  {}

  vector3D operator+(const vector3D& other) const
  {
    return vector3D(x + other.x,
                    y + other.y,
                    z + other.z);
  }

  vector3D
  operator-(const vector3D& other) const
  {
    return vector3D(x - other.x,
                    y - other.y,
                    z - other.z);
  }

  // Dot product
  float operator*(const vector3D& other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }

  vector3D operator*(float value) const
  {
    return {x * value, y * value, z * value};
  }

  float x;
  float y;
  float z;
};


inline
vector3D operator*(float k, const vector3D& u)
{
  return {k * u.x, k * u.y, k * u.z};
}

inline
vector3D cross(const vector3D& u, const vector3D& v)
{
  return {
    u.y * v.z - u.z * v.y,
    u.z * v.x - u.x * v.z,
    u.x * v.y - u.y * v.x,
  };
}

inline
float norm(const vector3D& u)
{
  return sqrt(u.x * u.x
              + u.y * u.y
              + u.z * u.z);
}

inline
vector3D normalize(const vector3D& u)
{
  float n = norm(u);
  return {
    u.x / n,
    u.y / n,
    u.z / n
  };
}
