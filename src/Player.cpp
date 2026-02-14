#include "Player.h"
#include "Animation.h"
#include "BoneShooter.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include "Item.h"
#include "PlayerException.h"
#include "PlayerValidation.h"
#include <iostream>

Player::Player(const CharacterData &data, ResourceManager &resources)
    : sprite(resources.getTexture(data.texture_name)), speed(data.moveSpeed),
      baseSpeed(data.moveSpeed), maxHealth(data.maxHealth),
      currentHealth(data.maxHealth), healthRegen(0.f), level(1), currentXP(0.f), // level(101) arunca exceptie
      xpToNextLevel(50.f), facingRight(true), damageReduction(0.f),
      cooldownMultiplier(1.f) {
    // pt flying skull
    if (data.texture_name == "spinning_skull") {
        animation = std::make_unique<Animation>(sprite);
        const int frameW = 149;
        const int frameH = 128;
        const sf::Vector2u texSize =
                resources.getTexture(data.texture_name).getSize();
        const int frames = std::max(1u, texSize.y / frameH);
        for (int i = 0; i < frames; ++i) {
            animation->addFrame(
                Frame{sf::IntRect({0, i * frameH}, {frameW, frameH}), 0.05});
        }
        sprite.setOrigin({frameW / 2.f, frameH / 2.f});
        sprite.setTextureRect(sf::IntRect({0, 0}, {frameW, frameH}));
        sprite.setScale({0.7f, 0.7f});
    } else {
        sf::Vector2u size = resources.getTexture(data.texture_name).getSize();
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setScale({0.2f, 0.2f});
    }

    sprite.setPosition({5000.f, 5000.f});

    if (data.startingWeapon == "Fire wand") {
        addWeapon(std::make_unique<FireWand>(resources));
    } else if (data.startingWeapon == "Demonic Book") {
        addWeapon(std::make_unique<DemonicBook>(resources));
    } else if (data.startingWeapon == "Bone Shooter") {
        addWeapon(std::make_unique<BoneShooter>(resources));
    }
}

Player::~Player() = default;

void Player::addWeapon(std::unique_ptr<Weapon> weapon) {
    // verificare daca detinem arma
    std::string weaponName = weapon->getName();
    for (auto &w: weapons) {
        if (w->getName() == weaponName) {
            // lvl up
            if (w->canLevelUp()) {
                w->levelUp();
                std::cout << weaponName << " leveled up to Level " << w->getLevel()
                        << "!" << std::endl;
            }
            return;
        }
    }
    // else adauga + setOwner
    weapon->setOwner(*this);
    weapons.push_back(std::move(weapon));
}

void Player::update(float dt, const sf::RenderWindow &window,
                    const std::vector<std::unique_ptr<Enemy> > &enemies,
                    std::vector<Projectile> &projectiles) {
    if (animation) {
        animation->update(dt);
    }

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
        if (sprite.getScale().x > 0) {
            float scaleY = sprite.getScale().y;
            sprite.setScale({-scaleY, scaleY});
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity.x += speed * dt;
        facingRight = true;
        if (sprite.getScale().x < 0) {
            float scaleY = sprite.getScale().y;
            sprite.setScale({scaleY, scaleY});
        }
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

    for (auto &w: weapons) {
        w->update(dt, getPos(), enemies, projectiles);
    }
    validateState();
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
        // aplicare efect instant
        items.back()->applyEffect(*this);
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

// float Player::getDamageReduction() const { return damageReduction; }

// float Player::getCooldownMultiplier() const { return cooldownMultiplier; }

void Player::increaseSpeed(float multiplier) { speed = baseSpeed * multiplier; }

// float Player::getSpeed() const { return speed; }

sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }

bool Player::isFacingRight() const { return facingRight; }

std::ostream &operator<<(std::ostream &os, const Player &obj) {
    obj.print(os);
    return os;
}

// float Player::getHealthPoints() const { return currentHealth / maxHealth; }

void Player::addXP(float amount) { currentXP += amount; }

void Player::levelUp() {
    /*
     * Verificare care intr-un context al unui joc endless ar fi absurda sa aiba
     * un level maxim, insa jocul meu este facut cu ideea de a bate boss-ul la
     * final. E aproape imposibil sa ajungi la level 100, daca ajungi, atunci
     * modifica valoarea developer-ului -> int levelMaxMircea = 13;
     */
    if (level >= 100) {
        level = 100;
        return;
    }

    if (currentXP < 0.f) {
        currentXP = 0.f; // How did you end here...?
    }

    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }

    if (maxHealth <= 0.f) {
        maxHealth = 100.f;
        currentHealth = maxHealth;
    }

    level++;
    currentXP = 0.f;
    xpToNextLevel = 50.f + (level * 10.f); // formula de scale de lvl-up

    // reset pt a reaplica stats
    damageReduction = 0.f;
    cooldownMultiplier = 1.f;
    speed = baseSpeed;

    for (const auto &item: items) {
        item->applyEffect(*this);
    }

    // level up stuff
    std::cout << "\n========== LEVEL UP ==========" << std::endl;
    std::cout << "Player reached Level " << level << "!" << std::endl;
    std::cout << "XP needed for next level: " << xpToNextLevel << std::endl;

    std::cout << "\n--- Player Stats ---" << std::endl;
    std::cout << "  Health: " << currentHealth << "/" << maxHealth << std::endl;
    std::cout << "  Speed: " << speed << std::endl;
    std::cout << "  Damage Reduction: " << (damageReduction * 100.f) << "%"
            << std::endl;
    std::cout << "  Cooldown Multiplier: " << cooldownMultiplier << "x"
            << std::endl;

    // weapon stts
    if (!weapons.empty()) {
        std::cout << "\n--- Weapons (" << weapons.size() << ") ---" << std::endl;
        for (const auto &weapon: weapons) {
            std::cout << "  " << weapon->getName() << " [Lv." << weapon->getLevel()
                    << "] - Damage: " << weapon->getDamage() << std::endl;
        }
    }

    // item stats
    if (!items.empty()) {
        std::cout << "\n--- Items (" << items.size() << ") ---" << std::endl;
        for (const auto &item: items) {
            std::cout << "  " << item->getName() << " [Lv." << item->getLevel() << "]"
                    << std::endl;
        }
    }

    std::cout << "==============================\n" << std::endl;
    validateState();
}

void Player::heal(float amount) {
    currentHealth += amount;
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }
}

void Player::setPos(sf::Vector2f pos) { sprite.setPosition(pos); }

void Player::validateState() const {
    if (level > PlayerValidation::MAX_LEVEL) {
        throw PlayerException("Level exceeds limit");
    }
    if (maxHealth > PlayerValidation::MAX_HEALTH) {
        throw PlayerException("Max Health exceeds limit");
    }
    if (currentXP > PlayerValidation::MAX_XP) {
        throw PlayerException("XP exceeds limit");
    }
    if (currentXP < PlayerValidation::MIN_XP) {
        throw PlayerException("XP is negative");
    }
}
