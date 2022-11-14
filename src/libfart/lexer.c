#include "include/lexer.h"
#include <string.h>

fart_lexer *fart_lexer_init(char *source)
{
    fart_lexer *lexer = malloc(sizeof(fart_lexer));

    if (lexer == NULL)
        return NULL;

    lexer->source = source;
    lexer->source_length = strlen(source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;

    // xor bx (2 byte) + offset (256 byte) + reset jump (3 byte) + overload jump (4 byte)
    lexer->binary_size = 2 + 256 + 3 + 4;

    return lexer;
}

void fart_lexer_advance(fart_lexer *lexer)
{
    lexer->column++;
    lexer->position++;
}

void fart_lexer_skip_whitespace(fart_lexer *lexer)
{
    char current = fart_lexer_current(lexer);

    while (IS_WHITESPACE(current))
    {
        if (current == '\n')
        {
            lexer->line++;
            lexer->column = 1;
        }

        fart_lexer_advance(lexer);
        current = fart_lexer_current(lexer);
    }
}

char fart_lexer_current(fart_lexer *lexer)
{
    if (lexer->position >= lexer->source_length)
        return '\0';

    return lexer->source[lexer->position];
}

fart_token fart_lexer_next(fart_lexer *lexer)
{
    fart_lexer_skip_whitespace(lexer);
    switch (fart_lexer_current(lexer))
    {
    case '\0':
        return (fart_token){.kind = FART_TOKEN_EOF};
    case '+':
        lexer->binary_size += 5;
        return fart_lexer_collect_optimized(lexer, '+', FART_TOKEN_PLUS);
    case '-':
        lexer->binary_size += 5;
        return fart_lexer_collect_optimized(lexer, '-', FART_TOKEN_MINUS);
    default:
        return fart_lexer_next(lexer);
    }
}

fart_token fart_lexer_collect_optimized(fart_lexer *lexer, char value, fart_token_kind kind)
{
    fart_lexer_advance(lexer);
    char current = fart_lexer_current(lexer);

    size_t count = 1;

    while (current == value)
    {
        count++;

        fart_lexer_advance(lexer);
        current = fart_lexer_current(lexer);

        if (count == 254)
        {
            break;
        }
    }

    return (fart_token){.kind = kind, .value = count};
}

fart_token *fart_lexer_run(fart_lexer *lexer)
{
    size_t capacity = sizeof(fart_token), length = 0;
    fart_token *tokens = malloc(capacity);

    for (;;)
    {
        if ((capacity - (sizeof(fart_token) * length)) < sizeof(fart_token))
        {
            capacity *= 2;
            tokens = realloc(tokens, capacity);
        }

        fart_token token = fart_lexer_next(lexer);

        tokens[length++] = token;
        if (token.kind == FART_TOKEN_EOF)
        {
            break;
        }
    }

    return tokens;
}
