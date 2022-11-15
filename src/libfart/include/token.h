#ifndef LIBFART_TOKEN
#define LIBFART_TOKEN

#include <stdlib.h>

/**
 * this enum is for storing brainfuck tokens (+, - etc...) kind.
 */
typedef enum
{
    FART_TOKEN_EOF,       // file end.
    FART_TOKEN_PLUS,      // increase cell (+).
    FART_TOKEN_MINUS,     // decrease cell (-).
    FART_TOKEN_NEXT,      // moving to next cell (>).
    FART_TOKEN_BACK,      // moving back (<).
    FART_TOKEN_OUTPUT,    // print current cell as ascii (.).
    FART_TOKEN_INPUT,     // get one byte input and put it in current cell (,).
    FART_TOKEN_LOOP_OPEN, // create new loop ([).
    FART_TOKEN_LOOP_CLOSE // close loop (]).
} fart_token_kind;

/**
 * this struct is for storing brainfuck tokens with some extra data.
 */
typedef struct
{
    fart_token_kind kind; // for storing collected token kind (type).
    size_t value;         // reserved for extra value.
} fart_token;

#endif /* LIBFART_TOKEN */
