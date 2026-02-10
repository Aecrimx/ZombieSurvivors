//
// Created by Mircea on 15.12.2025.
//

#include "Enemy.h"

std::ostream &operator<<(std::ostream &os, const Enemy &obj) {
    obj.print(os);
    return os;
}
