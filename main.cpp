#include "protos.h"
#include "flog.cpp"
#include "stack.cpp"


int main () {

    Stack lol = StackCtor ();

    StackDump (lol);
    
    for (int i = 0; i < 40; i++) {

        StackPush (&lol, (i+1)*2);
    }

    StackDump (lol);

    lol.hash = 1<<19;

    StackDump (lol);

    for (int i = 0; i < 25; i++) {

        StackPop (&lol);
    }

    StackDump (lol);

}