//#include "networking.h"
//#include "Gamestates/GameData.h"
//
//sf::Packet& operator <<(sf::Packet& packet, const packetdata& m)
//{
//  return packet << m.pos.x << m.pos.y;
//}
//sf::Packet& operator >>(sf::Packet& packet, packetdata & m)
//{
//  return packet >> m.pos.x >> m.pos.y ;
//}
//
//server::server()
//{
//  connected.clear();
//  socket.bind(5300);
//  socket.setBlocking(false);
//  std::cout << sf::IpAddress::getPublicAddress() << std::endl;
//  std::cout << sf::IpAddress::getLocalAddress() << std::endl;
//}
//
//void server::connect()
//{
//  std::cout << "Server started" << std::endl;
//  while (connected.size() != MAXPLAYERS)
//  {
//    //std::cout << "a";
//    sf::Packet packet;
//    sf::IpAddress ip;
//    unsigned short port;
//    if (socket.receive(packet,ip,port) == sf::Socket::Done)
//    {
//      //std::cout << "recived" << std::endl;
//      int type;
//      if (packet >> type)
//      {
//        if (type == CONNECT)
//        {
//          std::string name;
//          packet >> name;
//          connected.push_back({ name, ip, port });
//          std::cout << name
//                    << " connected at: " << ip << ":"
//                    << port << std::endl;
//          packet.clear();
//          packet << CONNECTED;
//          int playercount = connected.size() - 1;
//          packet << playercount;
//          for (const auto& players : connected)
//          {
//            if (players.name == name)
//              continue ;
//            packet << players.name;
//          }
//          if (socket.send(packet,ip,port) == sf::Socket::Done)
//          {
//            packet.clear();
//            packet << OTHERPLAYERCONNECTED;
//            packet << name;
//            for (const auto& player : connected)
//            {
//              socket.send(packet,player.ip,player.port);
//            }
//          }
//        }
//        else
//        {
//          //std::cout << "wrong data" << std::endl;
//        }
//      }
//      else
//      {
//        std::cout << "not a type" << std::endl;
//      }
//      //std::cout << "e";
//      //std::this_thread::sleep_for(std::chrono::milliseconds(1));
//    }
//  }
//  std::cout << "all players connected";
//  //sf::Packet packet;
//  //packet << CONNECTED;
//  //for (const auto& item : connected)
//  //{
//  //  socket.send(packet,item.ip,item.port);
//  //}
//  run();
//}
//void server::run()
//{
//  while (true)
//  {
//    sf::Packet packet;
//    connection tempconnection;
//    if (socket.receive(packet,tempconnection.ip,tempconnection.port) == sf::Socket::Done)
//    {
//      std::cout << "recived" << std::endl;
//      int type;
//      packet >> type;
//      packet >> tempconnection.name;
//      if (type == MOVEMENT)
//      {
//        packetdata pdata;
//        packet >> pdata;
//        for (const auto& player : connected)
//        {
//          sf::Packet sendP;
//          if (player.name == tempconnection.name)
//          {
//            sendP << CONNECTIONCHECK;
//            socket.send(sendP,player.ip,player.port);
//            continue ;
//          }
//
//          sendP << MOVEMENT << tempconnection.name << pdata;
//          if (socket.send(sendP,player.ip,player.port) == sf::Socket::Done)
//          {std::cout << "sent" << std::endl;}
//        }
//      }
//    }
//  }
//}
//
//void client::clientstart(std::string _name,sf::IpAddress _ip)
//{
//  name = _name;
//  std::cout << name << std::endl;
//  while (true)
//  {
//    sf::Packet connection_packet;
//    connection_packet << CONNECT << name;
//    if (
//      socket.send(connection_packet, sf::IpAddress::LocalHost, 5300) ==
//      sf::Socket::Done)
//    {
//      std::cout << "sent" << std::endl;
//    }
//    sf::Packet recpacket;
//    unsigned short port;
//    sf::IpAddress ip;
//    if (socket.receive(recpacket, ip, port) == sf::Socket::Done)
//    {
//      std::cout << "connected retuen" << std::endl;
//      int type = 0;
//      recpacket >> type;
//      if (type == CONNECTED)
//      {
//        PORT = port;
//        IP   = ip;
//        std::cout << "connected" << std::endl;
//        int i;
//        if (recpacket >> i)
//          if (i >= 1)
//            for (int j = 0; j < i; ++j)
//            {
//              std::string pname;
//              recpacket >> pname;
//              player_names.emplace_back(pname);
//            }
//        break ;
//      }
//    }
//  }
//  listener();
//}
//void client::listener()
//{
//  while (true)
//  {
//    if (tried_times >= 5)
//    {
//      data.currentState = MENU;
//    }
//
//    sf::Packet packet;
//    sf::IpAddress ip;
//    unsigned short port;
//    socket.receive(packet, ip, port);
//    int type;
//    if (packet >> type)
//    {
//      switch (type)
//      {
//        case OTHERPLAYERCONNECTED:
//        {
//          std::string pname;
//          packet >> pname;
//          player_names.emplace_back(pname);
//          break ;
//        }
//        case MOVEMENT_:
//        {
//          std::string r_name;
//          packetdata pdata;
//          packet >> r_name;
//          packet >> pdata;
//          //data.otherPlayerPacket.insert_or_assign(r_name,pdata.pos);
//          break;
//        }
//        case CONNECTIONCHECK:
//        {
//          tried_times = 0;
//          break ;
//        }
//        default:
//          break;
//      }
//    }
//  }
//}
//
//void client::send(packettype type)
//{
//  sf::Packet packet;
//  switch (type)
//  {
//    case CONNECT:
//      break;
//    case CONNECTED:
//      break;
//    case MOVEMENT_:
//    {
////      packet << MOVEMENT << name << data.thisPlayerPos.x
////             << data.thisPlayerPos.y;
//      if (socket.send(packet, IP, PORT) != sf::Socket::Done)
//      {
//      }
//      tried_times++;
//      break;
//    }
//    case OTHERPLAYERCONNECTED:
//      break;
//    case OTHERPLAYERDISCONECTED:
//      break;
//  }
//}
//