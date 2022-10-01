#include "protos.h"

int main () {

    Stack lol = StackCtor ();

    for (int i = 0; i < 40; i++) {

        StackPush (&lol, 19*i);
    }

    int t = StackPop (&lol);
    flog (t);

}