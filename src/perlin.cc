#include <cmath>
#include <map>
#include <limits>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include "vector3d.hh"

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

float interp(float x, float y, float a)
{
  float Cx = 3 * a * a - 2 * a * a * a;
  return x + Cx * (y - x);
}

float perlin(float x, float y, float z, size_t seed, float res)
{
    static std::map<size_t, std::vector<int>> perms;
    if (!perms.count(seed))
      {
        perms[seed].resize(512);
        for (int i = 0; i <= 511; ++i)
          perms[seed][i] = i / 2;
        std::random_shuffle(perms[seed].begin(), perms[seed].end());
      }
    float sqrt2 = 1.0f/sqrt(2);
    float sqrt3 = 1.0f/sqrt(3);
    std::vector<vector3D> gradients;
    for (int sgn1 : {-1, 1})
      for (int sgn2 : {-1, 1})
        {
          gradients.emplace_back(0, sgn1 * sqrt2, sgn2 * sqrt2);
          gradients.emplace_back(sgn1 * sqrt2, 0, sgn2 * sqrt2);
          gradients.emplace_back(sgn1 * sqrt2, sgn2 * sqrt2, 0);
        }
    for (int sgn1 : {-1, 1})
      for (int sgn2 : {-1, 1})
        for (int sgn3 : {-1, 1})
          gradients.emplace_back(sgn1 * sqrt3, sgn2 * sqrt3, sgn3 * sqrt3);
    for (int sgn : {-1, 1})
      {
        gradients.emplace_back(sgn, 0, 0);
        gradients.emplace_back(0, sgn, 0);
        gradients.emplace_back(0, 0, sgn);
      }

    //Adapter pour la résolution
    assert(res != 0);
    x /= res;
    y /= res;
    z /= res;

    //On récupère les positions de la grille associée à (x,y)
    long x0 = floor(x);
    long y0 = floor(y);
    long z0 = floor(z);

    //Masquage
    long i = (x0 + std::numeric_limits<long>::max() / 2) % 255;
    long j = (y0 + std::numeric_limits<long>::max() / 2) % 255;
    long k = (z0 + std::numeric_limits<long>::max() / 2) % 255;
    assert(i >= 0);
    assert(j >= 0);
    assert(k >= 0);

    //Pour récupérer les vecteurs
    int g[2][2][2];
    for (int dx : {0, 1})
      for (int dy : {0, 1})
        for (int dz : {0, 1})
          {
            g[dx][dy][dz] = perms[seed][i + dx + perms[seed][j + dy + perms[seed][k + dz]]] %
              gradients.size();
          }

    float coef[2][2][2];
    for (int dx : {0, 1})
      for (int dy : {0, 1})
        for (int dz : {0, 1})
          {
            coef[dx][dy][dz] = gradients[g[dx][dy][dz]]
              * vector3D(x - (x0 + dx), y - (y0 + dy), z - (z0 + dz));
          }

    float l00 = interp(coef[0][0][0], coef[1][0][0], x - x0);
    float l01 = interp(coef[0][0][1], coef[1][0][1], x - x0);
    float l10 = interp(coef[0][1][0], coef[1][1][0], x - x0);
    float l11 = interp(coef[0][1][1], coef[1][1][1], x - x0);

    float l0 = interp(l00, l10, y - y0);
    float l1 = interp(l01, l11, y - y0);

    return interp(l0, l1, z - z0);
}
