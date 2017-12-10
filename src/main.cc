#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "parser.hh"
#include "raytrace.hh"

void output_ppm(std::ostream& out, const data& d, uint8_t *pixels)
{
  out << "P3\n";
  out << d.width << " " << d.height << '\n';
  out << "255\n";
  for (unsigned j = 0; j < d.height; ++j)
    {
      for (unsigned i = 0; i < d.width; ++i)
        out << static_cast<unsigned>(pixels[(j * d.width + i) * 4 + 0]) << " "
            << static_cast<unsigned>(pixels[(j * d.width + i) * 4 + 1]) << " "
            << static_cast<unsigned>(pixels[(j * d.width + i) * 4 + 2]) << " ";
      std::cout << std::endl;
    }
  out << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
      std::cerr << "Usage: ./rt <input-file>\n";
      return EXIT_FAILURE;
    }
  std::ifstream config_file(argv[1]);
  if (!config_file)
    {
      std::cerr << "Failed to open the file." << std::endl;
      return EXIT_FAILURE;
    }

  data d = parse(config_file);

  uint8_t *pixels = new uint8_t[d.width * d.height * 4];
  raytrace(d, pixels);
  output_ppm(std::cout, d, pixels);
  delete[] pixels;
}
