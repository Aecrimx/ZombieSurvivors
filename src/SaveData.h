#ifndef OOP_SAVEDATA_H
#define OOP_SAVEDATA_H

#include <map>
#include <string>

struct CharacterSaveData {
    bool unlocked;
    int highScore;

    CharacterSaveData() : unlocked(false), highScore(0) {
    }

    CharacterSaveData(bool unlock, int score)
        : unlocked(unlock), highScore(score) {
    }
};
/*
 * Hocus e unlocked by default
 * Demonul are un win requirement
 * flying skull 5 flying skull kills
 */
struct SaveData {
    std::map<std::string, CharacterSaveData> characters;
    int flyingSkullKills;
    int totalWins;

    SaveData() : flyingSkullKills(0), totalWins(0) {
        characters["wizard"] =
                CharacterSaveData(true, 0);
        characters["demon"] = CharacterSaveData(false, 0);
        characters["flying_skull"] =
                CharacterSaveData(false, 0);
    }
};

#endif // OOP_SAVEDATA_H
