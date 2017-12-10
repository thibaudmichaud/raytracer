#include "data.hh"

color_t lighting(const data& d, const collision& col)
{
  color_t res{0, 0, 0};
  for (light* light: d.lights)
    light->apply(d, res, col);
  res.clamp();
  return res;
}
