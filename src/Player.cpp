#include "Player.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include "Item.h"
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
        addWeapon(std::make_unique<FireWand>(resources));
    } else if (data.startingWeapon == "Demonic Book") {
        addWeapon(std::make_unique<DemonicBook>(resources));
    }
    // else if (data.startingWeapon == "Bone")
    // {addWeapon(std::make_unique<Bone>(resources));}
}

Player::~Player() = default;

void Player::addWeapon(std::unique_ptr<Weapon> weapon) {
    weapons.push_back(std::move(weapon));
}

void Player::update(float dt, const sf::RenderWindow &window,
                    const std::vector<std::unique_ptr<Enemy> > &enemies,
                    std::vector<Projectile> &projectiles) {
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
    }//TODO: Basically e bug e ca se stack-eaza vectorul de movement atunci cand apas WASD si LM, sa pastrez asta ca feature ca un fel de sprint? (im bad at balancing games)

    sprite.move(velocity);

    for (auto &w: weapons) {
        w->update(dt, getPos(), enemies, projectiles);
    }
}

void Player::draw(sf::RenderWindow &window) const {
    for (const auto &w: weapons) {
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
    float finalDamage = amount * (1.f - damageReduction);
    std::cout << "player hit: took " << finalDamage << " amount of damage"
            << std::endl;
    currentHealth -= finalDamage;
    if (currentHealth < 0)
        currentHealth = 0;
}

void Player::addItem(std::unique_ptr<Item> item) {
    if (items.size() < 3) {
        items.push_back(std::move(item));
    }
}

const std::vector<std::unique_ptr<Weapon> > &Player::getWeapons() const {
    return weapons;
}

const std::vector<std::unique_ptr<Item> > &Player::getItems() const {
    return items;
}

bool Player::shouldLevelUp() const { return currentXP >= xpToNextLevel; }

int Player::getLevel() const { return level; }

float Player::getCurrentXP() const { return currentXP; }

float Player::getXPToNextLevel() const { return xpToNextLevel; }

bool Player::isDead() const { return currentHealth <= 0; }

float Player::getMaxHealth() const { return maxHealth; }

void Player::increaseMaxHealth(float amount) {
    maxHealth += amount;
    currentHealth += amount;
}

void Player::setHealthRegen(float regen) { healthRegen = regen; }

float Player::getHealthRegen() const { return healthRegen; }

void Player::setDamageReduction(float reduction) {
    damageReduction = reduction;
}

void Player::setCooldownMultiplier(float mult) { cooldownMultiplier = mult; }

float Player::getDamageReduction() const { return damageReduction; }

float Player::getCooldownMultiplier() const { return cooldownMultiplier; }

void Player::increaseSpeed(float multiplier) { speed = baseSpeed * multiplier; }

float Player::getSpeed() const { return speed; }

sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }

bool Player::isFacingRight() const { return facingRight; }

std::ostream &operator<<(std::ostream &os, const Player &obj) {
    obj.print(os);
    return os;
}

float Player::getHealthPoints() const { return currentHealth / maxHealth; }

void Player::addXP(float amount) {
    currentXP += amount;
}

void Player::levelUp() {
    level++;
    currentXP = 0.f;
    xpToNextLevel = 50.f + (level * 10.f);//formula de scale de lvl-up
}

void Player::heal(float amount) {
    currentHealth += amount;
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }
}
