#include "src/Game.h"
#include "src/InvalidStateException.h"
#include "src/MenuState.h"
#include "src/ResourceLoadException.h"
#include "src/SaveDataException.h"
#include <iostream>

int main() {
    try {
        Game game(1200, 800, "Zombie Survivors");

        game.pushState(std::make_unique<MenuState>(game));

        game.run();

        return 0;
    } catch (const ResourceLoadException &e) {
        std::cerr << "Resource Error: " << e.what() << std::endl;
        return 1;
    } catch (const SaveDataException &e) {
        std::cerr << "Save Data Error: " << e.what() << std::endl;
        return 2;
    } catch (const InvalidStateException &e) {
        std::cerr << "Game State Error: " << e.what() << std::endl;
        return 3;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 99;
    }
}
