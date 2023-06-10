//
// Created by NESco on 14/04/2023.
//

#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
{
  ost.openFromFile("Data/Sounds/Horror-Soundscape.wav");
  slowsteps = std::make_unique<sf::SoundBuffer>();
  faststeps = std::make_unique<sf::SoundBuffer>();
  slowsteps->loadFromFile("Data/Sounds/slow-footsteps.wav");
  faststeps->loadFromFile("Data/Sounds/fast-footsteps.wav");
  pickup = std::make_unique<sf::SoundBuffer>();
  fuel = std::make_unique<sf::SoundBuffer>();
  munch = std::make_unique<sf::SoundBuffer>();
  pickup->loadFromFile("Data/Sounds/pickup.wav");
  fuel->loadFromFile("Data/Sounds/furnace_deposit.wav");
  munch->loadFromFile("Data/Sounds/slow-footsteps.wav");

  madness_objective = std::make_unique<sf::SoundBuffer>();
  madness_objective->loadFromFile("Data/Sounds/madness-objective.wav");
  new_objective = std::make_unique<sf::SoundBuffer>();
  new_objective->loadFromFile("Data/Sounds/new-objective.wav");
  you_lose = std::make_unique<sf::SoundBuffer>();
  you_lose->loadFromFile("Data/Sounds/you-lose.wav");
  fast = false;
  StartMusic();
}

void AudioManager::PlayerSounds(char action)  //Non-walking player sounds triggered here
{
  //action codes:
  //C- Coal pickup    F- Furnace deposit    E- eating coal   M- Madness objective   O- New Objective
  switch (action)
    {
      case 'C':
        playeractions.setBuffer(*pickup);
        break;
      case 'F':
        playeractions.setBuffer(*fuel);
        break;
      case 'E':
        playeractions.setBuffer(*pickup);
        break;
      case 'M':
        playeractions.setBuffer(*madness_objective);
        break;
      case 'O':
        playeractions.setBuffer(*new_objective);
        break;
      case 'L':
        playeractions.setBuffer(*you_lose);
        break;
    }
    playeractions.play();
}


void AudioManager::Footsteps(int walkspeed)  //Three settings- off, slow and fast. Altered based on player walking speed
{
  if (walkspeed > 0)
  {
    if (walkspeed >= 2) //Walking fast
    {
      if (!fast)
      {
        fast = true;
        playersteps.setBuffer(*faststeps);
        }
    }
    else  //Walking slow
    {
      if (fast)
      {
        fast = false;
        playersteps.setBuffer(*slowsteps);
        }
    }
    if (playersteps.getStatus() != sf::SoundSource::Status::Playing)  //Plays footsteps, if not currently playing
    {
      playersteps.play();
    }
  }
  else  //Stops audio when standing still
  {
    playersteps.stop();
  }
}

void AudioManager::StartMusic()  //Starts music at the start of the game
{
  ost.setLoop(true);
  ost.setVolume(20);  //100 is max, and is deafening. Don't go higher than 30 if you want any other audible audio
  ost.play();
}