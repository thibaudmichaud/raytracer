#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "parser.hh"

// Safe getLine crossplatform coming from
// https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& safeGetline(std::istream& is, std::string& t)
{
  t.clear();

  // The characters in the stream are read one-by-one using a std::streambuf.
  // That is faster than reading them one-by-one using the std::istream.
  // Code that uses streambuf this way must be guarded by a sentry object.
  // The sentry object performs various tasks,
  // such as thread synchronization and updating the stream state.

  std::istream::sentry se(is, true);
  std::streambuf* sb = is.rdbuf();

  for(;;) {
    int c = sb->sbumpc();
    switch (c)
    {
      case '\n':
        return is;
      case '\r':
        if(sb->sgetc() == '\n')
          sb->sbumpc();
        return is;
      case EOF:
        // Also handle the case when the last line has no line ending
        if(t.empty())
          is.setstate(std::ios::eofbit);
        return is;
      default:
        t += (char)c;
    }
  }
}

void parse_screen(std::istringstream& iss, data& data)
{
  iss >> data.width;
  iss >> data.height;
}

void parse_camera(std::istringstream& iss, data& data)
{
  iss >> data.cam.pos.x;
  iss >> data.cam.pos.y;
  iss >> data.cam.pos.z;
  iss >> data.cam.u.x;
  iss >> data.cam.u.y;
  iss >> data.cam.u.z;
  iss >> data.cam.v.x;
  iss >> data.cam.v.y;
  iss >> data.cam.v.z;
}

void parse_sphere(std::istringstream& iss, data& data)
{
  float radius;
  float pos_x, pos_y, pos_z;
  float diff, refl, spec, shin;
  unsigned color_r, color_g, color_b;
  float refr, opac, perlin;
  iss >> radius;
  iss >> pos_x;
  iss >> pos_y;
  iss >> pos_z;
  iss >> diff;
  iss >> refl;
  iss >> spec;
  iss >> shin;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  iss >> refr;
  iss >> opac;
  iss >> perlin;
  data.objs.push_back(new sphere(diff, refl, spec, shin, {color_r, color_g,
                                 color_b}, refr, opac, radius, pos_x, pos_y,
                                 pos_z, perlin));
}

void parse_plane(std::istringstream& iss, data& data)
{
  float a, b, c, d;
  float diff, refl, spec, shin;
  unsigned color_r, color_g, color_b;
  float refr, opac, perlin;
  iss >> a;
  iss >> b;
  iss >> c;
  iss >> d;
  iss >> diff;
  iss >> refl;
  iss >> spec;
  iss >> shin;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  iss >> refr;
  iss >> opac;
  iss >> perlin;
  data.objs.push_back(new plane(diff, refl, spec, shin, {color_r, color_g,
                                color_b}, refr, opac, normalize({a, b, c}), d, perlin));
}

void parse_triangle(std::istringstream& iss, data& data)
{
  float a_x, a_y, a_z;
  float b_x, b_y, b_z;
  float c_x, c_y, c_z;
  float diff, refl, spec, shin;
  unsigned color_r, color_g, color_b;
  float refr, opac;
  iss >> a_x;
  iss >> a_y;
  iss >> a_z;
  iss >> b_x;
  iss >> b_y;
  iss >> b_z;
  iss >> c_x;
  iss >> c_y;
  iss >> c_z;
  iss >> diff;
  iss >> refl;
  iss >> spec;
  iss >> shin;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  iss >> refr;
  iss >> opac;
  data.objs.push_back(new triangle(diff, refl, spec, shin, {color_r, color_g,
                                   color_b}, refr, opac, a_x, a_y, a_z, b_x,
                                   b_y, b_z, c_x, c_y, c_z));
}

void parse_alight(std::istringstream& iss, data& data)
{
  unsigned color_r, color_g, color_b;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  data.lights.push_back(new alight({color_r, color_g, color_b}));
}

void parse_dlight(std::istringstream& iss, data& data)
{
  float dir_x, dir_y, dir_z;
  unsigned color_r, color_g, color_b;
  iss >> dir_x;
  iss >> dir_y;
  iss >> dir_z;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  data.lights.push_back(new dlight(dir_x, dir_y, dir_z, {color_r, color_g,
                                   color_b}));
}

void parse_plight(std::istringstream& iss, data& data)
{
  float pos_x, pos_y, pos_z;
  unsigned color_r, color_g, color_b;
  iss >> pos_x;
  iss >> pos_y;
  iss >> pos_z;
  iss >> color_r;
  iss >> color_g;
  iss >> color_b;
  data.lights.push_back(new plight(pos_x, pos_y, pos_z, {color_r, color_g,
                                   color_b}));
}

data parse(std::ifstream& config_file)
{
  data data;

  std::string line;
  while (safeGetline(config_file, line))
    {
      std::istringstream iss(line);
      std::string s;
      if (iss >> s)
        {
          if (s == "screen")
            parse_screen(iss, data);
          else if (s == "sphere")
            parse_sphere(iss, data);
          else if (s == "camera")
            parse_camera(iss, data);
          else if (s == "sphere")
            parse_sphere(iss, data);
          else if (s == "plane")
            parse_plane(iss, data);
          else if (s == "triangle")
            parse_triangle(iss, data);
          else if (s == "alight")
            parse_alight(iss, data);
          else if (s == "dlight")
            parse_dlight(iss, data);
          else if (s == "plight")
            parse_plight(iss, data);
        }
    }

  return data;
}
