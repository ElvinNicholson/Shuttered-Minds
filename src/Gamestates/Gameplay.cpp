#include "Gameplay.h"
#include <cmath>
Gameplay::Gameplay(std::shared_ptr<GameData> _current_state) :
  GameState(_current_state)
{
  init();
}

bool Gameplay::init()
{
  gameplayTimer = 300.0f;

  // Player
  leftRight = 0;
  upDown    = 0;
  player = std::make_unique<Player>();

  // Window views
  sf::Vector2f windowSize = sf::Vector2f (1080, 720);
  viewPlayer = sf::View(player->getPosition(), windowSize);

  // Tilemap
  tileMap = std::make_unique<Tilemap>();
  player->SetWalls(tileMap->getWallMap());

  // Heat System
  textureCoal.loadFromFile("Data/Images/coin.png");
  for (int i = 0; i < 12; i++)
  {
    coals.emplace_back(std::make_unique<Coal>(textureCoal, sf::Vector2f(0, 0)));
  }

  textureFurnace.loadFromFile("Data/Images/Furnace.png");
  furnacePos.emplace_back(std::make_shared<sf::Vector2f>(sf::Vector2f(1848, 168))); // Top Right
  furnacePos.emplace_back(std::make_shared<sf::Vector2f>(sf::Vector2f(120, 744))); // Top Left
  furnacePos.emplace_back(std::make_shared<sf::Vector2f>(sf::Vector2f(1080, 1416))); // Middle
  furnacePos.emplace_back(std::make_shared<sf::Vector2f>(sf::Vector2f(1800, 2424))); // Bottom Right
  furnacePos.emplace_back(std::make_shared<sf::Vector2f>(sf::Vector2f(482, 2502))); // Bottom Left

  for (auto& pos : furnacePos)
  {
    furnaces.emplace_back(std::make_unique<Furnace>(textureFurnace, sf::Vector2f(pos->x - 72, pos->y - 120)));
  }

  coalCount = std::make_unique<sf::Text>();
  coalCount->setFont(gamedata->font);
  coalCount->setString("Coal: " +  std::to_string(player->getCoalCount()));
  coalCount->setPosition(0, windowSize.y/20);
  timer = std::make_unique<sf::Text>();
  timer->setFont(gamedata->font);
  timer->setString(std::to_string(gameplayTimer));
  timer->setPosition(windowSize.x - 67, 2);

  thermometer =  std::make_unique<Thermometer>();

  // Lighting System
  lightingSystem = std::make_unique<LightingSystem>(49, 56, 48);
  renderTexLight.create(1080, 720);
  renderTexMap.create(1080, 720);

  // Player Lighting
  playerPos = std::make_shared<sf::Vector2f>(0, 0);
  player->setPosition(300,300);
  lightingSystem->createLightSource(playerPos, sf::Color(255, 255, 255));

  // Furnace Lighting
  for (auto& pos : furnacePos)
  {
    lightingSystem->createLightSource(pos, sf::Color(255, 222, 160));
  }

  // Initialize the shader used for rendering objects based on lighting
  shaderObjects.loadFromFile("Data/Shaders/ObjectLighting.frag", sf::Shader::Fragment);

  // Get wall position from tilemap for lighting system
  for (auto& wallPos : tileMap->getWallPos())
  {
    lightingSystem->setObstacle(wallPos.x, wallPos.y);
  }

  // Create a list of edges based on the walls
  lightingSystem->createEdgeMap();

  // Get bars position from tilemap for lighting system
  for (auto& barPos : tileMap->getBarPos())
  {
    lightingSystem->createBar(barPos);
  }

  // Other stuff
  objectiveText = std::make_unique<Objective>(gamedata->font);
  audiomanager = std::make_unique<AudioManager>();

  miniMapTex.loadFromFile("Data/Images/Minimap.png");
  miniMap = std::make_unique<Minimap>(miniMapTex, windowSize);

  iconTex.loadFromFile("Data/Images/Interact.png");
  pickprompt = std::make_unique<UISymbol>(iconTex, windowSize);

  monster_timer = 0.0;
  return true;
}

