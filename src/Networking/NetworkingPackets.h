#pragma once

#include <SFML/Network.hpp>
#include <memory>
#include <thread>
#include <iostream>
#include <unordered_map>

enum GameEvents
{
  NONE_,
  START_GAME,
  END_GAME,
  MOVEMENT,
  COAL,
  FUEL_FURNACE,
  CONNECTION_CHECK
};

class GamePacket
{
 public:
  int clientID = 0;
  int gameEvents = 0;
  float posX = 0;
  float posY = 0;
  float posZ = 0;

  friend sf::Packet& operator <<(sf::Packet& packet, const GamePacket& gamePacket);
  friend sf::Packet& operator >>(sf::Packet& packet, GamePacket& gamePacket);
};

