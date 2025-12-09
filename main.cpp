#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
class SomeClass {
public:
    explicit SomeClass(int) {}
};

SomeClass *getC() {
    return new SomeClass{2};
}
//////////////////////////////////////////////////////////////////////

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
    ///
    std::cout << "Hello, world!\n";
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////

    SomeClass *c = getC();
    std::cout << c << "\n";
    delete c;  // comentarea acestui rând ar trebui să ducă la semnalarea unui mem leak

    sf::RenderWindow window;
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    std::cout << "Fereastra a fost creată\n";
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                                    ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    //schimbarea valorilor de sus schimba rezolutia aparenta insa nu cred ca e okay asa:))
    window.setView(view);

    //player stuff
    sf::Texture texture("../test.png");
    texture.setSmooth(true);
    sf::Sprite sprite(texture);

    //scaling sprite mai mic
    sprite.setScale({0.2f, 0.2f});

    sprite.setColor(sf::Color(0, 255, 0));
    sprite.setOrigin({texture.getSize().x/2.f, texture.getSize().y/2.f});
    sf::Vector2<float> PlayerPos= {100.f, 100.f};


    sf::Texture backgroundTexture("../dirt.png");
    backgroundTexture.setRepeated(true);

    sf::RectangleShape background;
    background.setSize({4000.f, 4000.f});  // world size (big map)
    background.setTexture(&backgroundTexture);
    background.setTextureRect(sf::IntRect({0, 0}, {4000, 4000}));

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



        window.display();
    }

    std::cout << "Programul a terminat execuția\n";
    return 0;
}
