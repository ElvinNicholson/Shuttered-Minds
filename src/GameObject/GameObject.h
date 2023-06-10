#pragma once

// Standard Library
#include <memory>
#include <iostream>
#include <cmath>

// External Library
#include <SFML/Graphics.hpp>

class GameObject
{
 public:
  GameObject();

  void setTexture(const sf::Texture &texture);
  void setTextureRect(sf::IntRect intRect);
  void setPosition(float x, float y);
  void setPosition(sf::Vector2f newPos);
  virtual void move(float x, float y);
  virtual void renderObject(sf::RenderWindow& renderWindow);
  void renderObject(sf::RenderTexture& renderTexture);
  void renderObject(sf::RenderTexture& renderTexture, const sf::Shader& shader);
  void setOriginCentre();
  void setScale(float x, float y);
  void setRotation(float angle);
  sf::Vector2f getPosition();
  sf::Vector2i getTilePos();
  sf::FloatRect getGlobalBounds();

  void setColour(sf::Color colour);

 protected:
  std::unique_ptr<sf::Sprite> sprite;

 private:
};
