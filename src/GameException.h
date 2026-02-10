#ifndef OOP_GAMEEXCEPTION_H
#define OOP_GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;

public:
    explicit GameException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
    //virtual ~GameException() = default;
};

#endif // OOP_GAMEEXCEPTION_H
