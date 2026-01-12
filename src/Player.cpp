#include "Player.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include <iostream>

Player::Player(const CharacterData &data, ResourceManager &resources)
    : sprite(resources.getTexture(data.texture_name)), speed(data.moveSpeed),
      baseSpeed(data.moveSpeed), maxHealth(data.maxHealth),
      currentHealth(data.maxHealth), healthRegen(0.f), level(1), currentXP(0.f),
      xpToNextLevel(50.f), facingRight(true), damageReduction(0.f),
      cooldownMultiplier(1.f) {
  sprite.setScale({0.2f, 0.2f});

  sf::Vector2u size = resources.getTexture(data.texture_name).getSize();
  sprite.setOrigin({size.x / 2.f, size.y / 2.f});
  sprite.setPosition({5000.f, 5000.f});

  if (data.startingWeapon == "Fire wand") {
    addWeapon(FireWand(resources));
  } else if (data.startingWeapon == "Demonic Book") {
    addWeapon(DemonicBook(resources));
  }
  // else if (data.startingWeapon == "Bone") {addWeapon(Bone(resources));}
}

Player::~Player() {
  for (Weapon *w : weapons) {
    delete w;
  }
  weapons.clear();

  for (Item *i : items) {
    delete i;
  }
  items.clear();
}

Player::Player(const Player &other)
    : sprite(other.sprite), speed(other.speed), baseSpeed(other.baseSpeed),
      maxHealth(other.maxHealth), currentHealth(other.currentHealth),
      healthRegen(other.healthRegen), level(other.level),
      currentXP(other.currentXP), xpToNextLevel(other.xpToNextLevel),
      facingRight(other.facingRight), damageReduction(other.damageReduction),
      cooldownMultiplier(other.cooldownMultiplier) {
  for (const auto *w : other.weapons) {
    weapons.push_back(w->copy());
  }
  // Note: Items are not copied (pointer ownership issues)
}

Player &Player::operator=(const Player &other) {
  if (this != &other) {
    for (Weapon *w : weapons) {
      delete w;
    }
    weapons.clear();

    for (Item *i : items) {
      delete i;
    }
    items.clear();

    sprite = other.sprite;
    speed = other.speed;
    baseSpeed = other.baseSpeed;
    maxHealth = other.maxHealth;
    currentHealth = other.currentHealth;
    healthRegen = other.healthRegen;
    level = other.level;
    currentXP = other.currentXP;
    xpToNextLevel = other.xpToNextLevel;
    facingRight = other.facingRight;
    damageReduction = other.damageReduction;
    cooldownMultiplier = other.cooldownMultiplier;

    for (const auto *w : other.weapons) {
      weapons.push_back(w->copy());
    }
  }
  return *this;
}

void Player::addWeapon(const Weapon &weapon) {
  weapons.push_back(weapon.copy());
}

void Player::update(float dt, const sf::RenderWindow &window,
                    const std::vector<std::unique_ptr<Enemy>> &enemies,
                    std::vector<Projectile> &projectiles) {
  // Health regeneration
  if (healthRegen > 0.f && currentHealth < maxHealth) {
    currentHealth += healthRegen * dt;
    if (currentHealth > maxHealth)
      currentHealth = maxHealth;
  }

  sf::Vector2f velocity(0.f, 0.f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    velocity.y -= speed * dt;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    velocity.y += speed * dt;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    velocity.x -= speed * dt;
    facingRight = false;
    if (sprite.getScale().x > 0)
      sprite.setScale({-0.2f, 0.2f});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    velocity.x += speed * dt;
    facingRight = true;
    if (sprite.getScale().x < 0)
      sprite.setScale({0.2f, 0.2f});
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f dir = mousePos - sprite.getPosition();

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
      velocity += (dir / len) * speed * dt;
  }

  sprite.move(velocity);

  for (auto &w : weapons) {
    w->update(dt, getPos(), enemies, projectiles);
  }
}

void Player::draw(sf::RenderWindow &window) const {
  for (const auto *w : weapons) {
    w->draw(window);
  }

  window.draw(sprite);

  // Health bar stuff
  sf::RectangleShape BlackRect({50.f, 8.f});
  BlackRect.setOrigin({25.f, -50.f});
  BlackRect.setPosition(sprite.getPosition());
  BlackRect.setFillColor(sf::Color::Black);
  window.draw(BlackRect);

  float health = currentHealth / maxHealth;
  if (health > 0) {
    sf::RectangleShape hpRect({50.f * health, 8.f});
    hpRect.setOrigin({25.f, -50.f});
    hpRect.setPosition(sprite.getPosition());
    hpRect.setFillColor(sf::Color::Red);
    window.draw(hpRect);
  }
}

sf::Vector2f Player::getPos() const { return sprite.getPosition(); }

void Player::takeDamage(float amount) {
  // Apply damage reduction from items
  float finalDamage = amount * (1.f - damageReduction);

  std::cout << "player hit: took " << finalDamage << " amount of damage"
            << std::endl;
  currentHealth -= finalDamage;
  if (currentHealth < 0)
    currentHealth = 0;
}

void Player::addItem(Item *item) {
  if (items.size() < 3) {
    items.push_back(item);
  }
}

void Player::addXP(float amount) {
  currentXP += amount;
  // Level up check happens externally in GameState
}

void Player::levelUp() {
  level++;
  currentXP = 0.f;
  xpToNextLevel = 50.f + (level * 10.f); // XP scaling formula
}

void Player::heal(float amount) {
  currentHealth += amount;
  if (currentHealth > maxHealth) {
    currentHealth = maxHealth;
  }
}