void Gameplay::update(float dt)
{
  // Heat system
  if (!thermometer->update(dt))
  {
    gamedata->currentState = GAME_OVER;
  }
  setTemperature(dt);
  //updateCoalPos();

  // Update this player
  player->movePlayer(leftRight, upDown, dt);
  *playerPos = player->getPosition();

  // Update button prompts
  bool prompt = false;
  for (auto& furnace : furnaces)
  {
    if (player->Areacheck(furnace->getGlobalBounds()))
    {prompt = true;}
  }
  for (auto& coal : coals)
  {
    if (player->Areacheck(coal->getPosition()))
    {prompt = true;}
  }
  pickprompt->Changestate(prompt);

  // Update other player
  updateOtherPlayers(dt);

  // Send my data to the server
  sendMyPacket();

  // Audio
  audiomanager->Footsteps(player->getStepSpeed());

  // Lighting System
  updateFurnaceLighting();
  lightingSystem->update(dt);

  // UI
  objectiveText->update(dt);
  gameplayTimer -= dt;

  if ((300 - (int)gameplayTimer)%30 == 0)
  {
    player->Madness();
    objectiveText->changeObjective(player->getObjective());
    coalCount->setString("Coal: " +  std::to_string(player->getCoalCount()));
  }

  if (gameplayTimer <= 0){
    gamedata->win = true;
    gamedata->currentState = GAME_OVER;
  }

  int roundedTimer = round(gameplayTimer);
  if (roundedTimer%60 < 10){
    timer->setString(std::to_string(roundedTimer/60)+":0"+std::to_string((roundedTimer%60)));
  }else{
    timer->setString(std::to_string(roundedTimer/60)+":"+std::to_string((roundedTimer%60)));
  }

  //monster_timer += dt;
  //if(monster_timer> 20){
  //  otherPlayers[2]->toggleMonster();
  //  monster_timer = 0;
  //}
}

/**
 * @details Render GamePlayer Elements
 * @param window render this window
 */
void Gameplay::render(sf::RenderWindow& window)
{
  // Render world
  viewPlayer.setCenter(player->getPosition());
  sf::Vector2i viewStart = sf::Vector2i((viewPlayer.getCenter().x - 540) / 48,
                                        (viewPlayer.getCenter().y - 360) / 48);

  // Clear render textures
  renderTexLight.clear();
  renderTexMap.clear();
  renderTexMap.setView(viewPlayer);

  // Bake light texture to renderTexLight
  lightingSystem->render(renderTexLight, viewPlayer);

  // Render the map based on the baked lighting textures on renderTexMap
  tileMap->render(renderTexLight, renderTexMap, viewStart);

  shaderObjects.setUniform("texture", sf::Shader::CurrentTexture);
  shaderObjects.setUniform("renderTexture", renderTexLight.getTexture());
  // Render furnaces
  for (auto& furnace : furnaces)
  {
    furnace->renderObject(renderTexMap, shaderObjects);
  }

  // Render coals
  for (auto& coal : coals)
  {
    coal->renderObject(renderTexMap, shaderObjects);
  }

  // Render other players
  for (auto& others : otherPlayers)
  {
    others->renderObject(renderTexMap, shaderObjects);
  }

  // Draw player on renderTexMap
  player->renderObject(renderTexMap);

  // Draw overhead on top of player
  tileMap->renderOverhead(renderTexMap, viewStart);

  // Draw renderTexMap on screen
  renderTexMap.display();
  window.draw(sf::Sprite(renderTexMap.getTexture()));

  // Render UI
  thermometer->renderObject(window);
  miniMap->render(window);
  window.draw(*coalCount);
  window.draw(*timer);
  objectiveText->render(window);
  pickprompt->render(window);
}

void Gameplay::eventHandler(sf::Event event)
{
  switch (event.type)
  {
    case sf::Event::KeyPressed:
      keyPressed(event);
      break;

    case sf::Event::KeyReleased:
      keyReleased(event);
      break;

    case sf::Event::JoystickButtonPressed:
      joystickPressed(event);
      break;

    case sf::Event::JoystickMoved:
      joystickMoved(event);
      break;

    default:
      break;
  }
}

