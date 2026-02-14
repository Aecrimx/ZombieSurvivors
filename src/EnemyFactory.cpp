#include "EnemyFactory.h"
#include "Bat.h"
#include "BigZombieBoss.h"
#include "Flying_Skull.h"
#include "KnightZombie.h"
#include "Player.h"
#include "Zombie.h"

EnemyFactory::EnemyFactory(ResourceManager &resManager, const Player &playerRef,
                           std::vector<Projectile> &projectilesRef)
    : resourceManager(resManager), player(playerRef),
      projectiles(projectilesRef) {
}

std::unique_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, sf::Vector2f position) {
    switch (type) {
        case EnemyType::Zombie:
            return std::make_unique<Zombie>(resourceManager.getTexture("zombie"), position);
        case EnemyType::Bat:
            return std::make_unique<Bat>(resourceManager.getTexture("bat"), position, player.getLevel());
        case EnemyType::KnightZombie:
            return std::make_unique<KnightZombie>(
                resourceManager.getTexture("knight_zombie"), position, player.getLevel());
        case EnemyType::FlyingSkull:
            return std::make_unique<Flying_Skull>(
                resourceManager.getTexture("spinning_skull"), position, projectiles, resourceManager);
        case EnemyType::BigZombieBoss:
            return std::make_unique<BigZombieBoss>(resourceManager.getTexture("boss"), position, projectiles, resourceManager, player.getLevel());
        default:
            return nullptr;
    }
}
