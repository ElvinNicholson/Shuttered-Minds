#pragma once

#include <atomic>
#include "../Networking/NetworkingPackets.h"

struct GameData;


class Client
{
 public:
  Client();

  bool connect();
  void run();
  void send(GamePacket& gamePacket);

  void setPlayerPtr(std::shared_ptr<std::unordered_map<int, GamePacket>> ptr) { otherPlayerPacket = ptr; }
  void setCoalPtr(std::shared_ptr<std::vector<sf::Vector2f>> ptr) { coalPos = ptr; }
  void setFurnacePtr(std::shared_ptr<std::vector<int>> ptr) { furnaceEvent = ptr; }
  void setGameDataPrt(std::shared_ptr<GameData> ptr) {game_data = ptr; }
  std::atomic<bool> running = false;
  std::atomic<bool> connected = false;

 private:
  int clientId;
  std::unique_ptr<sf::TcpSocket> socket;

  std::shared_ptr<std::unordered_map<int, GamePacket>> otherPlayerPacket;
  std::shared_ptr<std::vector<sf::Vector2f>> coalPos;
  std::shared_ptr<std::vector<int>> furnaceEvent;
  std::shared_ptr<GameData> game_data;
};
