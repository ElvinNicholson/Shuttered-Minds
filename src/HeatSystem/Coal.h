#ifndef SFMLGAME_COAL_H
#define SFMLGAME_COAL_H

#include "../GameObject/GameObject.h"

class Coal : public GameObject
{
 public:
  Coal(const sf::Texture& texture, sf::Vector2f position);

  bool getVisibility() { return isVisible; }
  void setVisibility(bool visibility) { isVisible = visibility; }

 private:
  bool isVisible;

};

#endif // SFMLGAME_COAL_H
