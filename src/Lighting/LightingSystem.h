#ifndef SFMLGAME_LIGHTINGSYSTEM_H
#define SFMLGAME_LIGHTINGSYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "Edge.h"
#include "Line.h"
#include "LightSource.h"

class LightingSystem
{
 public:
  LightingSystem(int width, int height, int _tile_size);
  void update(float dt);
  void render(sf::RenderTexture& render_tex, const sf::View& view);

  void setLightVisible(bool visibility, int index);
  void setLightRadius(float radius, int index);
  void setObstacle(int x, int y);
  void createBar(sf::Vector2i bar_pos);
  void createEdgeMap();
  void createDebugLines();
  void createLightSource(std::shared_ptr<sf::Vector2f> position, sf::Color color);

 private:
  // Raytracing stuff
  std::vector<std::vector<std::unique_ptr<Tile>>> obstacle_map;
  std::vector<Edge> vector_edges;
  std::vector<std::unique_ptr<LightSource>> light_sources;

  int tile_size;
  sf::Shader shader;

  // Debug stuff
  std::vector<Line> edge_lines;
  std::vector<sf::CircleShape> edge_dots;
};

#endif // SFMLGAME_LIGHTINGSYSTEM_H
