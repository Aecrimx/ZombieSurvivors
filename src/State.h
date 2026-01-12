#ifndef OOP_STATE_H
#define OOP_STATE_H

#include <ostream>
#include <SFML/Graphics.hpp>

class Game;

class State {
protected:
    Game &game;

public:
    explicit State(Game &ref) : game(ref) {
    }

    virtual ~State() = default;

    virtual void handleInput() = 0;

    virtual void update(float dt) = 0;

    virtual void draw() = 0;

    virtual void Resize(int w, int h) = 0; // pt fullscreen

    friend std::ostream &operator<<(std::ostream &os, const State & /*obj*/) {
        return os << "[State]" << ": din pacate si eu sunt doar o clasa abstracta si nu am ce afisa momentan";
    }
};

#endif // OOP_STATE_H
