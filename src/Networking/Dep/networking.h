//
//#pragma once
//#include <SFML/Network.hpp>
//#include <memory>
//#include <array>
//#include <iostream>
//#include <mutex>
//#include <thread>
//#include <atomic>
//
//class GameData;
//
//enum packettype
//{
//  CONNECT,
//  CONNECTED,
//  MOVEMENT_,
//  OTHERPLAYERCONNECTED,
//  OTHERPLAYERDISCONECTED,
//  CONNECTIONCHECK
//};
//
//struct packetdata
//{
//  sf::Vector2f pos;
//};
//
//
//struct connection
//{
//  std::string name;
//  sf::IpAddress ip;
//  unsigned short port;
//};
//
//
//class server
//{
// public:
//  server();
//  void connect();
//  void run();
// private:
//  std::vector<connection> connected;
//  sf::UdpSocket socket;
//
//  const int MAXPLAYERS = 4;
//};
//
//class client
//{
// public:
//  client(GameData& _data): data(_data) {}
//  void clientstart(std::string name,sf::IpAddress ip);
//  void listener();
//  void send(packettype type);
//  std::vector<std::string> player_names;
// private:
//  GameData& data;
//  sf::UdpSocket socket;
//  sf::IpAddress IP;
//  unsigned short PORT;
//  std::string name;
//  int tried_times;
//
//};