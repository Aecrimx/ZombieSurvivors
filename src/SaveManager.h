#ifndef OOP_SAVEMANAGER_H
#define OOP_SAVEMANAGER_H

#include "SaveData.h"
#include <fstream>
#include <string>
/*
 * pt tema 3 singleton
 */
class SaveManager {
private:
    SaveData data;
    std::string saveFilePath;

    SaveManager();

public:
    static SaveManager &getInstance() {
        static SaveManager instance;
        return instance;
    }

    SaveManager(const SaveManager &) = delete;

    SaveManager &operator=(const SaveManager &) = delete;

    void loadSave();

    void saveSave();

    // const SaveData &getData() const { return data; }

    bool isCharacterUnlocked(const std::string &character) const;

    int getHighScore(const std::string &character) const;

    void updateHighScore(const std::string &character, int score);

    // void unlockCharacter(const std::string &character);

    // int getFlyingSkullKills() const { return data.flyingSkullKills; }

    void incrementFlyingSkullKills();

    void incrementWins();

    void checkUnlocks();
};

#endif // OOP_SAVEMANAGER_H
