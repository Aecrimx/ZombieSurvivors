#ifndef OOP_SAVEMANAGER_H
#define OOP_SAVEMANAGER_H

#include "SaveData.h"
#include <fstream>
#include <string>
/*
 * pt tema 3 singleton
 */
#include "SingletonTemplate.h"

class SaveManager : public Singleton<SaveManager> {
private:
    SaveData data;
    std::string saveFilePath;

    void checkUnlocks();

protected:
     SaveManager();

public:

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


};

#endif // OOP_SAVEMANAGER_H
