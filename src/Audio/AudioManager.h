//
// Created by NESco on 14/04/2023.
//

#ifndef SFMLGAME_AUDIOMANAGER_H
#define SFMLGAME_AUDIOMANAGER_H
#include <SFML/Audio.hpp>

class AudioManager
{
 public:
  AudioManager();
  void PlayerSounds(char action);
  void Footsteps(int walkspeed);
  void StartMusic();

 private:
  std::unique_ptr<sf::SoundBuffer> slowsteps;
  std::unique_ptr<sf::SoundBuffer> faststeps;
  std::unique_ptr<sf::SoundBuffer> pickup;
  std::unique_ptr<sf::SoundBuffer> fuel;
  std::unique_ptr<sf::SoundBuffer> munch;
  std::unique_ptr<sf::SoundBuffer> madness_objective;
  std::unique_ptr<sf::SoundBuffer> new_objective;
  std::unique_ptr<sf::SoundBuffer> you_lose;

  sf::Sound playersteps;
  sf::Sound othersteps;
  sf::Sound playeractions;

  sf::Music ost;

  bool fast;

};

#endif // SFMLGAME_AUDIOMANAGER_H
