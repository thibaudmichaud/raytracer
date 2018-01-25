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
#include "perlin.hh"

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

  constexpr double speed = 6.;
  constexpr double rotation_coef = 1 / 400.;
  constexpr auto pi_over_two = std::acos(0);

  const vector3D init_pos = data.cam.pos;
  const vector3D init_right = data.cam.u;
  const vector3D init_up = data.cam.v;
  const vector3D init_front = cross(data.cam.v, data.cam.u);;

  vector3D front = init_front;
  double yaw_roll = -pi_over_two;
  double pitch_roll = 0.;
  auto previous_mouse = sf::Mouse::getPosition();

  sf::Clock clock;


  while (window.isOpen())
  {
    bool retrace = false;
    sf::Event event;
    auto delta_time = clock.getElapsedTime().asSeconds();
    clock.restart();

    while (window.pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed:
        {
          window.close();
          break;
        }
        case sf::Event::KeyPressed:
        {
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          {
            window.close();
            break;
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          {
            data.cam.pos = data.cam.pos - front * speed * delta_time;
            retrace = true;
          }

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          {
            data.cam.pos = data.cam.pos + front * speed * delta_time;
            retrace = true;
          }

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          {
            data.cam.pos = data.cam.pos - data.cam.u * speed * delta_time;
            retrace = true;
          }

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          {
            data.cam.pos = data.cam.pos + data.cam.u * speed * delta_time;
            retrace = true;
          }

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
          {
            yaw_roll = -pi_over_two;
            pitch_roll = 0.;
            data.cam.pos = init_pos;
            data.cam.u = init_right;
            data.cam.v = init_up;
            retrace = true;
          }

          break;
        }

        case sf::Event::MouseMoved:
        {
          //FIXME NEED to be proofread by aga
          auto new_position = sf::Mouse::getPosition();
          if (new_position != previous_mouse)
          {
            auto delta_x = new_position.x - previous_mouse.x;
            auto delta_y = new_position.y - previous_mouse.y;

            yaw_roll -= delta_x * rotation_coef;
            pitch_roll -= delta_y * rotation_coef;
            pitch_roll = std::max(std::min(pi_over_two, pitch_roll),
                                  -pi_over_two);

            auto cos_pitch = std::cos(pitch_roll);
            auto sin_pitch = std::sin(pitch_roll);
            auto cos_yaw = std::cos(yaw_roll);
            auto sin_yaw = std::sin(yaw_roll);
            front.x = cos_yaw * cos_pitch;
            front.y = sin_pitch;
            front.z = sin_yaw * cos_pitch;
            front = normalize(front);
            data.cam.u = normalize(cross(front, init_up));
            data.cam.v = normalize(cross(data.cam.u, front));
            retrace = true;
            previous_mouse = new_position;
          }
          break;
        }

        default:
        {
        }
      }
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

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed)
        window.close();


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
  while ((c = getopt (argc, argv, "irnc:")) != -1)
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
        case 'n':
          opts::perlin_normal = true;
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
