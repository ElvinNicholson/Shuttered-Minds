#include "ControllerCursor.h"

ControllerCursor::ControllerCursor()
{
  cursorTexture.loadFromFile("../Data/Images/UI/Cursor.png");
  setTexture(cursorTexture);
  setTextureRect(sf::IntRect(0, 0, 24, 24));
  setScale(2, 2);
  setOriginCentre();

  canInteract = false;
}

void ControllerCursor::setInteract(bool _canInteract)
{
  canInteract = _canInteract;

  if (canInteract)
  {
    setTextureRect(sf::IntRect(24, 0, 24, 24));
  }
  else
  {
    setTextureRect(sf::IntRect(0, 0, 24, 24));
  }
}
