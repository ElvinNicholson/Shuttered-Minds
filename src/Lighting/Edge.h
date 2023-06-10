#ifndef SFMLGAME_EDGE_H
#define SFMLGAME_EDGE_H

#include <SFML/Graphics.hpp>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

struct Edge
{
 public:
  sf::Vector2f start;
  sf::Vector2f end;
};

struct Tile
{
 public:
  Tile()
  {
    for (int i = 0; i < 4; i++)
    {
      edge_id.emplace_back(0);
      edge_exist.emplace_back(false);
    }
  }

  std::vector<int> edge_id;
  std::vector<bool> edge_exist;
  bool is_obstacle = false;
};

#endif // SFMLGAME_EDGE_H
