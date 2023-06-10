#include "Server.h"

Server::Server()
{
  clientId = std::rand();

  if (listener == nullptr)
  {
    listener = std::make_unique<sf::TcpListener>();
  }

  if (listener->listen(53000) != sf::Socket::Done)
  {
    std::cout << "Error" << std::endl;
  }
}

void Server::run()
{
  while (running)
  {
    sf::TcpSocket& cSock = connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();

    if (listener->accept(cSock) != sf::Socket::Done)
    {
      /// Remove invalid socket from connections vector
      connections.pop_back();
      return;
    }

    std::cout << "Client connected @ " << cSock.getRemotePort() << std::endl;

    workers.emplace_back([&] {
                           listen(cSock);
                         });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Server::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;

  sf::Packet packet;

  while (continue_receiving)
  {
    auto status = cSocket.receive(packet);

    /// Handle disconnects
    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false;
      std::cout << "Disconnected" << std::endl;

      for (int i = 0; i < connections.size(); i++)
      {
        if (connections[i]->getLocalPort() == cSocket.getLocalPort())
        {
          continue;
        }

        connections.erase(std::next(connections.begin(), i));
        break;
      }
    }

    /// Receiving Packets
    GamePacket gamePacket;

    if (packet >> gamePacket)
    {
      switch (gamePacket.gameEvents)
      {
        case COAL:
          (*coalPos)[gamePacket.posZ] = sf::Vector2f(gamePacket.posX, gamePacket.posY);
          break;

        case FUEL_FURNACE:
          furnaceEvent->emplace_back(gamePacket.posX);
          break;

        case MOVEMENT:
          (*otherPlayerPacket)[gamePacket.clientID] = gamePacket;
          break;
        case START_GAME:
          otherPlayerPacket->insert_or_assign(gamePacket.clientID,gamePacket);
          break;
        default:
          std::cout << __LINE__ << "packet error" << std::endl;

          break;
      }
    }

    /// Sending data to all connections
    send(packet, cSocket.getRemotePort());
  }

  cSocket.disconnect();
}

/// Sends packets received from clients to other clients
void Server::send(sf::Packet packet, unsigned short sender)
{
  for (auto& connection : connections)
  {
    // Don't send message back to sender
    if (connection->getRemotePort() == sender)
    {
      sf::Packet check;
      check << CONNECTION_CHECK;
      connection->send(check);
    }
    else
    {
      connection->send(packet);
    }

  }
}

/// Sends GamePacket to all other client connected to host
void Server::sendMyPacket(GamePacket& gamePacket)
{
  sf::Packet packet;
  gamePacket.clientID = clientId;
  packet << gamePacket;

  for (auto& connection : connections)
  {
    connection->send(packet);
  }
}
