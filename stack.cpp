#include "config.h"

///@todo poison check, hash, memset

void setPoisonInside (void* var, size_t sizeofVar) {

    switch(sizeofVar) {
        case 1 : *((unsigned char*     ) var)      = POISON1; break;
        case 2 : *((unsigned short*    ) var)      = POISON2; break;
        case 4 : *((unsigned int*      ) var)      = POISON4; break;
        case 8 : *((unsigned long long*) var)      = POISON8; break;
        default:
            for (int i = 0; i < sizeofVar;i++) {

                 *(((unsigned char*    ) var) + i) = POISON1;
            }
        break;
    }
}

bool isPoisonInside (void* var, size_t sizeofVar) {

    switch(sizeofVar) {
        case 1:
            if ( *( (unsigned char*     ) var) == POISON1) return 1;
            else                                           return 0;
            break;
        case 2:
            if ( *( (unsigned short*    ) var) == POISON2) return 1;
            else                                           return 0;
            break;
        case 4:
            if ( *( (unsigned int*      ) var) == POISON1) return 1;
            else                                           return 0;
            break;
        case 8:
            if ( *( (unsigned long long*) var) == POISON1) return 1;
            else                                           return 0;
            break;
        default:
            for (int i = 0; i < sizeofVar; i++) {

                if (*( ( (unsigned char*) var) + i) != POISON1) return 0;
            }
            return 1;
    }
}

Stack StackCtor () {

    Stack stk = {};

    stk.canL      = CANL;
    stk.canR      = CANR;
    stk.size      = 0;
    stk.capacity  = 4;
    stk.errCode   = ok;

    stk.stack     = (ELEM_TYPE*) calloc (stk.capacity * sizeof (ELEM_TYPE) + 2 * sizeof (unsigned int), 1);
    stk.stackCanL = (unsigned int*) stk.stack;
    stk.stack     = (ELEM_TYPE*)(((unsigned int*) stk.stack) + 1);
    stk.stackCanR = (unsigned int*) (stk.stack + stk.capacity);

   *stk.stackCanL = CANL;
   *stk.stackCanR = CANR;

    return stk;
}

void StackDtor (Stack* stk) {

    setPoison ( stk->canL      );
    setPoison ( stk->canR      );
    setPoison ( stk->size      );
    setPoison (*stk->stackCanL );
    setPoison (*stk->stackCanR );
    setPoison ( stk->stackCanL );
    setPoison ( stk->stackCanR );
    setPoison ( stk->errCode   );

    for (int i = 0; i < stk->capacity;i++) setPoison (stk->stack[i]);

    setPoison ( stk->capacity  );

    free      ( stk->stackCanL );
    setPoison ( stk->stack     );
    free (stk);
}

void StackPush (Stack* stk, ELEM_TYPE val) {

    if (StackErrCheck (stk) & POISONED_ERRCOD) return;

    if (stk->size == stk->capacity) StackSizeShift (stk, 1);

    stk->stack[stk->size] = val;
    stk->size++;

    StackErrCheck (stk);
}

ELEM_TYPE StackPop (Stack* stk) {

    if (StackErrCheck (stk)&POISONED_ERRCOD) return 0;

    ELEM_TYPE retVal = stk->stack[stk->size - 1];
    stk->stack[stk->size-1] = 0;
    stk->size--;

    if (stk->capacity > 4 and stk->size == stk->capacity * 3 / 8) StackSizeShift (stk, -1);

    StackErrCheck (stk);
    return retVal;
}

void StackSizeShift (Stack* stk, int delta) {

    if (StackErrCheck (stk) & POISONED_ERRCOD) return;

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

   StackErrCheck (stk);
}

