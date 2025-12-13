#include "Game.h"
#include "State.h"
#include <iostream>

sf::View LetterboxView(sf::View view, int winWidth, int winHeight) {
    float windowRatio = winWidth / (float) winHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
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

sf::Vector2f Game::getWindowSize() const {return sf::Vector2f((unsigned int)width, (unsigned int)height);}

Game::Game(const int width,const int height, const std::string &title) : title(title), width((unsigned int)width), height((unsigned int)height), fullscreen_toggle(0)
{
    window.create(sf::VideoMode({(unsigned int) width, (unsigned int) height}), title, sf::Style::Default);
    window.setVerticalSyncEnabled(true);
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) states.pop();
}

sf::RenderWindow& Game::getWindow() {return window;}
ResourceManager& Game::getResourceManager() {return resourceManager;}

void Game::run() {
    sf::Clock clock;

    sf::View view = window.getDefaultView();

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        if (states.empty()) {
            window.close();
            break;
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::Resized>()) {
                const auto* resized = event->getIf<sf::Event::Resized>();
                view = LetterboxView(view, resized->size.x, resized->size.y);
                window.setView(view);
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    shouldExit = true; // probabil ar trb sa fie in states ca sa am pauza dar acm meh
                else if (keyPressed->scancode == sf::Keyboard::Scancode::F) {
                    if (fullscreen_toggle == 0) {
                        fullscreen_toggle = 1;
                        window.create(sf::VideoMode::getDesktopMode(), title, sf::State::Fullscreen);
                        window.setVerticalSyncEnabled(true);
                    }
                    else {
                        fullscreen_toggle = 0;
                        window.create(sf::VideoMode({(unsigned int)width, (unsigned int)height}), title, sf::Style::Default);
                        window.setVerticalSyncEnabled(true);
                    }
                }
            }
        }
        //restu de event-uri le dau catch state-urile

        states.top()->handleInput();
        states.top()->update(dt);

        window.clear();
        states.top()->draw(dt);
        window.display();
    }
}