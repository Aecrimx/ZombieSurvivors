
#ifndef OOP_ANIMATION_H
#define OOP_ANIMATION_H

#include <SFML/Graphics.hpp>
/*
 *Used the tutorial https://github.com/SFML/SFML/wiki/Tutorial%3A-Easy-Animations-With-Spritesheets
 *for the structure of the class
 */

struct Frame {
    sf::IntRect rect;
    double duration;
};

class Animation {
    std::vector<Frame> frames;
    double totalLength;
    double progress;
    sf::Sprite &target;

public:
    Animation(sf::Sprite &target);
    virtual ~Animation();
    void addFrame(Frame&& frame);
    void update(double elapsed);

    double getLength() const;
};


#endif //OOP_ANIMATION_H