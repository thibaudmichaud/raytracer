#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <cmath>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.hh"
#include "ppm.hh"
#include "raytrace.hh"

#define PI 3.1415926
#define ANGLE(X) (X * PI) / 180
#define THETA ANGLE(5)

void interactive_mode(data& data)
{
  auto mode = sf::VideoMode(data.width, data.height, 32);
  sf::RenderWindow window(mode, "RayTracer");
  sf::Image image;
  sf::Sprite sprite;
  sf::Texture texture;
  texture.create(data.width, data.height);
  sf::Uint8 *pixels = new sf::Uint8[data.width * data.height * 4];
  raytrace(data, pixels);
  unsigned nframes = 0;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed)
        window.close();

    bool retrace = false;
    // Up, Down, Left, Right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      {
        data.cam.pos.y -= 1;
        retrace = true;
      }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      {
        data.cam.pos.y += 1;
        retrace = true;
      }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      {
        data.cam.pos.x -= 1;
        retrace = true;
      }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      {
        data.cam.pos.x += 1;
        retrace = true;
      }
    // Zoom in out
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      {
        data.cam.pos.z += 1;
        retrace = true;
      }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      {
        data.cam.pos.z -= 1;
        retrace = true;
      }
    // Rotation arround u
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      {
        float v_y = data.cam.v.y * cos(-THETA) - data.cam.v.z * sin(-THETA);
        float v_z = data.cam.v.y * sin(-THETA) + data.cam.v.z * cos(-THETA);
        data.cam.v.y = v_y;
        data.cam.v.z = v_z;
        retrace = true;
      }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      {
        float v_y = data.cam.v.y * cos(THETA) - data.cam.v.z * sin(THETA);
        float v_z = data.cam.v.y * sin(THETA) + data.cam.v.z * cos(THETA);
        data.cam.v.y = v_y;
        data.cam.v.z = v_z;
        retrace = true;
      }
    // Rotation arround v
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      {
        float u_x = data.cam.u.x * cos(-THETA) - data.cam.u.z * sin(-THETA);
        float u_z = data.cam.u.x * sin(-THETA) + data.cam.u.z * cos(-THETA);
        data.cam.u.x = u_x;
        data.cam.u.z = u_z;
        retrace = true;
      }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      {
        float u_x = data.cam.u.x * cos(THETA) - data.cam.u.z * sin(THETA);
        float u_z = data.cam.u.x * sin(THETA) + data.cam.u.z * cos(THETA);
        data.cam.u.x = u_x;
        data.cam.u.z = u_z;
        retrace = true;
      }

    if (retrace)
      {
        raytrace(data, pixels);
        ++nframes;
      }

    window.clear();
    image.create(data.width, data.height, pixels);
    texture.update(image);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
  }
  delete[] pixels;
}

void output_mode(data& data)
{
  // Shares buffer with cout
  std::ostream out(std::cout.rdbuf());
  sf::Uint8 *pixels = new sf::Uint8[data.width * data.height * 4];
  raytrace(data, pixels);
  output_ppm(out, data, pixels);
  delete[] pixels;
}

int main(int argc, char *argv[])
{
  std::string config_filename;
  bool interactive(false);
  char c;
  while ((c = getopt (argc, argv, "irc:")) != -1)
    switch (c)
      {
        case 'i':
          interactive = true;
          break;
        case 'c':
          config_filename = optarg;
          break;
        case 'r':
          opts::refr = true;
          break;
      }
  std::ifstream config_stream(config_filename);
  if (!config_stream)
    {
      std::cerr << "Failed to open the file." << std::endl;
      return EXIT_FAILURE;
    }
  auto data = parse(config_stream);

  if (interactive)
    interactive_mode(data);
  else
    output_mode(data);
}
