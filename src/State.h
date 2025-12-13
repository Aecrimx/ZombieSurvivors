#ifndef OOP_STATE_H
#define OOP_STATE_H

#include <SFML/Graphics.hpp>

class Game;

class State {
protected:
    Game& game;

public:
    explicit State(Game& ref) : game(ref) {}
    virtual ~State() = default;

    virtual void handleInput() = 0;
    virtual void update(float dt) = 0;
    virtual void draw(float dt) = 0;
};


#endif //OOP_STATE_H