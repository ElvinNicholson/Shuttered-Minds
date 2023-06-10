#include "Thermometer.h"

Thermometer::Thermometer()
{
  texture = std::make_unique<sf::Texture>();
  texture->loadFromFile("Data/Images/thermometer.png");
  setTexture(*texture);

  sf::Vector2f windowSize = sf::Vector2f(1080, 720);
  setPosition(10, windowSize.y/2 - getGlobalBounds().height/2);

  heatBar.setSize(sf::Vector2f(4,200));
  heatBar.setFillColor(sf::Color::Red);
  heatBar.setRotation(180);
  heatBar.setPosition(sf::Vector2f(getPosition().x + getGlobalBounds().width/1.9,
                                   getPosition().y + getGlobalBounds().height/1.1));

  heatLevel = 0;
  heatMultiplier = 0;
}

void Thermometer::renderObject(sf::RenderWindow& window)
{
  GameObject::renderObject(window);
  window.draw(heatBar);
}

bool Thermometer::update(float dt)
{
  if (heatMultiplier != 0)
  {
    if (heatBar.getSize().y >= 4 && heatBar.getSize().y <= 400)
    {
      heatLevel = heatBar.getSize().y + dt * heatMultiplier;
      heatBar.setSize(sf::Vector2f(heatBar.getSize().x, heatLevel));
    }
    else if (heatBar.getSize().y < 4)
    {
      heatBar.setSize(sf::Vector2f(heatBar.getSize().x, 4));
      heatMultiplier = 0;
      return false;
    }
    else if (heatBar.getSize().y > 400)
    {
      heatBar.setSize(sf::Vector2f(heatBar.getSize().x, 400));
      heatMultiplier = 0;
    }
  }
  return true;
}

void Thermometer::setHeatMultiplier(float newHeat)
{
  heatMultiplier = newHeat;
}

void Thermometer::setHeatMultipler(float heatAddOrMinus, bool isAdding)
{
  if (isAdding)
  {
    heatMultiplier += heatAddOrMinus;
  }
  else
  {
    heatMultiplier -= heatAddOrMinus;
  }
}
