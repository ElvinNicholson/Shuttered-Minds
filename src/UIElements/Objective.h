#ifndef SFMLGAME_OBJECTIVE_H
#define SFMLGAME_OBJECTIVE_H

#include <SFML/Graphics.hpp>

class Objective
{
 public:
  Objective(sf::Font& font);

  void update(float dt);
  void changeObjective(sf::String newObj);
  void render(sf::RenderWindow& window);

 private:
  void updateMove(float dt);

  sf::Text preface;
  sf::Text text;
  sf::String currentObj;

  bool textMoving = false;
  bool textBigger = false;
  float changeRate = 100.5;
  float seconds = 0;
};

#endif // SFMLGAME_OBJECTIVE_H
