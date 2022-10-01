#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "flog.cpp"

// DEFINE NDEBUG TO DISABLE LOGS
//Constants to specify stack element type and element output mode

#define ELEM_TYPE int ///< Type of element a stack operates with
#define OUT_FORM "%d" ///< Output format for printf functions

//end of block

//inside defines

#define CANL 0xDEADBEEF ///< Left cannary of a structure
#define CANR 0xD34DB33F ///< Right cannary of a structure

#define POISON1 0xBD ///< 1 byte Poison
#define POISON2 0xBDCF ///< 2 byte Poison
#define POISON4 0xBADC0FEE ///< 4 byte Poison
#define POISON8 0xBADC0FEEF04DED32 ///< 8 byte Poison

#define MULT 37u ///< Multiplier for hash

#define setPoison(var) setPoisonInside (&var, sizeof (var)) ///< Macros for auto-poison

#define isPoison(var) isPoisonInside (&var, sizeof (var)) ///< Macros for checking for poison

#ifndef NDEBUG
#define StackDump(stk) StackDumpInside (&stk, #stk, __FILE__, __FUNCTION__, __LINE__) ///< Stack dump macros
#else
#define StackDump(stk) ;
#endif
//endof inside defines

/// @brief Describes errors within stack
enum StackErrorCodes {
    ok                   = 0,    ///< All ok
    POISON_ACCESS        = 1<<0, ///< One or more struct elements is poison
    BAD_CAN_L            = 1<<1, ///< Dead left cannary of structure
    BAD_CAN_R            = 1<<2, ///< Dead right cannary of structure
    BAD_STACK_CAN_L      = 1<<3, ///< Dead left cannary of stack
    BAD_STACK_CAN_R      = 1<<4, ///< Dead right cannary of stack
    NULL_STACK_PTR       = 1<<5, ///< NULL ptr for stack
    NULL_STACK_CAN_L_PTR = 1<<6, ///< NULL ptr for left stack cannary
    NULL_STACK_CAN_R_PTR = 1<<7, ///< NULL ptr for right stack cannary
    WRONG_SIZE           = 1<<8, ///< Size is more than capacity
    POISONED_ERRCOD      = 1<<9, ///< Errcod variable is poisoned; Ususally means that struct has been destructed
    WRONG_HASH           = 1<<10 ///< Hash was changed without any changes from specified function
};

/// @brief Stores a stack with several support variables
struct Stack {

    unsigned int  canL      = 0xDEADBEEF; ///< left cannary of struct
    unsigned int  hash      = 0;          ///< hash value
    size_t        size      = 0;          ///< size of stack
    size_t        capacity  = 0;          ///< capacity of stack
    size_t        errCode   = ok;         ///< error code
    ELEM_TYPE*    stack     = NULL;       ///< ptr to stack
    unsigned int* stackCanL = NULL;       ///< left cannary of stack
    unsigned int* stackCanR = NULL;       ///< right cannary of stack
    unsigned int  canR      = 0xD34DB33F; ///< right cannary of struct
};

/// @brief Constructor for Stack; Sets default capacity to 4
/// @return Fully operable Stack struct
Stack StackCtor ();

/// @brief Destroys a stack var.
/// @param stk ptr to a destroyable variable
void StackDtor (Stack* stk);

/// @brief Places an element at the end of a stack
/// @param stk ptr to stack struct
/// @param val value of an element to push
void StackPush (Stack* stk, ELEM_TYPE val);

/// @brief Removes an element from stack and returns its value
/// @param stk ptr to stack
/// @return element that was removed
ELEM_TYPE StackPop (Stack* stk);

/// @brief Recallocates a whole stak with a size shift
/// @param stk ptr to stack
/// @param delta direction of sie shift: >0 - doubles the size, <0 halves the size
void ReallocStackSizeShift (Stack* stk, int delta);

/// @brief Dumps stack to logs_out
/// @param stk ptr to stack
/// @param StackName name of stack
/// @param fileName name of file function was called at
/// @param funcName name of func function was called at
/// @param line line function was called at
void StackDumpInside (Stack* stk, const char* StackName, const char* fileName, const char* funcName, size_t line);

/// @brief Prints errors drom a stack variable
/// @param stk ptr to stack
/// @return error code stk->errCode
unsigned long long StackErrCheck (Stack* stk);

/// @brief Prints errors for a StackDump func
/// @param stk ptr to stack
void StackLogPrintErrors (Stack* stk);

/// @brief Counts hash for stack
/// @param stk ptr to stack
void StackCountHash (Stack* stk);

/// @brief Verifies hash for stack (tells if it was changed without any Stack specified functions)
/// @param stk ptr to stack
void StackVerifyHash (Stack* stk);

/// @brief Sets var to a poisoned state
/// @param var ptr to var
/// @param sizeofVar sizeof var
void setPoisonInside (void* var, size_t sizeofVar);

/// @brief Checks for a variable to be poisoned
/// @param var ptr to var
/// @param sizeofVar size of var
/// @return 1 if poisoned, 0 if not
bool isPoisonInside (void* var, size_t sizeofVar);
