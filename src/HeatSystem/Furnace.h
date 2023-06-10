#ifndef SFMLGAME_FURNACE_H
#define SFMLGAME_FURNACE_H

#include "../GameObject/GameObject.h"

class Furnace : public GameObject
{
 public:
  Furnace(const sf::Texture& texture, sf::Vector2f position);

  float getFuelCount() {return fuelCount;}
  float getSecondsCold() {return secondsCold;}

  void update(float dt);
  void addCoal();
  bool removeCoal();

 private:
  float secondsCold = 0;
  float fuelCount = 5;
  float decaySpeed = 0.1;
};

#endif // SFMLGAME_FURNACE_H