void StackDumpInside (Stack* stk, const char* StackName, const char* fileName, const char* funcName, size_t line) {

    flogFileInit;

    if (stk == NULL or StackErrCheck (stk)&POISONED_ERRCOD){
        fprintf (logOutf, "In file %s, function %s, line %u, stack named %s is destroyed and has no values\n", fileName, funcName, line, StackName);
        return;
    }


    fprintf (logOutf, "In file %s, function %s, line %u, stack named %s was dumped : \n", fileName, funcName, line, StackName);

    fprintf (logOutf, "\tErrors : \n");
    StackLogPrintErrors (stk);

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

        fprintf (logOutf, "\t\t%s.stack[%d] : " OUT_FORM , StackName, i, stk->stack[i]);
        if (isPoison (stk->stack[i])) fprintf (logOutf, " (POISONED)");
        fprintf (logOutf, "\n");
    }
    fprintf (logOutf, "\tEnd of stack\nEnd of dump\n");
}

unsigned long long StackErrCheck (Stack* stk) {

    //checking for poison
    if (isPoison ( stk->errCode  )) stk->errCode = POISONED_ERRCOD;
    if (isPoison ( stk->canL     ) or
        isPoison ( stk->canR     ) or
        isPoison ( stk->capacity ) or
        isPoison ( stk->size     ) or
        isPoison ( stk->stack    ) or
        isPoison ( stk->stackCanL) or
        isPoison (*stk->stackCanL) or
        isPoison ( stk->stackCanR) or
        isPoison (*stk->stackCanR)    ) stk->errCode |= POISON_ACCESS;
    else {

        for (int i = 0; i < stk->capacity;i++) {

            if (isPoison (stk->stack[i])) {

                stk->errCode |= POISON_ACCESS;
                break;
            }
        }
    }
    //endof check

    if ( stk->canL      != CANL         ) stk->errCode |= BAD_CAN_L;
    if ( stk->canR      != CANR         ) stk->errCode |= BAD_CAN_R;
    if ( stk->stackCanL == NULL         ) stk->errCode |= NULL_STACK_CAN_L_PTR;
    if ( stk->stackCanR == NULL         ) stk->errCode |= NULL_STACK_CAN_R_PTR;
    if (*stk->stackCanL != CANL         ) stk->errCode |= BAD_STACK_CAN_L;
    if (*stk->stackCanR != CANR         ) stk->errCode |= BAD_STACK_CAN_R;
    if ( stk->stack     == NULL         ) stk->errCode |= NULL_STACK_PTR;
    if ( stk->size      >  stk->capacity) stk->errCode |= WRONG_SIZE;

    return stk->errCode;
}

void StackLogPrintErrors (Stack* stk) {

    char names [10][40] = {};
    int iter = 0;
    if (stk->errCode & POISON_ACCESS        ) strcpy (names[iter++], "\t\t[POISON_ACCESS       ]\n");
    if (stk->errCode & BAD_CAN_L            ) strcpy (names[iter++], "\t\t[BAD_CAN_L           ]\n");
    if (stk->errCode & BAD_CAN_R            ) strcpy (names[iter++], "\t\t[BAD_CAN_R           ]\n");
    if (stk->errCode & BAD_STACK_CAN_L      ) strcpy (names[iter++], "\t\t[BAD_STACK_CAN_L     ]\n");
    if (stk->errCode & BAD_STACK_CAN_R      ) strcpy (names[iter++], "\t\t[BAD_STACK_CAN_R     ]\n");
    if (stk->errCode & NULL_STACK_PTR       ) strcpy (names[iter++], "\t\t[NULL_STACK_PTR      ]\n");
    if (stk->errCode & NULL_STACK_CAN_L_PTR ) strcpy (names[iter++], "\t\t[NULL_STACK_CAN_L_PTR]\n");
    if (stk->errCode & NULL_STACK_CAN_R_PTR ) strcpy (names[iter++], "\t\t[NULL_STACK_CAN_R_PTR]\n");
    if (stk->errCode & WRONG_SIZE           ) strcpy (names[iter++], "\t\t[WRONG_SIZE          ]\n");
    if (stk->errCode & POISONED_ERRCOD      ) strcpy (names[iter++], "\t\t[POISONED_ERRCOD     ]\n");

    if (iter == 0) fprintf (logOutf, "\t\t[ok]\n");
    else
        for (int i = 0; i < iter; i++) fprintf (logOutf, "%s", names[i]);

}
