#ifndef LIBFART_TOKEN
#define LIBFART_TOKEN

#include <stdlib.h>

/**
 * this enum is for storing brainfuck tokens (+, - etc...) kind.
 */
typedef enum
{
    FART_TOKEN_EOF,
    FART_TOKEN_PLUS,
    FART_TOKEN_MINUS,
    FART_TOKEN_NEXT,
    FART_TOKEN_BACK,
    FART_TOKEN_OUTPUT,
    FART_TOKEN_INPUT
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
