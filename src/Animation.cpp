#include "Animation.h"
#include <cmath>

Animation::Animation(sf::Sprite &target) : target(target) {
    progress = totalLength = 0.f;
}

Animation::~Animation() = default;

void Animation::addFrame(Frame &&frame) {
    totalLength += frame.duration;
    frames.push_back(std::move(frame));
}

void Animation::update(const double elapsed) {
    if (frames.empty())
        return;

    progress += elapsed;

    if (totalLength > 0.0)
        progress = std::fmod(progress, totalLength);

    double time = progress;

    for (const auto &frame: frames) {
        if (time <= frame.duration) {
            target.setTextureRect(frame.rect);
            return;
        }
        time -= frame.duration;
    }

    target.setTextureRect(frames.back().rect);
}

// double Animation::getLength() const {return totalLength;}
