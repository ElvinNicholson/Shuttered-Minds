//
// Created by NESco on 24/04/2023.
//

#ifndef SFMLGAME_UISYMBOL_H
#define SFMLGAME_UISYMBOL_H
#include <SFML/Graphics.hpp>

class UISymbol
{
 public:
  UISymbol(sf::Texture& texture, sf::Vector2f windowsize);
  void Changestate(bool active);

  void render(sf::RenderWindow& window);

 private:
  sf::Sprite sprite;
};

#endif // SFMLGAME_UISYMBOL_H
