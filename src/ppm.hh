#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "data.hh"

void output_ppm(std::ostream& ppm_file,
                const std::vector<std::vector<color_t>> colors);

void output_ppm(std::ostream& ppm_file, const data& data, sf::Uint8 *pixels);
