#include "EnemySpawnManager.h"
#include <iostream>

EnemySpawnManager::EnemySpawnManager() : rng(std::random_device{}()) {
}

void EnemySpawnManager::addRule(const EnemyType type, const int weight,
                                const float startTime, const float endTime) {
    rules.push_back({type, weight, startTime, endTime});
}

EnemyType EnemySpawnManager::pickEnemy(const float currentTime) {
    std::vector<SpawnRule> activeRules;
    int totalWeight = 0;

    for (const auto &rule: rules) {
        bool timeCondition = currentTime >= rule.startTime &&
                             (rule.endTime == -1.0f || currentTime < rule.endTime);
        if (timeCondition) {
            activeRules.push_back(rule);
            totalWeight += rule.weight;
        }
    }

    if (activeRules.empty()) {
        return EnemyType::None;
    }

    std::uniform_int_distribution<int> dist(0, totalWeight - 1);
    const int randomValue = dist(rng);
    int currentWeight = 0;

    for (const auto &rule: activeRules) {
        currentWeight += rule.weight;
        if (randomValue < currentWeight) {
            std::cout << "Spawned Enemy Type: " << getEnemyName(rule.type)
                    << " (Weight: " << rule.weight << "/" << totalWeight << ")"
                    << std::endl;
            return rule.type;
        }
    }

    return EnemyType::None;
}

void EnemySpawnManager::reset() { rules.clear(); }

bool EnemySpawnManager::shouldSpawnBoss(const float currentTime) {
    return currentTime >= 900.0f; // 900 sec = 15 min
}

std::string EnemySpawnManager::getEnemyName(const EnemyType type) {
    switch (type) {
        case EnemyType::Zombie:
            return "Zombie";
        case EnemyType::Bat:
            return "Bat";
        case EnemyType::KnightZombie:
            return "Knight Zombie";
        case EnemyType::FlyingSkull:
            return "Flying Skull";
        case EnemyType::BigZombieBoss:
            return "Big Zombie Boss";
        case EnemyType::None:
            return "None";
        default:
            return "Unknown";
    }
}
