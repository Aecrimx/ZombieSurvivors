#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

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


int main() {
    sf::RenderWindow window;

    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Zombie Survivors", sf::Style::Default);
    std::cout << "Fereastra a fost creată\n";
    window.setVerticalSyncEnabled(true);

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    //schimbarea valorilor de sus schimba rezolutia aparenta insa nu cred ca e okay asa:))
    window.setView(view);

    //player stuff
    sf::Texture texture("../assets/wizard.png");
    texture.setSmooth(false);
    sf::Sprite sprite(texture);

    //scaling sprite mai mic
    sprite.setScale({0.2f, 0.2f});

    //sprite.setColor(sf::Color(0, 255, 0));
    sprite.setOrigin({texture.getSize().x/2.f, texture.getSize().y/2.f});
    sf::Vector2<float> PlayerPos= {100.f, 100.f};


    sf::Texture backgroundTexture("../assets/grass.png");
    backgroundTexture.setRepeated(true);

    sf::RectangleShape background;
    background.setSize({4000.f, 4000.f});  // world size (big map)
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));

    //test text
    sf::Font font("../fonts/game_over.ttf");
    sf::Text text(font);

    text.setString("Hello world!:)");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::White);

    int toggle = 0;
    while(window.isOpen()) {


        bool shouldExit = false;

        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                std::cout << "Fereastra a fost închisă\n";
            }
            else if (event->is<sf::Event::Resized>()) {
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
                const auto* resized = event->getIf<sf::Event::Resized>();
                view = LetterboxView(view, resized->size.x, resized->size.y);
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                //std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::F) {
                    if (toggle == 0) {
                        toggle = 1;
                        window.create(sf::VideoMode({1920, 1080}), "My Window", sf::Style::Default, sf::State::Fullscreen);
                        window.setVerticalSyncEnabled(true);
                    }
                    else {
                        toggle = 0;
                        window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default, sf::State::Windowed);
                        window.setVerticalSyncEnabled(true);
                    }
                }
            }
        }
        if(shouldExit) {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }
        const float speed = 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) PlayerPos.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) PlayerPos.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) PlayerPos.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) PlayerPos.x += speed;


        window.setView(view);
        view.setCenter(PlayerPos);

        window.clear();


        window.draw(background);

        sprite.setPosition(PlayerPos);
        window.draw(sprite);

        //sf::View currentView = window.getView();

        window.draw(text);

        window.display();
    }

    std::cout << "Programul a terminat execuția\n";
    return 0;
}
