#include "Menu.h"

Menu::Menu(std::shared_ptr<GameData> _current_state) :
  GameState(_current_state)
{
  sf::Vector2f windowSize = sf::Vector2f(1080, 720);

  background = std::make_unique<ParallaxBackground>();
  textureLogo.loadFromFile("../Data/Images/UI/Logo.png");

  logo = std::make_unique<GameObject>();
  logo->setTexture(textureLogo);
  logo->setScale(2, 2);
  logo->setOriginCentre();
  logo->setPosition(sf::Vector2f(windowSize.x / 2, 150));

  hostButton = std::make_unique<UIButton>(gamedata->font,
                                          sf::Vector2f(windowSize.x / 4,windowSize.y / 2),
                                          "HOST");

  joinButton = std::make_unique<UIButton>(gamedata->font,
                                          sf::Vector2f(windowSize.x / 1.35,windowSize.y / 2),
                                          "JOIN");

  controllerCursor = std::make_unique<ControllerCursor>();
  controllerCursor->setPosition(sf::Vector2f(1080.0f / 2, 720.0f / 2));
}

void Menu::update(float dt)
{
  background->scrollBackground(dt);

  if (isController)
  {
    joystickMoved();
  }
}

void Menu::render(sf::RenderWindow& window)
{
  background->render(window);
  logo->renderObject(window);

  hostButton->render(window);
  joinButton->render(window);

  if (isController)
  {
    controllerCursor->renderObject(window);
  }
}

void Menu::eventHandler(sf::Event event)
{
  switch (event.type)
  {
    case sf::Event::MouseButtonPressed:
      mouseClicked(event);
      break;

    case sf::Event::JoystickMoved:
      isController = true;
      break;

    case sf::Event::JoystickButtonPressed:
      isController = true;
      joystickPressed(event);
      break;

    default:
      break;
  }
}

void Menu::mouseClicked(sf::Event event)
{
  if (hostButton->isClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
  {
    gamedata->isHost = true;
    gamedata->currentState = LOBBY;
  }
  else if (joinButton->isClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
  {
    gamedata->isHost = false;
    gamedata->currentState = LOBBY;
  }
}

void Menu::joystickMoved()
{
  int x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
  x = std::clamp(x / 10, -5, 5);
  int y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
  y = std::clamp(y / 10, -5, 5);

  controllerCursor->move(x, y);

  if (hostButton->isClicked(controllerCursor->getPosition()) || joinButton->isClicked(controllerCursor->getPosition()))
  {
    controllerCursor->setInteract(true);
  }
  else
  {
    controllerCursor->setInteract(false);
  }
}

void Menu::joystickPressed(sf::Event event)
{
  if (event.joystickButton.button == 0)
  {
    if (hostButton->isClicked(controllerCursor->getPosition()))
    {
      gamedata->isHost = true;
      gamedata->currentState = LOBBY;
    }
    else if (joinButton->isClicked(controllerCursor->getPosition()))
    {
      gamedata->isHost = false;
      gamedata->currentState = LOBBY;
    }
  }
}

/// Called when current State is changed to this
void Menu::stateChanged()
{

}
