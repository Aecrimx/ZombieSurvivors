#ifndef OOP_RESOURCELOADEXCEPTION_H
#define OOP_RESOURCELOADEXCEPTION_H

#include "GameException.h"
#include <string>

class ResourceLoadException : public GameException {
public:
    explicit ResourceLoadException(const std::string &resourceName)
        : GameException("Failed to load resource: " + resourceName) {
    }

    //virtual ~ResourceLoadException() = default;
};

#endif // OOP_RESOURCELOADEXCEPTION_H
