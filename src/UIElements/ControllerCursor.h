#ifndef SFMLGAME_CONTROLLERCURSOR_H
#define SFMLGAME_CONTROLLERCURSOR_H

#include "../GameObject/GameObject.h"

class ControllerCursor : public GameObject
{
 public:
  ControllerCursor();
  void setInteract(bool _canInteract);

 private:
  sf::Texture cursorTexture;

  bool canInteract;
};

#endif // SFMLGAME_CONTROLLERCURSOR_H
