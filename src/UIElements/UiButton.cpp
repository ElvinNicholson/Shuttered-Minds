#include "UIButton.h"

UIButton::UIButton(sf::Font& font, sf::Vector2f position, std::string _text)
{
  textureButton.loadFromFile("../Data/Images/UI/Button.png");
  button.setTexture(textureButton);
  button.setScale(3, 3);
  button.setOriginCentre();
  button.setPosition(position);

  text.setFont(font);
  text.setString(_text);
  text.setCharacterSize(96);
  text.setPosition(button.getPosition().x - text.getGlobalBounds().width / 2,
                   button.getPosition().y - button.getGlobalBounds().height * 0.875);
  text.setFillColor(sf::Color(62, 57, 79));

  active = true;
}

void UIButton::render(sf::RenderWindow& window)
{
  if (!active)
  {
    return;
  }

  button.renderObject(window);
  window.draw(text);
}

bool UIButton::isClicked(sf::Vector2f cursor)
{
  if (active && button.getGlobalBounds().contains(cursor.x, cursor.y))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void UIButton::setPosition(sf::Vector2f pos)
{
  button.setPosition(pos);
  text.setPosition(button.getPosition().x - text.getGlobalBounds().width / 2,
                   button.getPosition().y - button.getGlobalBounds().height * 0.875);
}
