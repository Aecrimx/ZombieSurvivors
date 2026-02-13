#include "SaveManager.h"
#include "SaveDataException.h"
#include <iostream>
#include <json.hpp>
#include <sys/stat.h>
#include <sys/types.h>

using nlohmann::json;

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

SaveManager::SaveManager() : saveFilePath("saves/save.json") { loadSave(); }

void SaveManager::loadSave() {
    std::ifstream file(saveFilePath);
    if (!file.is_open()) {
        std::cout << "Save file not found, using defaults" << std::endl;
        data = SaveData();
        saveSave();
        return;
    }

    try {
        json j;
        file >> j;

        data = SaveData();

        if (j.contains("characters") && j["characters"].is_object()) {
            for (auto it = j["characters"].begin(); it != j["characters"].end();
                 ++it) {
                const std::string name = it.key();
                const auto &cobj = it.value();
                if (!cobj.is_object())
                    continue;
                bool unlocked = cobj.value("unlocked", data.characters[name].unlocked);
                int hs = cobj.value("high_score", data.characters[name].highScore);
                data.characters[name] = CharacterSaveData(unlocked, hs);
            }
        }

        if (j.contains("stats") && j["stats"].is_object()) {
            const auto &s = j["stats"];
            data.flyingSkullKills =
                    s.value("flying_skull_kills", data.flyingSkullKills);
            data.totalWins = s.value("total_wins", data.totalWins);
        }
    } catch (const json::exception &e) {
        throw SaveDataException("Corrupted save file: " + std::string(e.what()));
    } catch (const std::exception &e) {
        throw SaveDataException("Save error: " + std::string(e.what()));
    }

    file.close();
    checkUnlocks();
}

void SaveManager::saveSave() {
    mkdir("saves", 0777);

    std::ofstream file(saveFilePath);
    if (!file.is_open()) {
        // continue, se va repara fisierul ca un default.
        return;
    }

    json j;
    json jchars = json::object();
    for (const auto &pair: data.characters) {
        json c = json::object();
        c["unlocked"] = pair.second.unlocked;
        c["high_score"] = pair.second.highScore;
        jchars[pair.first] = std::move(c);
    }
    j["characters"] = std::move(jchars);
    json jstats = json::object();
    jstats["flying_skull_kills"] = data.flyingSkullKills;
    jstats["total_wins"] = data.totalWins;
    j["stats"] = std::move(jstats);

    file << j.dump(2) << std::endl;
    file.close();
}

bool SaveManager::isCharacterUnlocked(const std::string &character) const {
    auto it = data.characters.find(character);
    if (it != data.characters.end()) {
        return it->second.unlocked;
    }
    return false;
}

int SaveManager::getHighScore(const std::string &character) const {
    auto it = data.characters.find(character);
    if (it != data.characters.end()) {
        return it->second.highScore;
    }
    return 0;
}

void SaveManager::updateHighScore(const std::string &character, int score) {
    auto it = data.characters.find(character);
    if (it != data.characters.end()) {
        if (score > it->second.highScore) {
            it->second.highScore = score;
            saveSave();
        }
    }
}

// void SaveManager::unlockCharacter(const std::string &character) {
//     auto it = data.characters.find(character);
//     if (it != data.characters.end()) {
//         it->second.unlocked = true;
//         saveSave();
//     }
// }

void SaveManager::incrementFlyingSkullKills() {
    data.flyingSkullKills++;
    checkUnlocks();
    saveSave();
}

void SaveManager::incrementWins() {
    data.totalWins++;
    checkUnlocks();
    saveSave();
}

void SaveManager::checkUnlocks() {
    if (data.totalWins >= 1) {
        data.characters["demon"].unlocked = true;
    }

    if (data.flyingSkullKills >= 5) {
        data.characters["flying_skull"].unlocked = true;
    }
}
