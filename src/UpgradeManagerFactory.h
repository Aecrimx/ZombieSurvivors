#ifndef UPGRADEMANAGERFACTORY_H
#define UPGRADEMANAGERFACTORY_H

#include "Item.h"
#include "Weapon.h"
#include "Player.h"
#include <vector>
#include <memory>
#include <functional>
#include <string>

struct UpgradeData {
    std::string name;
    std::string description;
    std::function<void(Player&)> apply;
};

class UpgradeManagerFactory {
    struct ItemEntry {
        std::unique_ptr<Item> prototype;
        std::string description;
    };
    struct WeaponEntry {
        std::unique_ptr<Weapon> prototype;
        std::string description;
    };

    std::vector<ItemEntry> itemPrototypes;
    std::vector<WeaponEntry> weaponPrototypes;

public:
    void registerItem(std::unique_ptr<Item> item, const std::string& description);
    void registerWeapon(std::unique_ptr<Weapon> weapon, const std::string& description);

    std::vector<UpgradeData> getUpgradeOptions(const Player& player) const;

private:
    std::vector<UpgradeData> getNewItemOptions(const Player& player) const;
    std::vector<UpgradeData> getNewWeaponOptions(const Player& player) const;

    static std::vector<UpgradeData> getItemLevelUpOptions(const Player& player);

    static std::vector<UpgradeData> getWeaponLevelUpOptions(const Player& player);

    static std::vector<UpgradeData> getStatUpgradeOptions(const Player& player);
};

#endif // UPGRADEMANAGERFACTORY_H
