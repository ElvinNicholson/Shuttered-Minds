#pragma once

#include <memory>

#include "Gamestates/Menu.h"
#include "Gamestates/Lobby.h"
#include "Gamestates/Gameplay.h"
#include "Gamestates/GameOver.h"

#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include "tmxlite/Map.hpp"
#include "vector"

#include "HeatSystem/Coal.h"
#include "HeatSystem/Furnace.h"
#include "Player/Player.h"

#include "Lighting/LightingSystem.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();

  void update(float dt);
  void render();
  void eventHandler(sf::Event event);

 private:
  bool isStateChanged();

  sf::RenderWindow& window;

  std::shared_ptr<GameData> gamedata;
  State last_state = MENU;
  std::vector<std::unique_ptr<GameState>> gamestates;
};

