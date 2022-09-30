#include "config.h"

///@todo poison check, hash,


Stack StackCtor () {

    Stack stk = {};

    stk.canL      = CANL;
    stk.canR      = CANR;
    stk.size      = 0;
    stk.capacity  = 4;

    stk.stack     = (ELEM_TYPE*) calloc (stk.capacity * sizeof (ELEM_TYPE) + 2 * sizeof (unsigned int), 1);
    stk.stackCanL = (unsigned int*) stk.stack;
    stk.stack     = (ELEM_TYPE*)(((unsigned int*) stk.stack) + 1);
    stk.stackCanR = (unsigned int*) (stk.stack + stk.capacity);

   *stk.stackCanL = CANL;
   *stk.stackCanR = CANR;

    return stk;
}

void StackDtor (Stack* stk) {

    stk->canL      = POISON4;
    stk->canR      = POISON4;
    stk->size      = POISON4;
   *stk->stackCanL = POISON4;
   *stk->stackCanR = POISON4;

    for (int i = 0; i < stk->capacity;i++) {

        if (sizeof (stk->stack[0]) == 1)      stk->stack[i] = POISON1;
        else if (sizeof (stk->stack[0]) == 2) stk->stack[i] = POISON2;
        else if (sizeof (stk->stack[0]) == 4) stk->stack[i] = POISON4;
        else if (sizeof (stk->stack[0]) == 8) stk->stack[i] = POISON8;
    }

    stk->capacity  = POISON4;

    free (stk->stackCanL);
    stk->stack = (ELEM_TYPE*)POISON4;
    free (stk);
}

void StackPush (Stack* stk, ELEM_TYPE val) {

    if (stk->size == stk->capacity) StackSizeShift (stk, 1);

    stk->stack[stk->size] = val;
    stk->size++;
}

ELEM_TYPE StackPop (Stack* stk) {

    ELEM_TYPE retVal = stk->stack[stk->size - 1];
    stk->stack[stk->size-1] = 0;
    stk->size--;

    if (stk->capacity > 4 and stk->size == stk->capacity * 3 / 8) StackSizeShift (stk, -1);

    return retVal;
}

void StackSizeShift (Stack* stk, int delta) {

   *((unsigned int*) (stk->stack + stk->capacity)) = 0;

    if (delta > 0) {

        stk->stack = (ELEM_TYPE*) calloc (stk->capacity * sizeof (ELEM_TYPE) * 2 + 2 * sizeof (unsigned int), 1);
        memcpy (stk->stack, stk->stackCanL, stk->capacity * sizeof (ELEM_TYPE) + 2 * sizeof (unsigned int));
        stk->capacity *= 2;

    }
    else if (delta < 0) {

        stk->stack = (ELEM_TYPE*) calloc (stk->capacity * sizeof (ELEM_TYPE) / 2  + 2 * sizeof (unsigned int), 1);
        memcpy (stk->stack, stk->stackCanL, stk->capacity * sizeof (ELEM_TYPE) / 2 + 2 * sizeof (unsigned int));
        stk->capacity /= 2;
    }

    free (stk->stackCanL);

    stk->stackCanL = (unsigned int*) stk->stack;

    stk->stack = (ELEM_TYPE*) (((unsigned int*) stk->stack) + 1);

    stk->stackCanR = (unsigned int*) (stk->stack + stk->capacity * sizeof (ELEM_TYPE));
   *stk->stackCanR = CANR;
}

void insideStackDump (Stack* stk, const char* StackName, const char* fileName, const char* funcName, size_t line) {

    flogFileInit;

    fprintf (logOutf, "In file %s, function %s, line %u, stack named %s was dumped : \n", fileName, funcName, line, StackName);

    fprintf (logOutf, "\tcanL = 0x%X (", stk->canL);
    if (stk->canL == POISON4) fprintf (logOutf, "POISONED)\n");
    else if (stk->canL == CANL) fprintf (logOutf, "ok)\n");
    else fprintf (logOutf, "NOT_OK)\n");

    fprintf (logOutf, "\tcanR = 0x%X (", stk->canR);
    if (stk->canR == POISON4) fprintf (logOutf, "POISONED)\n");
    else if (stk->canR == CANR) fprintf (logOutf, "ok)\n");
    else fprintf (logOutf, "NOT_OK)\n");

    fprintf (logOutf, "\tsize = %u (", stk->size);
    if (stk->size == POISON4) fprintf (logOutf, "POISONED)\n");
    else fprintf (logOutf, "ok)\n");

    fprintf (logOutf, "\tcapacity = %u (", stk->capacity);
    if (stk->capacity == POISON4) fprintf (logOutf, "POISONED)\n");
    else fprintf (logOutf, "ok)\n");

    fprintf (logOutf, "\tstackCanL = 0x%X (", *stk->stackCanL);
    if (*stk->stackCanL == POISON4) fprintf (logOutf, "POISONED)\n");
    else if (*stk->stackCanL == CANL) fprintf (logOutf, "ok)\n");
    else fprintf (logOutf, "NOT_OK)\n");

    fprintf (logOutf, "\tstackCanR = 0x%X (", *stk->stackCanR);
    if (*stk->stackCanR == POISON4) fprintf (logOutf, "POISONED)\n");
    else if (*stk->stackCanR == CANR) fprintf (logOutf, "ok)\n");
    else fprintf (logOutf, "NOT_OK)\n");

    if ((size_t) stk->stack == POISON4) return;

    fprintf (logOutf, "\t%s.stack : \n", StackName);
    for (int i = 0; i < stk->capacity; i++) {

        fprintf (logOutf, "\t\t%s.stack[%d] : " OUT_FORM "\n", StackName, i, stk->stack[i]);
    }
    fprintf (logOutf, "\tEnd of stack\nEnd of dump\n");
}