#include "Client.h"
#include "../Gamestates/GameData.h"


Client::Client()
{
  // Randomly generate a ClientID
  clientId = std::rand();
}

/// Tries to connect to a server
bool Client::connect()
{
  socket = std::make_unique<sf::TcpSocket>();

  if(socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "Connected" << std::endl;
    return true ;
  }
  std::cout << "Failed to connect" << std::endl;
  return false;
}

void Client::run()
{
  connected = true;
  running = true;

  while (running && connected)
  {
    sf::Packet packet;
    while (connected)
    {
      auto status = socket -> receive(packet);

      if (status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "Clean Disconnection" << std::endl;
        socket->disconnect();
        break;
      }
      else
      {
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

            default:
              otherPlayerPacket->insert_or_assign(gamePacket.clientID,gamePacket);
              break;
          }
        }
      }
    }
  }
}

/// Sends a GamePacket to the server
/// @param [out] gamePacket The GamePacket to be sent
void Client::send(GamePacket& gamePacket)
{
  sf::Packet packet;
  gamePacket.clientID = clientId;
  packet << gamePacket;

  if (socket->send(packet) != sf::Socket::Done)
  {
    std::cout << "test";
    if (game_data->currentState == State::GAMEPLAY)
    {
      otherPlayerPacket->clear();
      game_data->currentState = State::MENU;
    }
  }
}
