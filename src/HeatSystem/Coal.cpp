#include "Coal.h"

Coal::Coal(const sf::Texture& texture, sf::Vector2f position)
{
  isVisible = true;
  setTexture(texture);
  setOriginCentre();
  setScale(0.5, 0.5);
  setPosition(position);
  sprite->setColor(sf::Color::Black);
}
