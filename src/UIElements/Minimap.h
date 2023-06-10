//
// Created by E3-barrettpowe on 21/04/2023.
//

#ifndef SFMLGAME_MINIMAP_H
#define SFMLGAME_MINIMAP_H
#include <SFML/Graphics.hpp>

class Minimap
{
 public:
  Minimap(sf::Texture& texture, sf::Vector2f windowsize);
  ~Minimap();

  void render(sf::RenderWindow& window);

  void switchRender();

 private:
  sf::Sprite sprite;
  bool renderable = false;
};

#endif // SFMLGAME_MINIMAP_H
