#include "Player.h"
#include "FireWand.h"
#include <iostream>

Player::Player(const CharacterData &data, ResourceManager &resources)
    : sprite(resources.getTexture(data.texture_name)), speed(data.moveSpeed),
      maxHealth(data.maxHealth), currentHealth(data.maxHealth) {

  sprite.setScale({0.2f, 0.2f});

  sf::Vector2u size = resources.getTexture(data.texture_name).getSize();
  sprite.setOrigin({size.x / 2.f, size.y / 2.f});
  sprite.setPosition({5000.f, 5000.f});

  // addWeapon(Weapon(data.startingWeapon, data.Weapon_sprite, 35.f, 3.f));
  addWeapon(FireWand(resources));
  // de scos de aici as
}

Player::~Player() {
  for (Weapon *w : weapons) {
    delete w;
  }
  weapons.clear();
}

Player::Player(const Player &other)
    : sprite(other.sprite), speed(other.speed), maxHealth(other.maxHealth),
      currentHealth(other.currentHealth) {

  for (const auto *w : other.weapons) {
    weapons.push_back(w->copy());
  }
}

Player &Player::operator=(const Player &other) {
  if (this != &other) {
    for (Weapon *w : weapons) {
      delete w;
    }
    weapons.clear();

    sprite = other.sprite;
    speed = other.speed;
    maxHealth = other.maxHealth;
    currentHealth = other.currentHealth;

    for (const auto *w : other.weapons) {
      weapons.push_back(w->copy());
    }
  }
  return *this;
}

void Player::addWeapon(const Weapon &weapon) {
  weapons.push_back(weapon.copy());
}

void Player::update(float dt,
                    const std::vector<std::unique_ptr<Enemy>> &enemies,
                    std::vector<Projectile> &projectiles) {
  sf::Vector2f velocity(0.f, 0.f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    velocity.y -= speed * dt;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    velocity.y += speed * dt;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    velocity.x -= speed * dt;
    if (sprite.getScale().x > 0)
      sprite.setScale({-0.2f, 0.2f});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    velocity.x += speed * dt;
    if (sprite.getScale().x < 0)
      sprite.setScale({0.2f, 0.2f});
  }

  sprite.move(velocity);

  for (auto &w : weapons) {
    w->update(dt, getPos(), enemies, projectiles);
  }
}

void Player::draw(sf::RenderWindow &window) const { window.draw(sprite); }

sf::Vector2f Player::getPos() const { return sprite.getPosition(); }
