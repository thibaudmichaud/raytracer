#pragma once

struct color_t
{
  unsigned r;
  unsigned g;
  unsigned b;

  color_t()
    : r(0), g(0), b(0)
  {}

  color_t(unsigned r, unsigned g, unsigned b)
    : r(r), g(g), b(b)
  {}

  color_t operator+(const color_t& o) const
  {
    color_t res;
    res.r = r + o.r;
    res.g = g + o.g;
    res.b = b + o.b;
    res.clamp();
    return res;
  }

  color_t operator*(float k) const
  {
    color_t res;
    res.r = r * k;
    res.g = g * k;
    res.b = b * k;
    res.clamp();
    return res;
  }

  color_t operator/(float k) const
  {
    color_t res;
    res.r = r / k;
    res.g = g / k;
    res.b = b / k;
    res.clamp();
    return res;
  }

  color_t operator*(const color_t& c) const
  {
    color_t res;
    res.r = r * c.r / 255;
    res.g = g * c.g / 255;
    res.b = b * c.b / 255;
    res.clamp();
    return res;
  }

  bool operator==(const color_t& c) const
  {
    return r == c.r && g == c.g && b == c.b;
  }

  unsigned clamp_minmax(unsigned v) const
  {
    if (v > 255)
      return 255;
    else
      return v;
  }

  void clamp()
  {
    r = clamp_minmax(r);
    g = clamp_minmax(g);
    b = clamp_minmax(b);
  }
};
