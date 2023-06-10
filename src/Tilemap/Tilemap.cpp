#include "Tilemap.h"

Tilemap::Tilemap()
{
  shaderRaytraced.loadFromFile("Data/Shaders/RaytracedLighting.frag", sf::Shader::Fragment);
  shaderRaytraced.setUniform("texture", sf::Shader::CurrentTexture);

  shaderRadial.loadFromFile("Data/Shaders/RadialLighting.frag", sf::Shader::Fragment);
  shaderRadial.setUniform("texture", sf::Shader::CurrentTexture);

  shaderTransparency.loadFromFile("Data/Shaders/RadialTransparency.frag", sf::Shader::Fragment);
  shaderTransparency.setUniform("texture", sf::Shader::CurrentTexture);

  for (int i = 0; i < 56; i++)
  {
    floorMap.emplace_back();
    wallMap.emplace_back();
    decorMap.emplace_back();
    overheadMap.emplace_back();
  }

  loadMap();
}

void Tilemap::render(sf::RenderTexture& renderTexLight, sf::RenderTexture& renderTexMap, sf::Vector2i viewStart)
{
  shaderRaytraced.setUniform("renderTexture", renderTexLight.getTexture());

  // Occlusion culling
  for (int y = 0; y < 16; y++)
  {
    for (int x = 0; x < 24; x++)
    {
      if (!(0 <= y + viewStart.y && y + viewStart.y < 56) ||
          !(0 <= x + viewStart.x && x + viewStart.x < 49))
      {
        // Index is outside of range
        continue;
      }

      if (floorMap[y + viewStart.y][x + viewStart.x])
      {
        floorMap[y + viewStart.y][x + viewStart.x]->renderObject(renderTexMap, shaderRaytraced);
      }

      if (decorMap[y + viewStart.y][x + viewStart.x])
      {
        decorMap[y + viewStart.y][x + viewStart.x]->renderObject(renderTexMap, shaderRaytraced);
      }

      if (wallMap[y + viewStart.y][x + viewStart.x])
      {
        wallMap[y + viewStart.y][x + viewStart.x]->renderObject(renderTexMap, shaderRadial);
      }
    }
  }
}

void Tilemap::renderOverhead(sf::RenderTexture& renderTexMap, sf::Vector2i viewStart)
{
  // Occlusion culling
  for (int y = 0; y < 16; y++)
  {
    for (int x = 0; x < 24; x++)
    {
      if (!(0 <= y + viewStart.y && y + viewStart.y < 56) ||
          !(0 <= x + viewStart.x && x + viewStart.x < 49))
      {
        // Index is outside of range
        continue;
      }

      if (overheadMap[y + viewStart.y][x + viewStart.x])
      {
        overheadMap[y + viewStart.y][x + viewStart.x]->renderObject(renderTexMap, shaderTransparency);
      }
    }
  }
}

void Tilemap::loadMap()
{
  int size = 48;

  mapTexture = std::make_unique<sf::Texture>();
  if(!mapTexture->loadFromFile("Data/Map/Tilesheet_transparent.png"))
  {
    std::cout << "FAILED TO LOAD Tilesheet_transparent.png" << std::endl;
  }

  tmx::Map map;
  if(!map.load("Data/Map/Shuttered_Map.tmx"))
  {
    std::cout << "FAILED TO LOAD Shuttered_Map.tmx" << std::endl;
  }

  const auto& layers = map.getLayers();
  for (const auto & layer : layers)
  {
    const auto tile_layer = layer->getLayerAs<tmx::TileLayer>();

    const auto& tiles = tile_layer.getTiles();
    int idx = 0;
    int idy = 0;

    int index = 0;
    for (auto& tile : tiles)
    {
      idy = index / 49;
      idx = index % 49;

      int texture_x = tile.ID % 56 - 1;
      int texture_y = floor(tile.ID / 56);

      std::string layerName = layer->getName();

      if (layerName == "Floor")
      {
        if (tile.ID == 0)
        {
          index++;
          floorMap[idy].emplace_back();
          continue;
        }

        floorMap[idy].emplace_back(std::make_unique<GameObject>());
        floorMap[idy].back()->setTexture(*mapTexture);
        floorMap[idy].back()->setPosition(idx * size, idy * size);
        floorMap[idy].back()->setTextureRect(sf::IntRect(texture_x * 24, texture_y * 24, 24, 24));
        floorMap[idy].back()->setScale(2, 2);

        floorPos.emplace_back(sf::Vector2i((idx * size) + 24, (idy * size) + 24));
      }
      else if (layerName == "Wall")
      {
        if (tile.ID == 0)
        {
          index++;
          wallMap[idy].emplace_back();
          continue;
        }

        wallMap[idy].emplace_back(std::make_unique<GameObject>());
        wallMap[idy].back()->setTexture(*mapTexture);
        wallMap[idy].back()->setPosition(idx * size, idy * size);
        wallMap[idy].back()->setTextureRect(sf::IntRect(texture_x * 24, texture_y * 24, 24, 24));
        wallMap[idy].back()->setScale(2, 2);

        if (962 <= tile.ID && tile.ID <= 979)
        {
          // is a bar
          barPos.emplace_back(sf::Vector2i(idx, idy));
        }
        else
        {
          // is a wall
          wallPos.emplace_back(sf::Vector2i(idx, idy));
        }
      }
      else if (layerName == "Overhead")
      {
        if (tile.ID == 0)
        {
          index++;
          overheadMap[idy].emplace_back();
          continue;
        }

        overheadMap[idy].emplace_back(std::make_unique<GameObject>());
        overheadMap[idy].back()->setTexture(*mapTexture);
        overheadMap[idy].back()->setPosition(idx * size, idy * size);
        overheadMap[idy].back()->setTextureRect(sf::IntRect(texture_x * 24, texture_y * 24, 24, 24));
        overheadMap[idy].back()->setScale(2, 2);
      }
      else if (layerName == "Decor")
      {
        if (tile.ID == 0)
        {
          index++;
          decorMap[idy].emplace_back();
          continue;
        }

        decorMap[idy].emplace_back(std::make_unique<GameObject>());
        decorMap[idy].back()->setTexture(*mapTexture);
        decorMap[idy].back()->setPosition(idx * size, idy * size);
        decorMap[idy].back()->setTextureRect(sf::IntRect(texture_x * 24, texture_y * 24, 24, 24));
        decorMap[idy].back()->setScale(2, 2);
      }

      index++;
    }
  }
}

std::vector<std::vector<sf::FloatRect>> Tilemap::getWallMap()
{
  std::vector<std::vector<sf::FloatRect>> wallRects;

  wallRects.reserve(56);

  for (int i = 0; i < 56; i++)
  {
    wallRects.emplace_back();
  }

  int y = 0;
  for (auto& wall_layer : wallMap)
  {
    for (auto& wall : wall_layer)
    {
      if (wall)
      {
        wallRects[y].emplace_back(wall->getGlobalBounds());
      }
      else
      {
        wallRects[y].emplace_back(sf::FloatRect());
      }
    }
    y++;
  }

  return wallRects;
}
