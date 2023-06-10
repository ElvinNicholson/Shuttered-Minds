#ifndef SFMLGAME_UIBUTTON_H
#define SFMLGAME_UIBUTTON_H

#include "../GameObject/GameObject.h"

class UIButton
{
 public:
  UIButton(sf::Font& font, sf::Vector2f position, std::string _text);
  void render(sf::RenderWindow& window);
  bool isClicked(sf::Vector2f cursor);

  sf::FloatRect getGlobalBounds() { return button.getGlobalBounds(); }
  void setString(std::string string) { text.setString(string); }
  void setActive(bool status) { active = status; }
  void setPosition(sf::Vector2f pos);

 private:
  sf::Texture textureButton;
  GameObject button;
  sf::Text text;
  bool active;
};

#endif // SFMLGAME_UIBUTTON_H
