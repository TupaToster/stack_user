#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wformat-extra-args"

#define FLOG_DEBUG_TIME 1

#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <time.h>
#include <string.h>

void flogIntern (const void* val, const char* varType, const char* varName, size_t varSize, const char* fileName, const char* funcName, size_t line);

FILE* logOutf = NULL;

#define flogprintf(args)                                                                          \
            if (logOutf == NULL){                                                                 \
                logOutf = fopen ("logs_out", "a");                                                \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------\n"                    \
                "Logging session at compiled time : %s %s \n\n", __TIME__, __DATE__);             \
            }                                                                                     \
            fprintf (logOutf, args);


#define flog(a)                                                                                   \
            if (logOutf == NULL){                                                                 \
                logOutf = fopen ("logs_out", "a");                                                \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------\n"                    \
                "Logging session at compiled time : %s %s \n\n", __TIME__, __DATE__);             \
            }                                                                                     \
            flogIntern (&a, typeid (a).name (), #a, sizeof (a), __FILE__, __FUNCTION__, __LINE__)

#ifdef NO_LOG
#define flog(a) ;
#define flogprintf(args) ;
#endif

// i - int
// x - long long
// c - char
// f - float
// d - double
// e - long double
// Pc - char*
// P + varType = varType*
//unsigned types
// j - int
// y - long long
// h - char

#define flogFileInit if (logOutf == NULL){                                                        \
                logOutf = fopen ("logs_out", "a");                                                \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------\n"                    \
                "Logging session at compiled time : %s %s \n\n", __TIME__, __DATE__);             \
            } 

void flogIntern (const void* val, const char* varType, const char* varName, size_t varSize, const char* fileName, const char* funcName, size_t line) {

    fprintf (logOutf, "In file %s, function %s, line %u : %s = ", fileName, funcName, line, varName);


    if (!strcmp (varType, "Pc") or !strcmp (varType, "PKc")) fprintf (logOutf, "<%s>", *((const char**) val));
    else if (varType[0] == 'P') fprintf (logOutf, "%p", val);

    else if (!strcmp (varType, "i") or !strcmp (varType, "Ki")) fprintf (logOutf, "%d"   , *( ( const int*)                val));
    else if (!strcmp (varType, "x") or !strcmp (varType, "Kx")) fprintf (logOutf, "%lld" , *( ( const long long*)          val));
    else if (!strcmp (varType, "c") or !strcmp (varType, "Kc")) fprintf (logOutf, "%hhd" , *( ( const char*)               val));
    else if (!strcmp (varType, "f") or !strcmp (varType, "Kf")) fprintf (logOutf, "%f"   , *( ( const float*)              val));
    else if (!strcmp (varType, "d") or !strcmp (varType, "Kd")) fprintf (logOutf, "%Lf"  , *( ( const double*)             val));
    else if (!strcmp (varType, "j") or !strcmp (varType, "Kj")) fprintf (logOutf, "%u"   , *( ( const unsigned int*)       val));
    else if (!strcmp (varType, "y") or !strcmp (varType, "Ky")) fprintf (logOutf, "%llu" , *( ( const unsigned long long*) val));
    else if (!strcmp (varType, "h") or !strcmp (varType, "Kh")) fprintf (logOutf, "%hhu" , *( ( const unsigned char*)      val));
    else {
        for (int i = 0; i < varSize; i++) fprintf (logOutf, "%hhX", *((char*) val + i));
        fprintf (logOutf, " :: UNDEFINED_FORMAT : sizeof() = %u", varSize);
    }
    fprintf (logOutf, ";\n");
}