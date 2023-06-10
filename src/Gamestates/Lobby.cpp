#include "Lobby.h"
//#include "../networking.h"

Lobby::Lobby(std::shared_ptr<GameData> _current_state) :
  GameState(_current_state)
{
  background = std::make_unique<ParallaxBackground>();

  textStatus = std::make_unique<sf::Text>();
  textStatus->setFont(gamedata->font);
  textStatus->setFillColor(sf::Color::White);
  textStatus->setOutlineColor(sf::Color(62, 57, 79));
  textStatus->setOutlineThickness(2);
  textStatus->setCharacterSize(72);

  button = std::make_unique<UIButton>(gamedata->font,
                                      sf::Vector2f(0, 0),
                                      "");

  controllerCursor = std::make_unique<ControllerCursor>();
  controllerCursor->setPosition(sf::Vector2f(1080.0f / 2, 720.0f / 2));
}

void Lobby::update(float dt)
{
  background->scrollBackground(dt);

  checkGameStart();

  updateStatusText();

  if (isController)
  {
    joystickMoved();
  }
}

void Lobby::render(sf::RenderWindow& window)
{
  background->render(window);

  window.draw(*textStatus);

  button->render(window);

  if (isController)
  {
    controllerCursor->renderObject(window);
  }
}

void Lobby::eventHandler(sf::Event event)
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

void Lobby::mouseClicked(sf::Event event)
{
  if (button->isClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
  {
    if (gamedata->isHost)
    {
      sendGameStart();
    }
    else
    {
      refreshClient();
    }
  }
}

void Lobby::joystickMoved()
{
  int x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
  x = std::clamp(x / 10, -5, 5);
  int y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
  y = std::clamp(y / 10, -5, 5);

  controllerCursor->move(x, y);

  if (button->isClicked(controllerCursor->getPosition()))
  {
    controllerCursor->setInteract(true);
  }
  else
  {
    controllerCursor->setInteract(false);
  }
}

void Lobby::joystickPressed(sf::Event event)
{
  if (event.joystickButton.button == 0)
  {
    if (button->isClicked(controllerCursor->getPosition()))
    {
      if (gamedata->isHost)
      {
        sendGameStart();
      }
      else
      {
        refreshClient();
      }
    }
  }
}

/// Called when current State is changed to this
void Lobby::stateChanged()
{
  if (gamedata->isHost)
  {
    // Setup server
    setupHost();

    // Change button text
    button->setString("START");
  }
  else
  {
    // Setup client
    setupClient();

    // Change button text
    button->setString("REFRESH");
  }
  button->setActive(true);
  button->setPosition(sf::Vector2f(1080.0f / 2,
                                   720.0f / 2));
}

/// Creates a Server
void Lobby::setupHost()
{
  std::cout << "HOST" << std::endl;
  gamedata->networking.server = std::make_unique<Server>();
  gamedata->networking.server->setPlayerPtr(gamedata->otherPlayerPacket);
  gamedata->networking.server->setCoalPtr(gamedata->coalPos);
  gamedata->networking.server->setFurnacePtr(gamedata->furnaceEvent);
  std::thread serverThread ([&] { gamedata->networking.server->run(); });
  serverThread.detach();
}

/// Creates a Client
void Lobby::setupClient()
{
  //gamedata->networking.client.reset();
  std::cout << "CLIENT" << std::endl;
  gamedata->networking.client = std::make_unique<Client>();
  gamedata->networking.client->setPlayerPtr(gamedata->otherPlayerPacket);
  gamedata->networking.client->setCoalPtr(gamedata->coalPos);
  gamedata->networking.client->setFurnacePtr(gamedata->furnaceEvent);
  gamedata->networking.client->setGameDataPrt(gamedata);
  std::thread thread([&]
                     {if(gamedata->networking.client->connect())
                        {
                          gamedata->networking.client->run();
                        }
                     });
  thread.detach();

}

/// Changes the statusText displayed on the screen to display the current status
void Lobby::updateStatusText()
{
  if (gamedata->isHost)
  {
    // Host
    textStatus->setString("Connected Players: " + std::to_string(gamedata->networking.server->getClientTotal() - 1));
  }
  else
  {
    // Client
    if (gamedata->networking.client->connected)
    {
      textStatus->setString("Waiting for Host to start the game...");
    }
    else
    {
      textStatus->setString("Connection failed, no Host found.");
    }
  }

  textStatus->setPosition(1080.0f / 2 - textStatus->getGlobalBounds().width / 2,
                          200);
}

/// Only works for Host, sends a packet to clients which starts the game
void Lobby::sendGameStart()
{
  GamePacket gamePacket;
  gamePacket.gameEvents = START_GAME;
  gamedata->networking.server->sendMyPacket(gamePacket);

  // Change scene next tick instead of now so other clients can receive the packet
  startGame = true;
}

/// Try to reconnect to the server
void Lobby::refreshClient()
{
  setupClient();
}

/// Detects for when Host sends the packet to start the game
void Lobby::checkGameStart()
{
  if (!gamedata->isHost)
  {
    // Detect when Host starts the game
    for (const auto& packet : *gamedata->otherPlayerPacket)
    {
      GamePacket gamePacket = packet.second;

      if (gamePacket.gameEvents == START_GAME)
      {
        gamedata->currentState = GAMEPLAY;
      }
    }

    if (gamedata->networking.client->connected)
    {
      button->setActive(false);
    }
  }
  else
  {
    if (startGame)
    {
      gamedata->currentState = GAMEPLAY;
    }
  }
}