/// Called when current State is changed to this
void Gameplay::stateChanged()
{
  if (gamedata->isHost)
  {
    // Host randomizes coal position at the start of the game
    for (int i = 0; i < 12; i++)
    {
      sf::Vector2i position = tileMap->getFloorPos()[rand() % tileMap->getFloorPos().size()];
      coals[i]->setPosition(sf::Vector2f(position));

      GamePacket gamePacket;
      gamePacket.gameEvents = COAL;
      gamePacket.posX = position.x;
      gamePacket.posY = position.y;
      gamePacket.posZ = i;

      gamedata->networking.server->sendMyPacket(gamePacket);
      gamedata->coalPos->at(i) = sf::Vector2f(position);
    }
  }
}

void Gameplay::keyPressed(sf::Event event)
{
  if(event.key.code == sf::Keyboard::W)  //up: +upDown
  {
    upDown = 10;
  }
  if(event.key.code == sf::Keyboard::S)  //down: -upDown
  {
    upDown = -10;
  }
  if(event.key.code == sf::Keyboard::A)  //left: -leftRight
  {
    leftRight = -10;
  }
  if(event.key.code == sf::Keyboard::D)  //right: +leftRight
  {
    leftRight = 10;
  }

  if (event.key.code == sf::Keyboard::Space)
  {
    pickUpCoal();
    if (player->getObjective() == player->getTasks()[2])
    {
      player->AltInteract();
    }
    else if (player->getObjective() == player->getTasks()[1])
    {
      robFurnace();
    }
    else
    {
      fuelFurnace();
    }
    coalCount->setString("Coal: " +  std::to_string(player->getCoalCount()));
  }

  if (event.key.code == sf::Keyboard::M)
  {
    miniMap->switchRender();
  }

  /// UI TESTING INPUTS
  //if (event.key.code == sf::Keyboard::F)
  //{
  //    player->Madness();
  //    objectiveText->changeObjective(player->getObjective());
  //}

  //if (event.key.code == sf::Keyboard::Equal)
  //{
  //  thermometer->setHeatMultipler(1, true);
  //}
  //if (event.key.code == sf::Keyboard::Hyphen)
  //{
  //  thermometer->setHeatMultipler(1, false);
  //}
}

void Gameplay::keyReleased(sf::Event event)
{
  if(event.key.code == sf::Keyboard::W)  //up: +upDown
  {
    if (upDown > 0)
    {
      upDown = 0;}
  }
  if(event.key.code == sf::Keyboard::S)  //down: -upDown
  {
    if (upDown < 0)
    {
      upDown = 0;}
  }
  if(event.key.code == sf::Keyboard::A)  //left: -leftRight
  {
    if (leftRight < 0)
    {
      leftRight = 0;}
  }
  if(event.key.code == sf::Keyboard::D)  //right: +leftRight
  {
    if (leftRight > 0)
    {
      leftRight = 0;}
  }
}

void Gameplay::joystickPressed(sf::Event event)
{
  if (event.joystickButton.button == 0)
  {
    pickUpCoal();
    if (player->getObjective() == player->getTasks()[2])
    {
      player->AltInteract();
    }
    else if (player->getObjective() == player->getTasks()[1])
    {
      robFurnace();
    }
    else
    {
      fuelFurnace();
    }
  }
  if (event.joystickButton.button == 10)
  {
    miniMap->switchRender();
  }
}

void Gameplay::joystickMoved(sf::Event event)
{
  float horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
  float vertical   = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
  if (abs(horizontal) > 10)
  {
    leftRight = horizontal /10;
  }
  else
  {
    leftRight = 0;
  }
  if (abs(vertical) > 10)
  {
    upDown = -(vertical /10);
  }
  else
  {
    upDown = 0;
  }
}

/// Tries to pickup a coal near player.
/// @param [out] bool True if a coal is picked up, False otherwise
bool Gameplay::pickUpCoal()
{
  int i = 0;
  for (auto& coal : coals)
  {
    if (player->Interact(coal->getPosition()))
    {
      audiomanager->PlayerSounds('C');

      // Randomize this coal position
      sf::Vector2i position = tileMap->getFloorPos()[rand() % tileMap->getFloorPos().size()];
      coal->setPosition(sf::Vector2f(position.x, position.y));

      // Send the new position to network
      GamePacket gamePacket;
      gamePacket.gameEvents = COAL;
      gamePacket.posX = position.x;
      gamePacket.posY = position.y;
      gamePacket.posZ = i;

      if (gamedata->isHost)
      {
        gamedata->networking.server->sendMyPacket(gamePacket);
      }
      else
      {
        gamedata->networking.client->send(gamePacket);
      }

      gamedata->coalPos->at(i) = sf::Vector2f(position);

      return true;
    }

    i++;
  }
  return false;
}

