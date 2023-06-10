#include "Gamestate.h"

GameState::GameState(std::shared_ptr<GameData> _current_state) :
  gamedata(_current_state)
{
}