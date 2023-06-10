#pragma once

#include "Gamestate.h"
#include "../Tilemap/Tilemap.h"
#include "../Player/Player.h"
#include "../HeatSystem/Coal.h"
#include "../HeatSystem/Furnace.h"
#include "../HeatSystem/Thermometer.h"
#include "../Lighting/LightingSystem.h"
#include "../UIElements/Objective.h"
#include "../Audio/AudioManager.h"
#include "../UIElements/Minimap.h"
#include "../UIElements/UISymbol.h"


class Gameplay : public GameState
{
 public:
  Gameplay(std::shared_ptr<GameData> _current_state);
  bool init();
  void update(float dt) override;
  void render(sf::RenderWindow &window) override;
  void eventHandler(sf::Event event) override;

  void stateChanged() override;

  bool win;

 protected:

 private:
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void joystickPressed(sf::Event event);
  void joystickMoved(sf::Event event);

  bool pickUpCoal();
  bool fuelFurnace();
  bool robFurnace();

  void setTemperature(float dt);
  void setMadTemp(float dt);
  void updateFurnaceLighting();

  void updateOtherPlayers(float dt);
  void updateCoalPos();
  void sendMyPacket();

  // Window views
  sf::View viewPlayer;

  // Tilemap
  std::unique_ptr<Tilemap> tileMap;

  // Player
  float leftRight;
  float upDown;
  std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<Player>> otherPlayers;

  // Heat System
  sf::Texture textureCoal;
  std::vector<std::unique_ptr<Coal>> coals;

  sf::Texture textureFurnace;
  std::vector<std::unique_ptr<Furnace>> furnaces;
  std::vector<std::shared_ptr<sf::Vector2f>> furnacePos; // Used for lighting system's light sources

  // UI Elements
  std::unique_ptr<Thermometer> thermometer;
  std::unique_ptr<Objective> objectiveText;

  std::unique_ptr<sf::Text> coalCount;
  std::unique_ptr<sf::Text> timer;
  float gameplayTimer;
  sf::Texture miniMapTex;
  std::unique_ptr<Minimap> miniMap;
  sf::Texture iconTex;
  std::unique_ptr<UISymbol> pickprompt;

  // Lighting System
  std::unique_ptr<LightingSystem> lightingSystem;
  std::shared_ptr<sf::Vector2f> playerPos;

  sf::RenderTexture renderTexLight; // The lighting system will bake the light onto this texture
  sf::RenderTexture renderTexMap; // The map is rendered onto this texture

  sf::Shader shaderObjects; // The shader used for rendering objects based on lighting

  // Audio
  std::unique_ptr<AudioManager> audiomanager;

  float monster_timer;

};
