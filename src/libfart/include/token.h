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
} fart_token_kind;

/**
 * this struct is for storing brainfuck tokens with some extra data.
 */
typedef struct
{
    fart_token_kind kind; // for storing collected token kind (type).
    size_t repeat;        // reserved for plus and minus for optimization.
} fart_token;

#endif /* LIBFART_TOKEN */
