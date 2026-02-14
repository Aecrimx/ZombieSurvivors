#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "Enemy.h"
#include "EnemySpawnManager.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Player;

class EnemyFactory {
public:
    EnemyFactory(ResourceManager &resManager, const Player &playerRef,
                 std::vector<Projectile> &projectilesRef);

    std::unique_ptr<Enemy> createEnemy(EnemyType type, sf::Vector2f position);

private:
    ResourceManager &resourceManager;
    const Player &player;
    std::vector<Projectile> &projectiles;
};

#endif // ENEMY_FACTORY_H