/// Tries to add a fuel to furnace near player.
/// @param [out] bool True if coal is added to furnace, False otherwise
bool Gameplay::fuelFurnace()
{
  int i = 0;
  for (auto& furnace : furnaces)
  {
    if (player->Interact(furnace->getGlobalBounds())) // Player has coal
    {
      furnace->addCoal();
      audiomanager->PlayerSounds('F');

      // Send FUEL_FURNACE event to network
      GamePacket gamePacket;
      gamePacket.gameEvents = FUEL_FURNACE;
      gamePacket.posX = i;
      if (gamedata->isHost)
      {
        gamedata->networking.server->sendMyPacket(gamePacket);
      }
      else
      {
        gamedata->networking.client->send(gamePacket);
      }

      return true;
    }

    i++;
  }
  return false;
}

/// Tries to take a fuel from furnace near player
/// @param [out] bool False if coal is taken from the furnace, True otherwise
bool Gameplay::robFurnace()
{
  for (auto& furnace : furnaces)
  {
    if (player->Areacheck(furnace->getGlobalBounds()))
    {
      if(furnace->removeCoal())
      {
        player->Interact();
        audiomanager->PlayerSounds('C');
      }
    }
  }
}

void Gameplay::setTemperature(float dt)
{
  float heat = 0;

  if (player->getObjective() == player->getTasks()[2])
  {
    heat -= player->MadChill(dt);
    thermometer->setHeatMultiplier(heat);
  }
  else
  {
    int i = 0;
    // Add coal to furnace when receiving FUEL_FURNACE event
    for (auto& furnaceIndex : *gamedata->furnaceEvent)
    {
      furnaces[furnaceIndex]->addCoal();
      gamedata->furnaceEvent->erase(gamedata->furnaceEvent->begin() + i);
      i++;
    }

    for (auto& furnace : furnaces)
    {
      furnace->update(dt);

      if (furnace->getFuelCount() == 0)
      {
        heat -= furnace->getSecondsCold();
      }
      else
      {
        heat += furnace->getFuelCount();
      }
    }

    heat = heat / furnaces.size();

    if (player->getObjective() == player->getTasks()[1])
    {
      heat = -heat;
    }
  }
  //std::cout << heat << "\n";
  //std::cout << player->getCoalCount() << "\n";
  thermometer->setHeatMultiplier(heat);
}

/// Turns off/on the lighting of furnaces
void Gameplay::updateFurnaceLighting()
{
  int x = 1;
  for (auto& furnace : furnaces)
  {
    if (furnace->getFuelCount() <= 0)
    {
      lightingSystem->setLightVisible(false, x);
    }

    if (furnace->getFuelCount() > 0)
    {
      lightingSystem->setLightVisible(true, x);
      lightingSystem->setLightRadius(9 - furnace->getFuelCount(), x);
    }
    x++;
  }
}

/// Updates the position of other players based on networking
void Gameplay::updateOtherPlayers(float dt)
{
  int i = 0;
  for (const auto& packet : *gamedata->otherPlayerPacket)
  {
    GamePacket gamePacket = packet.second;

    if (i >= otherPlayers.size())
    {
      // Create new players if necessary
      otherPlayers.emplace_back(std::make_unique<Player>());
    }

    if (gamePacket.gameEvents == MOVEMENT)
    {
      otherPlayers[i]->setOtherPlayerPos(sf::Vector2f(gamePacket.posX, gamePacket.posY), dt);
    }

    i++;
  }
}

void Gameplay::updateCoalPos()
{
  int i = 0;
  for (auto& coal : *gamedata->coalPos)
  {
    coals[i]->setPosition(coal);
    i++;
  }
}

/// Sends my GamePacket to the network
void Gameplay::sendMyPacket()
{
  GamePacket gamePacket;
  gamePacket.posX = playerPos->x;
  gamePacket.posY = playerPos->y;
  gamePacket.gameEvents = MOVEMENT;

  if (gamedata->isHost)
  {
    gamedata->networking.server->sendMyPacket(gamePacket);
  }
  else
  {
    gamedata->networking.client->send(gamePacket);
  }
}
