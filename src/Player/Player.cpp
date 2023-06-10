#include "Player.h"

Player::Player()
{
  init();
}

void Player::init()
{
  //LOAD IN PLAYER TEXTURES
  spritesheet = std::make_unique<sf::Texture>();
  spritesheet->loadFromFile("Data/Images/PlayerSprites/spritesheet.png");
  monster_spritesheet = std::make_unique<sf::Texture>();
  monster_spritesheet->loadFromFile("Data/Images/PlayerSprites/monster_spritesheet.png");




  
  setTexture(*spritesheet);
  sprite->setTextureRect(sf::IntRect(0, 0, 24, 24));
  setOriginCentre();
  setScale(2.7, 2.7);
  is_monster = false;

  //FOR MONSTER ANIMATOIN
  //setTexture(*monster_spritesheet);
  //setScale(1, 1);
  coals = 0;
  steps = 0;
  mad = false;
  tasks.emplace_back("Survive");
  tasks.emplace_back("Hoard coal for yourself");
  tasks.emplace_back("Eat coal to stay warm");
  objective = std::make_unique<std::string>(tasks[0]);

  step_timer = 0;
}

void Player::movePlayer(float rl, float ud, float dt)
{
  int x = 0;
  int y = 0;
  if (rl > 1)  //right
  {
    x += speed * abs(rl) * dt;
  }
  else if (rl < -1)  //left
  {
    x -= speed * abs(rl) * dt;

  }
  if (ud > 1)  //up
  {
    y -= speed * abs(ud) * dt;
  }
  else if (ud < -1)  //down
  {
    y += speed * abs(ud) * dt;
  }
  if (abs(ud) + abs(rl) > 1)  //checks the player's speed
  {
    if (abs(ud) > 5 || abs(rl) > 5)
    {pace = 2;}  //fast
    else
    {pace = 1;}  //slow
  }
  else
  {pace = 0;}  //standing still
  if(is_monster){
    animateMonster(rl,ud,dt);
  }else
  {
    Animate(rl, ud, dt);
  }
  if (Collision(x, y)) //checks if the proposed move is possible. Stops the player if not.
  {move(x, y);}
}

void Player::Animate(float rl, float ud, float dt)
{
  step_timer += dt;
  if (abs(rl) > abs(ud))
  {
    if (rl > 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(24*10+(24*steps), 0, 24, 24));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
    if (rl < 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(24*7+(24*steps), 0, 24, 24));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
  }
  else if (abs(ud) >= abs(rl))
  {
    if (ud > 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(24*13+(24*steps), 0, 24, 24));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
    if (ud < 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(24*4+(24*steps), 0, 24, 24));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
  }
}

void Player::animateMonster(float rl, float ud, float dt)
{

  //To use monstEr animation set texture to monster_spritesheet
  //Then setScale(1.2,1.2);

  step_timer += dt;
  if (abs(rl) > abs(ud))
  {
    if (rl > 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(73*steps, 101, 73, 94));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
    if (rl < 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(73*steps, 295, 73, 94));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
  }
  else if (abs(ud) >= abs(rl))
  {
    if (ud > 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(73*steps, 0, 73, 92));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
    if (ud < 0)
    {
      if(step_timer>0.2){
        sprite->setTextureRect(sf::IntRect(73*steps, 197, 73, 92));
        steps += 1;
        if(steps == 3) steps = 0;
        step_timer = 0.0;
      }
    }
  }
}

void Player::toggleMonster()
{
  if(is_monster){
    is_monster = !is_monster;
    setTexture(*spritesheet);
    setScale(2.7, 2.7);
    move(10, 10);
  }else{
    is_monster = !is_monster;
    setTexture(*monster_spritesheet);
    setScale(1.1,1.1);
    move(-20, -10);

  }
}

bool Player::Areacheck(const sf::Vector2f spots)  //Checks to see if a point is within the player's range
{
  sf::Vector2f place = sprite->getPosition();
  sf::FloatRect space = sprite->getGlobalBounds();
  sf::IntRect armlength(place.x-50, place.y-50, 100+space.width, 100+space.height);
  if (armlength.contains(spots.x, spots.y))
  {return true;}
  else
  {return false;}
}

bool Player::Areacheck(const sf::FloatRect area)  //Checks to see if an area is within the player's range
{
  sf::Vector2f place = sprite->getPosition();
  sf::FloatRect space = sprite->getGlobalBounds();
  sf::FloatRect armlength(place.x-25, place.y-25, 50+space.width, 50+space.height);
  if (armlength.intersects(area, armlength))
  {return true;}
  else
  {return false;}
}

bool Player::Interact(const sf::Vector2f spots)  //pick up coal
{
  if (Areacheck(spots))
  {
    if (coals < max_coals)
    {
      if (*objective != tasks[2])
      {
        coals += 1;
      }
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}


bool Player::Interact(const sf::FloatRect area)  //put coal in furnace
{
  if (Areacheck(area))
  {
    if (coals > 0)
    {
      coals -= 1;
      return true;
    }
    return false;
  }
  else
  {
    return false;
  }
}

bool Player::AltInteract()  //eat coal
{
  if (*objective == tasks[2])
  {
    //std::cout << "munch munch";
    coals = 0;
    seconds_hungy = -5;
    return true;
  }
  return false;
}

bool Player::Interact()  //take coal from a furnace
{
  if (*objective == tasks[1])
  {
      coals += 1;
      return true;
  }
  return false;
}

bool Player::Collision(float x, float y)  //checks if the proposed location is in a wall.
{
  //run this when the player moves.
  //return false if they can't move there.
  sf::Vector2f pos = sprite->getPosition() + sf::Vector2f(x, y);
  sf::FloatRect playerpos (pos.x, pos.y, sprite->getGlobalBounds().width-48, sprite->getGlobalBounds().height-48);

  sf::Vector2i tilePos = getTilePos();
  for (int j = -1; j < 2; j++)
  {
    for (int k = -1; k < 2; k++)
    {
      if (!(0 <= j + tilePos.y && j + tilePos.y < 56) ||
          !(0 <= k + tilePos.x && k + tilePos.x < 49))
      {
        // Index is outside of range
        continue;
      }

      if (playerpos.intersects(walls[j + tilePos.y][k + tilePos.x]))  //walls have a consistent northwest offset
      {
        return false;
      }
    }
  }

  return true;
}

float Player::MadChill(float dt)
{
  seconds_hungy += dt;
  return seconds_hungy;
}

void Player::Madness()  //If sane, go mad. If mad, go sane
{
  if (!mad)
  {
    *objective = tasks[rand()%2+1];
    max_coals = 20;
    
    mad = true;
  }
  else
  {
    seconds_hungy = 0;
    *objective = tasks[0];

    if (coals > 4)
    {
      coals = 4;
    }
    max_coals = 4;

    mad = false;
  }
}

void Player::setOtherPlayerPos(sf::Vector2f current_pos, float dt)
{
  float rl = 0;
  float ud = 0;

  if (current_pos.x < last_position.x)
  {
    rl = -10;
  }
  else if (current_pos.x > last_position.x)
  {
    rl = 10;
  }

  if (current_pos.y < last_position.y)
  {
    ud = 10;
  }
  else if (current_pos.y > last_position.y)
  {
    ud = -10;
  }

  last_position = current_pos;

  Animate(rl, ud, dt);
  setPosition(current_pos);
}
