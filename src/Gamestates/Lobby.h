#ifndef SFMLGAME_GAMESTATE_LOBBY_H
#define SFMLGAME_GAMESTATE_LOBBY_H

#include "Gamestate.h"
#include "../UIElements/ParallaxBackground.h"
#include "../UIElements/UIButton.h"
#include "../UIElements/ControllerCursor.h"

class Lobby : public GameState
{
 public:
  Lobby(std::shared_ptr<GameData> _current_state);

  void update(float dt) override;
  void render(sf::RenderWindow &window) override;

  void eventHandler(sf::Event event) override;
  void mouseClicked(sf::Event event);
  void joystickMoved();
  void joystickPressed(sf::Event event);

  void stateChanged() override;
  
 protected:

 private:
  void setupHost();
  void setupClient();

  void sendGameStart();
  void checkGameStart();
  void refreshClient();

  void updateStatusText();

  std::unique_ptr<ParallaxBackground> background;
  std::unique_ptr<sf::Text> textStatus;
  std::unique_ptr<UIButton> button;

  bool startGame = false;

  std::unique_ptr<ControllerCursor> controllerCursor;
  bool isController = false;
};

#endif // SFMLGAME_GAMESTATE_LOBBY_H
