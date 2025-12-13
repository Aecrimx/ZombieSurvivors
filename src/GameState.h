
#ifndef OOP_GAMESTATE_H
#define OOP_GAMESTATE_H

#include "State.h"
#include "Game.h"



#include <memory>

class GameState : public State{
    std::unique_ptr<sf::Sprite> player;
    /*
     *TO DO later: clasa player care da aici ref
     */
    std::unique_ptr<sf::Sprite> background;
    sf::View view;
    //maybe view de GUI si aici?
    sf::Vector2f playerPos;

public:
    explicit GameState(Game& game);

    void handleInput() override;
    void update(float dt) override;
    void draw(float dt) override;
};


#endif //OOP_GAMESTATE_H