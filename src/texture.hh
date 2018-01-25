#pragma once

#include "color.hh"

class texture_t
{
public:
  texture_t(float diff, float refl, float spec, float shin, color_t c,
         float refr, float opac, float perlin)
    : diff(diff), refl(refl), spec(spec), shin(shin),
      color(c), refr(refr), opac(opac), perlin(perlin)
  {
    assert(shin >= 0);
    assert(refr >= 0);
    assert(opac >= 0 && opac <= 1);
  }

  texture_t() = default;

  float diff;
  float refl;
  float spec;
  float shin;
  color_t color;
  float refr;
  float opac;
  float perlin;
};


