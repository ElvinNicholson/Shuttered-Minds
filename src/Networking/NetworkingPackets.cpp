#include "../Networking/NetworkingPackets.h"


sf::Packet& operator<<(sf::Packet& packet, const GamePacket& gamePacket)
{
  return packet << gamePacket.clientID << gamePacket.gameEvents << gamePacket.posX << gamePacket.posY << gamePacket.posZ;
}

sf::Packet& operator>>(sf::Packet& packet, GamePacket& gamePacket)
{
  return packet >> gamePacket.clientID >> gamePacket.gameEvents >> gamePacket.posX >> gamePacket.posY >> gamePacket.posZ;
}
