#include "Game.h"
#include "State.h"

#include "Armor.h"
#include "Boots.h"
#include "CooldownGauntlet.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include "HeartCrystal.h"
#include "Knife.h"
#include "MagicGun.h"
#include "SoulLantern.h"

sf::View Game::LetterboxView(sf::View view, const int winWidth, const int winHeight) {
    const float windowRatio = winWidth / static_cast<float>(winHeight);
    const float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;

    if (windowRatio < viewRatio) {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    } else {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    }
    view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
    return view;
}

sf::Vector2f Game::getWindowSize() const {
    return {
        static_cast<float>(static_cast<unsigned int>(width)),
        static_cast<float>(static_cast<unsigned int>(height))
    };
}

Game::Game(const int width, const int height, const std::string &title)
    : title(title), width(static_cast<unsigned int>(width)), height(static_cast<unsigned int>(height)),
      fullscreen_toggle(0) {
    window.create(sf::VideoMode({
                      static_cast<unsigned int>(width),
                      static_cast<unsigned int>(height)
                  }),
                  title, sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    //Iteme
    upgradeManagerFactory.registerItem(std::make_unique<Armor>(), "+10% Damage Reduction");
    upgradeManagerFactory.registerItem(std::make_unique<Boots>(), "+10% Movement Speed");
    upgradeManagerFactory.registerItem(std::make_unique<CooldownGauntlet>(), "+16.67% Cooldown Reduction");
    upgradeManagerFactory.registerItem(std::make_unique<HeartCrystal>(), "+10 Max HP, +0.5 HP/sec Regen");

    //Arme
    upgradeManagerFactory.registerWeapon(std::make_unique<MagicGun>(resourceManager), "Shoots lasers in a clock-wise pattern.");
    upgradeManagerFactory.registerWeapon(std::make_unique<SoulLantern>(resourceManager), "Shoots a soul scream to the left and right.");
    upgradeManagerFactory.registerWeapon(std::make_unique<FireWand>(resourceManager), "Shoots fireballs towards enemies.");
    upgradeManagerFactory.registerWeapon(std::make_unique<DemonicBook>(resourceManager), "Damaging aura that periodically damages enemies.");
    upgradeManagerFactory.registerWeapon(std::make_unique<Knife>(resourceManager, nullptr), "Throws a knife in the direction you're facing.");
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    pendingPops++;
}

sf::RenderWindow &Game::getWindow() { return window; }
ResourceManager &Game::getResourceManager() { return resourceManager; }

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();

        if (states.empty()) {
            window.close();
            break;
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::Resized>()) {
                const auto *resized = event->getIf<sf::Event::Resized>();
                width = static_cast<int>(resized->size.x);
                height = static_cast<int>(resized->size.y);

                states.top()->Resize(width, height);

            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::F) {
                    if (fullscreen_toggle == 0) {
                        fullscreen_toggle = 1;
                        window.create(sf::VideoMode::getDesktopMode(), title,
                                      sf::State::Fullscreen);
                        window.setVerticalSyncEnabled(true);
                    } else {
                        fullscreen_toggle = 0;
                        window.create(
                            sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}),
                            title, sf::Style::Default);
                        window.setVerticalSyncEnabled(true);
                    }
                    const auto newSize = window.getSize();
                    width = static_cast<int>(newSize.x);
                    height = static_cast<int>(newSize.y);

                    if (!states.empty()) {
                        states.top()->Resize(width, height);
                    }
                }
            }

            states.top()->handleEvent(*event);
        }

        states.top()->handleInput();
        states.top()->update(dt);

        while (pendingPops > 0 && !states.empty()) {
            states.pop();
            pendingPops--;
        }

        if (hasPendingReplace) {
            if (!states.empty())
                states.pop();
            states.push(std::move(pendingState));
            hasPendingReplace = false;
            states.top()->Resize(width, height);
        }

        window.clear();
        if (!states.empty()) {
            states.top()->draw();
        }
        window.display();
    }
}

void Game::scheduleReplace(std::unique_ptr<State> state) {
    pendingState = std::move(state);
    hasPendingReplace = true;
}
