
#include "Game.h"
//#include "networking.h"
#include <iostream>
#include <SFML/Network.hpp>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(nullptr));
}

Game::~Game()
{

}

bool Game::init()
{
  // Create gamestates
  gamedata = std::make_shared<GameData>();
  gamedata->font.loadFromFile("../Data/Fonts/m5x7.ttf");

  gamedata->currentState = MENU;
  last_state = MENU;

  gamedata->otherPlayerPacket = std::make_shared<std::unordered_map<int, GamePacket>>();
  gamedata->coalPos = std::make_shared<std::vector<sf::Vector2f>>();
  for (int i = 0; i < 12; i++)
  {
    gamedata->coalPos->emplace_back();
  }
  gamedata->furnaceEvent = std::make_shared<std::vector<int>>();



  gamestates.emplace_back(std::make_unique<Menu>(gamedata));
  gamestates.emplace_back(std::make_unique<Lobby>(gamedata));
  gamestates.emplace_back(std::make_unique<Gameplay>(gamedata));
  gamestates.emplace_back(std::make_unique<GameOver>(gamedata));


  return true;
}

void Game::update(float dt)
{
  if (isStateChanged())
  {
    gamestates[gamedata->currentState]->stateChanged();
    if (gamedata->currentState == MENU)
    {
      gamestates.clear();
      gamestates.emplace_back(std::make_unique<Menu>(gamedata));
      gamestates.emplace_back(std::make_unique<Lobby>(gamedata));
      gamestates.emplace_back(std::make_unique<Gameplay>(gamedata));
      gamestates.emplace_back(std::make_unique<GameOver>(gamedata));
    }
  }

  gamestates[gamedata->currentState]->update(dt);
}

void Game::render()
{
  gamestates[gamedata->currentState]->render(window);
}

void Game::eventHandler(sf::Event event)
{
  gamestates[gamedata->currentState]->eventHandler(event);
}

bool Game::isStateChanged()
{
  if (last_state != gamedata->currentState)
  {
    last_state = gamedata->currentState;
    return true;
  }
  return false;
}
