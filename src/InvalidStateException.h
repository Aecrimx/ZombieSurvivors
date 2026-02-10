#ifndef OOP_INVALIDSTATEEXCEPTION_H
#define OOP_INVALIDSTATEEXCEPTION_H

#include "GameException.h"
#include <string>

class InvalidStateException : public GameException {
public:
    explicit InvalidStateException(const std::string &msg)
        : GameException("Invalid State: " + msg) {
    }
    //virtual ~InvalidStateException() = default;
};

#endif // OOP_INVALIDSTATEEXCEPTION_H
