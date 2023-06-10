//
// Created by E3-barrettpowe on 21/04/2023.
//

#include "Minimap.h"

Minimap::Minimap(sf::Texture& texture, sf::Vector2f windowsize)
{
  sprite.setTexture(texture);
  sprite.setOrigin(sprite.getGlobalBounds().width/2,sprite.getGlobalBounds().height/2);
  sprite.setPosition(windowsize.x/2, windowsize.y/2);
  sprite.setScale(0.8,0.8);
}

Minimap::~Minimap()
{

}

void Minimap::render(sf::RenderWindow& window)
{
  if (renderable)
  {
    window.draw(sprite);
  }
}

void Minimap::switchRender()
{
  renderable = !renderable;
}
