#ifndef SFMLGAME_THERMOMETER_H
#define SFMLGAME_THERMOMETER_H

#include "../GameObject/GameObject.h"

class Thermometer : public GameObject
{
 public:
  Thermometer();

  void renderObject(sf::RenderWindow& window) override;
  bool update(float dt);

  void setHeatMultiplier(float newHeat);
  void setHeatMultipler(float heatToAdd, bool addHeat);

 private:
  std::unique_ptr<sf::Texture> texture;

  sf::RectangleShape heatBar;

  float heatLevel;
  float heatMultiplier;
};

#endif // SFMLGAME_THERMOMETER_H
