#ifndef SFMLGAME_TILEMAP_H
#define SFMLGAME_TILEMAP_H

#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Map.hpp>

#include "../GameObject/GameObject.h"

class Tilemap
{
 public:
  Tilemap();

  void render(sf::RenderTexture& renderTexLight, sf::RenderTexture& renderTexMap, sf::Vector2i viewStart);
  void renderOverhead(sf::RenderTexture& renderTexMap, sf::Vector2i viewStart);
  const std::vector<sf::Vector2i>& getWallPos() { return wallPos; }
  const std::vector<sf::Vector2i>& getBarPos() { return barPos; }
  const std::vector<sf::Vector2i>& getFloorPos() { return floorPos; }
  std::vector<std::vector<sf::FloatRect>> getWallMap();

 private:
  void loadMap();

  std::unique_ptr<sf::Texture> mapTexture;

  std::vector<std::vector<std::unique_ptr<GameObject>>> floorMap;
  std::vector<std::vector<std::unique_ptr<GameObject>>> wallMap;
  std::vector<std::vector<std::unique_ptr<GameObject>>> decorMap;
  std::vector<std::vector<std::unique_ptr<GameObject>>> overheadMap;

  std::vector<sf::Vector2i> wallPos; // Used for lighting system
  std::vector<sf::Vector2i> barPos; // Used for lighting system
  std::vector<sf::Vector2i> floorPos; // Used for coal spawning

  sf::Shader shaderRaytraced;
  sf::Shader shaderRadial;
  sf::Shader shaderTransparency;
};

#endif // SFMLGAME_TILEMAP_H
