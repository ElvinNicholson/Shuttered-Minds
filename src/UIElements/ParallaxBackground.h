#ifndef SFMLGAME_PARALLAXBACKGROUND_H
#define SFMLGAME_PARALLAXBACKGROUND_H

#include "../GameObject/GameObject.h"

class ParallaxBackground
{
 public:
  ParallaxBackground();
  void render(sf::RenderWindow& window);
  void scrollBackground(float dt);

 private:
  sf::Texture textureBackground;
  sf::FloatRect rectBackground;
  GameObject background;

  sf::Texture textureTree1;
  sf::FloatRect rectTree1;
  GameObject tree1;

  sf::Texture textureTree2;
  sf::FloatRect rectTree2;
  GameObject tree2;

  sf::Texture textureTree3;
  sf::FloatRect rectTree3;
  GameObject tree3;

  sf::Texture textureGround;
  sf::FloatRect rectGround;
  GameObject ground;
};

#endif // SFMLGAME_PARALLAXBACKGROUND_H
