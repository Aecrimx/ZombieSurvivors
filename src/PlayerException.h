#ifndef OOP_INVALIDSTATEEXCEPTION_H
#define OOP_INVALIDSTATEEXCEPTION_H

#include "GameException.h"
#include <string>

class PlayerException : public GameException {
public:
    explicit PlayerException(const std::string &msg)
        : GameException("Invalid State: " + msg) {
    }
    //virtual ~PlayerException() = default;
};

#endif // OOP_INVALIDSTATEEXCEPTION_H
