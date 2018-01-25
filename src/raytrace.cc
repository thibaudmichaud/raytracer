#include <cmath>
#include <iostream>
#include <limits>

#include "light.hh"
#include "data.hh"
#include "perlin.hh"
#include "raytrace.hh"

collision collide(const data& data, const ray& r, bool stopfirst)
{
  float mindist = std::numeric_limits<float>::max();
  collision col;
  for (object* obj: data.objs)
  {
    auto curr = obj->intersect(r);
    if (curr)
    {
      float dist = norm(curr.pos - r.pos);
      if (dist < mindist)
      {
        mindist = dist;
        col = curr;
        if (stopfirst)
          return col;
      }
    }
  }
  if (col && col.texture.perlin != 0)
    {
      float r = perlin(col.pos.x, col.pos.y, col.pos.z, 0, col.texture.perlin);
      float g = perlin(col.pos.x, col.pos.y, col.pos.z, 1, col.texture.perlin);
      float b = perlin(col.pos.x, col.pos.y, col.pos.z, 2, col.texture.perlin);
      if (opts::perlin_normal)
        col.normal = normalize(col.normal + vector3D(r / 3, g / 3, b / 3));
      else
        {
          unsigned ur = (r + 1) * 0.5 * 255;
          unsigned ug = (g + 1) * 0.5 * 255;
          unsigned ub = (b + 1) * 0.5 * 255;
          col.texture.color = {ur, ug, ub};
        }
    }
  return col;
}

color_t cast(const data& d, ray r, int depth, float eta1)
{
  if (depth == 0)
    return {0, 0, 0};
  collision col = collide(d, r, false);
  if (col)
    {
      color_t res = lighting(d, col);

      ray refl(col.pos, normalize(r.dir - 2 * col.normal * r.dir * col.normal));
      res = res + cast(d, refl, depth - 1, eta1) * col.texture.refl;

      if (opts::refr)
        {
          float cosi = r.dir * col.normal;
          float eta2 = col.texture.refr;
          vector3D n = col.normal;
          if (cosi < 0)
            cosi = -cosi;
          else
            {
              std::swap(eta1, eta2);
              n = -1 * n;
            }
          float eta = eta1 / eta2;
          float k = 1 - eta * eta * (1 - cosi * cosi);
          if (k >= 0)
            {
              vector3D refr_dir = eta * r.dir + (eta * cosi - sqrtf(k)) * n;
              ray refr(col.pos, refr_dir);
              res = res + cast(d, refr, depth - 1, eta2) * (1 - col.texture.opac);
            }
        }
      return res;
    }
  else
    return {0, 0, 0};
}

void raytrace(const data& d, sf::Uint8 *pixels)
{
  auto u = normalize(d.cam.u);
  auto v = normalize(d.cam.v);
  auto w = cross(u, v);
  float l = (d.width / 2) / (tan(FOV / 2));
  auto c = d.cam.pos + l * w;
// #pragma omp parallel for
  for (unsigned i = 0; i < d.width; i++)
    for (unsigned j = 0; j < d.height; j++)
    {
      int ci = i - d.width / 2;
      int cj = j - d.height / 2;
      auto pixelpos = c + ci * u + cj * v;
      vector3D dir = normalize(pixelpos - d.cam.pos);
      color_t color;

      if (!opts::depth_of_field)
        color = cast(d, {d.cam.pos, dir}, 6, 1);
      else
      {
        //auto color2 = cast(d, {d.cam.pos, dir}, 6, 1);
        auto focal_point = dir * opts::focal_length;
        for (auto x_var: {-1, 0, 1})
          for (auto y_var: {-1, 0, 1})
          {
            auto current_pos = c + (ci + x_var * opts::focal_variation) * u +
                                   (cj + y_var * opts::focal_variation) * v;
            vector3D current_dir = normalize(current_pos - focal_point);
            auto tmp_color = cast(d, {d.cam.pos, current_dir}, 6, 1);
            color = color + tmp_color / 9.;
            /*
            if (color.r != 0)
            std::cerr << "focal " << focal_point.x << " "
                      << focal_point.y << " "
                      << focal_point.z
                      << " dir " << dir.x  << " "
                      << dir.y << " " << dir.z
                      << " new dir " << current_dir.x
                      << " " << current_dir.y << " " << current_dir.z << std::endl
                      << " color " << color.r << " " << color.g << " " << color.b;
                      */
          }
        /*
        if (color.r != 0)
        std::cerr << "var " << (color2.r - color.r)
                  << " " << (color2.g - color.g)
                  << " " << (color2.b - color.b) << std::endl;
                  */
      }

      pixels[(j * d.width + i) * 4 + 0] = color.r;
      pixels[(j * d.width + i) * 4 + 1] = color.g;
      pixels[(j * d.width + i) * 4 + 2] = color.b;
      pixels[(j * d.width + i) * 4 + 3] = 255;
    }
}
