#ifndef ENEMY_SPAWN_MANAGER_H
#define ENEMY_SPAWN_MANAGER_H

#include "SingletonTemplate.h"
#include <random>
#include <vector>

enum class EnemyType {
    Zombie,
    Bat,
    KnightZombie,
    FlyingSkull,
    BigZombieBoss,
    None
};

struct SpawnRule {
    EnemyType type;
    int weight;
    float startTime;
    float endTime; // -1.0f pt infinit
};

class EnemySpawnManager : public Singleton<EnemySpawnManager> {
public:
    void addRule(EnemyType type, int weight, float startTime,
                 float endTime = -1.0f);

    EnemyType pickEnemy(float currentTime);

    static bool shouldSpawnBoss(float currentTime);

    void reset();

protected:
    EnemySpawnManager();

private:
    std::vector<SpawnRule> rules;
    std::mt19937 rng;

    static std::string getEnemyName(EnemyType type);
};

#endif // ENEMY_SPAWN_MANAGER_H
