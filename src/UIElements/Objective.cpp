#include "Objective.h"
#include <iostream>

Objective::Objective(sf::Font& font)
{
  preface.setFont(font);
  preface.setString("Objective: ");
  preface.setCharacterSize(30);

  text.setFont(font);
  text.setString("Survive");
  text.setCharacterSize(30);
  text.setPosition(sf::Vector2f(preface.getGlobalBounds().width * 1.01, preface.getPosition().y));
}

void Objective::update(float dt)
{
  if (textMoving)
  {
    updateMove(dt);
  }
  // Hold UI in place when at max size
  else if (!textMoving && textBigger)
  {
    seconds += dt;

    if (seconds >= 0.5 && text.getString() != currentObj)
    {
      text.setString(currentObj);
    }
    // Seconds to hold in place
    if (seconds >= 2)
    {
      textMoving = true;
      textBigger = false;
    }
  }
}

void Objective::updateMove(float dt)
{
  if (textBigger)
  {
    // Max size of text
    if (text.getCharacterSize() <= 70)
    {
      float scale =
        text.getCharacterSize() + changeRate * dt;
      text.setCharacterSize(scale);
    }
    else
    {
      textMoving = false;
    }
  }
  else
  {
    if (text.getCharacterSize() >= 30)
    {
      float scale =
        text.getCharacterSize() - changeRate * dt;
      text.setCharacterSize(scale);
    }
    else
    {
      text.setCharacterSize(30);
      textMoving = false;
    }
  }
}

void Objective::changeObjective(sf::String newObj)
{
  seconds = 0;
  currentObj = newObj;
  textMoving = true;
  textBigger = true;
}

void Objective::render(sf::RenderWindow& window)
{
  window.draw(preface);
  window.draw(text);
}
