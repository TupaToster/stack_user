#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef FLOG_DEBUG_TIME
#include "flog.cpp"
#endif

//Constants to specify stack element type and element output mode

#define ELEM_TYPE int
#define OUT_FORM "%d"

//end of block

//inside defines
#define CANL 0xDEADBEEF
#define CANR 0xD34DB33F

#define POISON1 0xBD
#define POISON2 0xBDCF
#define POISON4 0xBADC0FEE
#define POISON8 0xBADC0FEEF04DED32

#define MULT 37u

#define setPoison(var) setPoisonInside (&var, sizeof (var))

#define isPoison(var) isPoisonInside (&var, sizeof (var))

#define StackDump(stk) StackDumpInside (&stk, #stk, __FILE__, __FUNCTION__, __LINE__)

//endof inside defines
enum StackErrorCodes {
    ok                   = 0,
    POISON_ACCESS        = 1<<0,
    BAD_CAN_L            = 1<<1,
    BAD_CAN_R            = 1<<2,
    BAD_STACK_CAN_L      = 1<<3,
    BAD_STACK_CAN_R      = 1<<4,
    NULL_STACK_PTR       = 1<<5,
    NULL_STACK_CAN_L_PTR = 1<<6,
    NULL_STACK_CAN_R_PTR = 1<<7,
    WRONG_SIZE           = 1<<8,
    POISONED_ERRCOD      = 1<<9,
    WRONG_HASH           = 1<<10
};

struct Stack {

    unsigned int  canL      = 0xDEADBEEF;
    unsigned int  hash      = 0;
    size_t        size      = 0;
    size_t        capacity  = 0;
    size_t        errCode   = ok;
    ELEM_TYPE*    stack     = NULL;
    unsigned int* stackCanL = NULL;
    unsigned int* stackCanR = NULL;
    unsigned int  canR      = 0xD34DB33F;
};

Stack StackCtor ();

void StackDtor (Stack* stk);

void StackPush (Stack* stk, ELEM_TYPE val);

ELEM_TYPE StackPop (Stack* stk);

void ReallocStackSizeShift (Stack* stk, int delta);

void insideStackDump (Stack* stk, const char* StackName, const char* fileName, const char* funcName, size_t line);

unsigned long long StackErrCheck (Stack* stk);

void StackLogPrintErrors (Stack* stk);

void StackCountHash (Stack* stk);

void StackVerifyHash (Stack* stk);
