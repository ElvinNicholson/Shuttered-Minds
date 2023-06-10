#pragma once

#include <iostream>
#include <utility>

#include "../GameObject/GameObject.h"

class Player : public GameObject
{
 public:
  Player();
  void SetWalls(std::vector<std::vector<sf::FloatRect>> wall) { walls = std::move(wall); }

  std::vector<std::string> getTasks(){return tasks;};

  void movePlayer(float rl, float ud, float dt);
  void Animate(float rl, float ud, float dt);
  void animateMonster(float rl, float ud, float dt);

  bool Interact();
  bool Interact(const sf::Vector2f spots);
  bool Interact(const sf::FloatRect area);
  bool AltInteract();
  bool Areacheck(const sf::FloatRect area);
  bool Areacheck(const sf::Vector2f spots);

  bool Collision(float x, float y);

  int getStepSpeed() { return pace;}
  int getCoalCount() { return coals;}
  bool getMadness() { return mad;}

  void Madness();
  float MadChill(float dt);
  std::string getObjective() { return *objective; }

  void toggleMonster();
  bool is_monster;

  void setOtherPlayerPos(sf::Vector2f current_pos, float dt);


 private:
  void init();

  std::unique_ptr<sf::Texture> spritesheet;
  std::unique_ptr<sf::Texture> monster_spritesheet;



  std::vector<std::vector<sf::FloatRect>> walls;
  int steps;
  float step_timer;
  int pace;


  float speed = 25;
  int coals;
  int max_coals = 4;

  bool mad;
  float seconds_hungy = 0;
  std::vector<std::string> tasks;
  std::unique_ptr<std::string> objective;

  sf::Vector2f last_position;
};
