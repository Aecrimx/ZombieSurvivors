#include <iostream>
#include "src/Game.h"
#include "src/GameState.h"

int main() {

    Game game(1200, 800, "Zombie Survivors");

    game.pushState(std::make_unique<GameState>(game));

    game.run();

    return 0;
}