#include "protos.h"

int main () {

    Stack lol = StackCtor ();

    StackDump (lol);
    
    for (int i = 0; i < 40; i++) {

        StackPush (&lol, (i+1)*2);
    }

    StackDump (lol);

    for (int i = 0; i < 25; i++) {

        StackPop (&lol);
        StackPush (&lol, 19*i);
    }

    StackDump (lol);
    int t = StackPop (&lol);
    flog (t);

}