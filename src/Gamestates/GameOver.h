#pragma once

#include "Gamestate.h"


class GameOver : public GameState
{
 public:
  GameOver(std::shared_ptr<GameData> _current_state);

  bool init();
  void update(float dt) override;
  void render(sf::RenderWindow &window) override;
  void eventHandler(sf::Event event) override;

  void stateChanged() override;

 protected:

 private:
  std::unique_ptr<sf::Text> gameOverText;
  sf::Texture winScreenTexture;
  sf::Texture loseScreenTexture;
  sf::Sprite winScreen;

};
