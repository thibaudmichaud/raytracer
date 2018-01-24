#include "ppm.hh"
#include "data.hh"

void output_ppm(std::ostream& out,
                const std::vector<std::vector<color_t>> colors)
{
  out << "P3\n";
  out << colors[0].size() << " " << colors.size() << '\n';
  out << "255\n";
  for (auto& line: colors)
  {
    for (auto& color: line)
      out << color.r << " "
                << color.g << " "
                << color.b << " ";
    out << '\n';
  }
  out << std::endl;
}

void output_ppm(std::ostream& out, const data& data, sf::Uint8 *pixels)
{
  out << "P3\n";
  out << data.width << " " << data.height << '\n';
  out << "255\n";
  for (unsigned j = 0; j < data.height; ++j)
    for (unsigned i = 0; i < data.width; ++i)
      out << static_cast<int>(pixels[(j * data.width + i) * 4 + 0]) << " "
          << static_cast<int>(pixels[(j * data.width + i) * 4 + 1]) << " "
          << static_cast<int>(pixels[(j * data.width + i) * 4 + 2]) << " ";
  out << std::endl;
}
