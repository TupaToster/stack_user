#include "protos.h"
#include "flog.cpp"
#include "stack.cpp"


int main () {

    Stack lol = StackCtor ();

    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackPush (&lol, 1);
    StackDump (lol);
    StackPop (&lol);
    StackPop (&lol);
    StackPop (&lol);
    StackPop (&lol);
    StackPop (&lol);
    StackPop (&lol);
    StackDump (lol);
}