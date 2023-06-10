#pragma once

#include <list>
#include "../Networking/NetworkingPackets.h"

class Server
{
 public:
  Server();
  void run();
  void listen(sf::TcpSocket& cSocket);
  void send(sf::Packet packet, unsigned short sender);
  void sendMyPacket(GamePacket& gamePacket);

  void setPlayerPtr(std::shared_ptr<std::unordered_map<int, GamePacket>> ptr) { otherPlayerPacket = ptr; }
  void setCoalPtr(std::shared_ptr<std::vector<sf::Vector2f>> ptr) { coalPos = ptr; }
  void setFurnacePtr(std::shared_ptr<std::vector<int>> ptr) { furnaceEvent = ptr; }

  int getClientTotal() { return connections.size(); }

 private:
  sf::Uint32 clientId;
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<sf::TcpSocket>> connections;

  std::unique_ptr<sf::TcpListener> listener;

  std::shared_ptr<std::unordered_map<int, GamePacket>> otherPlayerPacket;
  std::shared_ptr<std::vector<sf::Vector2f>> coalPos;
  std::shared_ptr<std::vector<int>> furnaceEvent;

  bool running = true;
};
