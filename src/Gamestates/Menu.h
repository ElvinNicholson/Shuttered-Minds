#pragma once

#include "Gamestate.h"
#include "../UIElements/UIButton.h"
#include "../UIElements/ParallaxBackground.h"
#include "../UIElements/ControllerCursor.h"
#include "../GameObject/GameObject.h"

class Menu : public GameState
{
 public:
  Menu(std::shared_ptr<GameData> _current_state);

  void update(float dt) override;
  void render(sf::RenderWindow &window) override;

  void eventHandler(sf::Event event) override;
  void mouseClicked(sf::Event event);
  void joystickMoved();
  void joystickPressed(sf::Event event);

  void stateChanged() override;

 protected:

 private:
  std::unique_ptr<UIButton> hostButton;
  std::unique_ptr<UIButton> joinButton;

  std::unique_ptr<ParallaxBackground> background;

  sf::Texture textureLogo;
  std::unique_ptr<GameObject> logo;

  std::unique_ptr<ControllerCursor> controllerCursor;
  bool isController = false;
};
