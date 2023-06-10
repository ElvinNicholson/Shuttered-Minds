#include "Furnace.h"

Furnace::Furnace(const sf::Texture& texture, sf::Vector2f position)
{
  setTexture(texture);
  setTextureRect(sf::IntRect(0, 0, 144, 144));
  setPosition(position);
}

void Furnace::update(float dt)
{
  if (fuelCount > 0)
  {
    fuelCount -= dt * decaySpeed;
  }

  if (fuelCount <= 0)
  {
    setTextureRect(sf::IntRect(144, 0, 144, 144));
    fuelCount = 0;
    secondsCold += dt;
  }
}

void Furnace::addCoal()
{
  setTextureRect(sf::IntRect(0, 0, 144, 144));
  secondsCold = 0;
  fuelCount += 3;
  fuelCount = std::clamp(fuelCount, 0.0f, 5.0f);
}

bool Furnace::removeCoal()
{
  if (fuelCount > 0)
  {
    fuelCount -= 3;
    fuelCount = std::clamp(fuelCount, 0.0f, 5.0f);
    return true;
  }
  return false;
}
