//
// Created by NESco on 24/04/2023.
//

#include "UISymbol.h"

UISymbol::UISymbol(sf::Texture& texture, sf::Vector2f windowsize)
{
  sprite.setTexture(texture);
  sprite.setOrigin(sprite.getGlobalBounds().width/2,sprite.getGlobalBounds().height/2);
  sprite.setPosition(windowsize.x-100, windowsize.y-200);
  sprite.setScale(1.4,1.4);
}

void UISymbol::Changestate(bool active)
{
  if (active)
  {sprite.setColor(sf::Color(255, 255, 255, 255));}
  else
  {sprite.setColor(sf::Color(255, 255, 255, 100));}
}

void UISymbol::render(sf::RenderWindow& window)
{
  window.draw(sprite);
}