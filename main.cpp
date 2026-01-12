#include "src/Game.h"
#include "src/MenuState.h"
#include <iostream>

int main() {

  Game game(1200, 800, "Zombie Survivors");

  game.pushState(std::make_unique<MenuState>(game));

  game.run();

  return 0;
}