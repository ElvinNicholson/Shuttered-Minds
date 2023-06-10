#pragma once

#include "../Networking/Client.h"
#include "../Networking/Server.h"

#include <SFML/Graphics.hpp>

enum State
{
  MENU,
  LOBBY,
  GAMEPLAY,
  GAME_OVER,
  NONE
};

struct NetworkingData
{
  std::unique_ptr<Server> server = nullptr;
  std::unique_ptr<Client> client = nullptr;
};

struct GameData
{
  NetworkingData networking;
  State currentState;
  bool isHost = false;
  bool win = false;

  std::shared_ptr<std::unordered_map<int, GamePacket>> otherPlayerPacket;
  std::shared_ptr<std::vector<sf::Vector2f>> coalPos;
  std::shared_ptr<std::vector<int>> furnaceEvent;

  sf::Font font;
};