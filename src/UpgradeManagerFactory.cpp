#include "UpgradeManagerFactory.h"
#include <algorithm>


void UpgradeManagerFactory::registerItem(std::unique_ptr<Item> item, const std::string& description) {
    itemPrototypes.push_back({std::move(item), description});
}

void UpgradeManagerFactory::registerWeapon(std::unique_ptr<Weapon> weapon, const std::string& description) {
    weaponPrototypes.push_back({std::move(weapon), description});
}

std::vector<UpgradeData> UpgradeManagerFactory::getUpgradeOptions(const Player& player) const {
    std::vector<UpgradeData> options;

    auto newItems = getNewItemOptions(player);
    options.insert(options.end(), newItems.begin(), newItems.end());

    auto newWeapons = getNewWeaponOptions(player);
    options.insert(options.end(), newWeapons.begin(), newWeapons.end());

    auto itemLevelUps = getItemLevelUpOptions(player);
    options.insert(options.end(), itemLevelUps.begin(), itemLevelUps.end());
    
    auto weaponLevelUps = getWeaponLevelUpOptions(player);
    options.insert(options.end(), weaponLevelUps.begin(), weaponLevelUps.end());

    auto statUpgrades = getStatUpgradeOptions(player);
    options.insert(options.end(), statUpgrades.begin(), statUpgrades.end());

    return options;
}

std::vector<UpgradeData> UpgradeManagerFactory::getNewItemOptions(const Player& player) const {
    std::vector<UpgradeData> options;
    const auto& playerItems = player.getItems();

    if (playerItems.size() >= 3) {
        return options;
    }

    for (const auto& entry : itemPrototypes) {
        bool alreadyHas = false;
        for (const auto& item : playerItems) {
            if (item->getName() == entry.prototype->getName()) {
                alreadyHas = true;
                break;
            }
        }

        if (!alreadyHas) {
            Item* protoPtr = entry.prototype.get();
            options.push_back({
                protoPtr->getName(),
                entry.description,
                [protoPtr](Player& p) {
                    p.addItem(std::unique_ptr<Item>(protoPtr->clone()));
                }
            });
        }
    }
    return options;
}

std::vector<UpgradeData> UpgradeManagerFactory::getNewWeaponOptions(const Player& player) const {
    std::vector<UpgradeData> options;
    const auto& playerWeapons = player.getWeapons();

    if (playerWeapons.size() >= 3) {
        return options;
    }

    for (const auto& entry : weaponPrototypes) {
        bool alreadyHas = false;
        for (const auto& weapon : playerWeapons) {
            if (weapon->getName() == entry.prototype->getName()) {
                alreadyHas = true;
                break;
            }
        }

        if (!alreadyHas) {
            Weapon* protoPtr = entry.prototype.get();
            options.push_back({
                protoPtr->getName(),
                entry.description,
                [protoPtr](Player& p) {
                    p.addWeapon(std::unique_ptr<Weapon>(protoPtr->clone()));
                }
            });
        }
    }
    return options;
}

std::vector<UpgradeData> UpgradeManagerFactory::getItemLevelUpOptions(const Player& player) {
    std::vector<UpgradeData> options;
    for (const auto& item : player.getItems()) {
        if (item->canLevelUp()) {
            std::string itemName = item->getName();
            int nextLevel = item->getLevel() + 1;
            options.push_back({
                itemName + " Level Up",
                "Upgrade " + itemName + " to Level " + std::to_string(nextLevel),
                [itemName](const Player& p) {
                    for (auto& i : p.getItems()) {
                        if (i->getName() == itemName && i->canLevelUp()) {
                            i->levelUp();
                            break;
                        }
                    }
                }
            });
        }
    }
    return options;
}

std::vector<UpgradeData> UpgradeManagerFactory::getWeaponLevelUpOptions(const Player& player) {
    std::vector<UpgradeData> options;
    for (const auto& weapon : player.getWeapons()) {
        if (weapon->canLevelUp()) {
            std::string weaponName = weapon->getName();
            int nextLevel = weapon->getLevel() + 1;
            options.push_back({
                weaponName + " Level Up",
                "Upgrade " + weaponName + " to Level " + std::to_string(nextLevel),
                [weaponName](const Player& p) {
                    for (auto& w : p.getWeapons()) {
                        if (w->getName() == weaponName && w->canLevelUp()) {
                            w->levelUp();
                            break;
                        }
                    }
                }
            });
        }
    }
    return options;
}

std::vector<UpgradeData> UpgradeManagerFactory::getStatUpgradeOptions(const Player& /*player*/) {
    std::vector<UpgradeData> options;

    options.push_back({
        "Max HP Up", "+20 Maximum Health",
        [](Player& p) { p.increaseMaxHealth(20.f); }
    });
    
    options.push_back({
        "Health Regen", "+1.0 HP/sec Regeneration",
        [](Player& p) { p.setHealthRegen(p.getHealthRegen() + 1.0f); }
    });

    options.push_back({
        "Speed Up", "+15% Movement Speed",
        [](Player& p) { p.increaseSpeed(1.15f); }
    });

    return options;
}
