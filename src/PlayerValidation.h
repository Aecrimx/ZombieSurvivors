#ifndef OOP_PLAYER_VALIDATION_H
#define OOP_PLAYER_VALIDATION_H

//Teoretic sunt niste constante la care oricum prin gameplay normal
//nu ai avea cum sa ajungi la ele
//(decat daca folosesti cv in genul CheatEngine si modifici exact valoarea din memorie)

namespace PlayerValidation {
const int MAX_LEVEL = 100;
const float MAX_HEALTH = 10000.f;
const float MIN_XP = 0.f;
const float MAX_XP = 100000.f;
}

#endif // OOP_PLAYER_VALIDATION_H
