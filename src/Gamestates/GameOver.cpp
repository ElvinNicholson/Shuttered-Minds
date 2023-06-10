#include "GameOver.h"
#include "GamePlay.h"

GameOver::GameOver(std::shared_ptr<GameData> _current_state) :
  GameState(_current_state)
{
  init();
}

bool GameOver::init()
{
  winScreenTexture.loadFromFile("Data/Images/UI/WinScreen.png");
  loseScreenTexture.loadFromFile("Data/Images/UI/LoseScreen.png");
  winScreen.setTexture(loseScreenTexture);
  gameOverText = std::make_unique<sf::Text>();
  return true;
}



void GameOver::update(float dt)
{

}

void GameOver::render(sf::RenderWindow& window)
{
  window.draw(winScreen);
}

void GameOver::eventHandler(sf::Event event)
{
  if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      gamedata->currentState = MENU;
    }
  }
  if (event.type == sf::Event::JoystickButtonPressed)
  {
    if (event.joystickButton.button == 0)
    {
      gamedata->currentState = MENU;
    }
  }
}

/// Called when current State is changed to this
void GameOver::stateChanged()
{

  gameOverText->setFont(gamedata->font);
  gameOverText->setString("YOU LOSE");
  gameOverText->setPosition(160, 250);
  gameOverText->setCharacterSize(150);
  if(gamedata->win){
    gameOverText->setString("YOU WIN");
    winScreen.setTexture(winScreenTexture);
  }
}
