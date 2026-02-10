#ifndef OOP_SAVEDATAEXCEPTION_H
#define OOP_SAVEDATAEXCEPTION_H

#include "GameException.h"
#include <string>

class SaveDataException : public GameException {
public:
    explicit SaveDataException(const std::string &msg)
        : GameException("Save Data Error: " + msg) {
    }

    //virtual ~SaveDataException() = default;
};

#endif // OOP_SAVEDATAEXCEPTION_H
