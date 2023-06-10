#include "GameObject.h"

GameObject::GameObject()
{
  sprite = std::make_unique<sf::Sprite>();
}

void GameObject::setTexture(const sf::Texture& texture)
{
  sprite -> setTexture(texture);
}

void GameObject::setTextureRect(sf::IntRect intRect)
{
  sprite -> setTextureRect(intRect);
}

void GameObject::setPosition(float x, float y)
{
  sprite -> setPosition(x, y);
}

void GameObject::setPosition(sf::Vector2f newPos)
{
  sprite ->setPosition(newPos);
}

void GameObject::move(float x, float y)
{
  sprite -> move(x, y);
}

void GameObject::renderObject(sf::RenderWindow& renderWindow)
{
  renderWindow.draw(*sprite);
}

void GameObject::renderObject(sf::RenderTexture& renderTexture)
{
  renderTexture.draw(*sprite);
}

void GameObject::renderObject(sf::RenderTexture& renderTexture, const sf::Shader& shader)
{
  renderTexture.draw(*sprite, &shader);
}

void GameObject::setOriginCentre()
{
  sf::FloatRect spriteBounds = sprite -> getLocalBounds();

  sprite -> setOrigin(spriteBounds.width/2, spriteBounds.height/2);
}

void GameObject::setScale(float x, float y)
{
  sprite -> setScale(x, y);
}

void GameObject::setRotation(float angle)
{
  sprite -> setRotation(angle);
}

sf::Vector2f GameObject::getPosition()
{
  return sprite -> getPosition();
}

sf::Vector2i GameObject::getTilePos()
{
  sf::Vector2f spritePos = getPosition();

  return {(int)std::floor(spritePos.x/48), (int)std::floor(spritePos.y/48)};
}

sf::FloatRect GameObject::getGlobalBounds()
{
  return sprite->getGlobalBounds();
}

void GameObject::setColour(sf::Color colour)
{
  sprite->setColor(colour);
}