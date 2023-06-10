#pragma once

// Game Codebase
#include "GameData.h"

class GameState
{
 public:
  GameState(std::shared_ptr<GameData> _current_state);

  virtual void update(float dt) = 0;
  virtual void render(sf::RenderWindow& window) = 0;
  virtual void eventHandler(sf::Event event) = 0;

  virtual void stateChanged() = 0;

 protected:
  std::shared_ptr<GameData> gamedata;

 private:

};
