#ifndef SFMLGAME_LIGHTSOURCE_H
#define SFMLGAME_LIGHTSOURCE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "Edge.h"

class LightSource
{
 public:
  LightSource(std::shared_ptr<sf::Vector2f> _start, sf::Color _color);
  void update(float dt, const std::vector<Edge>& vector_edges);
  void setVisible(bool visibility) { visible = visibility; }
  void render(sf::RenderTexture& render_tex, const sf::View& view, sf::Shader& shader);
  void setRadius(float _radius) { radius = _radius; }

 private:
  void calculateRayCollisions(sf::Vector2f origin, const std::vector<Edge>& vector_edges);
  void createTriangles();

  std::shared_ptr<sf::Vector2f> start;
  sf::Vector2f last_position;

  std::vector<sf::Vector3f> collision_points;
  std::vector<sf::ConvexShape> triangles;
  sf::Color color;
  sf::RenderTexture myRenderTexture;

  bool visible;
  float radius;
};

#endif // SFMLGAME_LIGHTSOURCE_H
