#include "ParallaxBackground.h"

ParallaxBackground::ParallaxBackground()
{
  textureBackground.loadFromFile("../Data/Images/UI/Background.png");
  textureBackground.setRepeated(true);
  rectBackground = sf::FloatRect(0, 0, 320, 180);
  background.setTextureRect(sf::IntRect(rectBackground));
  background.setScale(4, 4);
  background.setTexture(textureBackground);

  textureTree1.loadFromFile("../Data/Images/UI/Tree1.png");
  textureTree1.setRepeated(true);
  rectTree1 = sf::FloatRect(0, 0, 320, 180);
  tree1.setTextureRect(sf::IntRect(rectTree1));
  tree1.setScale(4, 4);
  tree1.setTexture(textureTree1);

  textureTree2.loadFromFile("../Data/Images/UI/Tree2.png");
  textureTree2.setRepeated(true);
  rectTree2 = sf::FloatRect(0, 0, 320, 180);
  tree2.setTextureRect(sf::IntRect(rectTree2));
  tree2.setScale(4, 4);
  tree2.setTexture(textureTree2);

  textureTree3.loadFromFile("../Data/Images/UI/Tree3.png");
  textureTree3.setRepeated(true);
  rectTree3 = sf::FloatRect(0, 0, 320, 180);
  tree3.setTextureRect(sf::IntRect(rectTree3));
  tree3.setScale(4, 4);
  tree3.setTexture(textureTree3);

  textureGround.loadFromFile("../Data/Images/UI/Ground.png");
  textureGround.setRepeated(true);
  rectGround = sf::FloatRect(0, 0, 320, 180);
  ground.setTextureRect(sf::IntRect(rectGround));
  ground.setScale(4, 4);
  ground.setTexture(textureGround);
}

void ParallaxBackground::render(sf::RenderWindow& window)
{
  background.renderObject(window);
  tree1.renderObject(window);
  tree2.renderObject(window);
  tree3.renderObject(window);
  ground.renderObject(window);
}

void ParallaxBackground::scrollBackground(float dt)
{
  rectBackground.left += dt * 20;
  background.setTextureRect(sf::IntRect(rectBackground));

  rectTree1.left += dt * 40;
  tree1.setTextureRect(sf::IntRect(rectTree1));

  rectTree2.left += dt * 60;
  tree2.setTextureRect(sf::IntRect(rectTree2));

  rectTree3.left += dt * 80;
  tree3.setTextureRect(sf::IntRect(rectTree3));

  rectGround.left += dt * 100;
  ground.setTextureRect(sf::IntRect(rectGround));
}
