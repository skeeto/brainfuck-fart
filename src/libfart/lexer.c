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
        return fart_lexer_collect_optimized(lexer, '+', FART_TOKEN_PLUS);
    case '-':
        return fart_lexer_collect_optimized(lexer, '-', FART_TOKEN_MINUS);
    default:
        return fart_lexer_next(lexer);
    }
}

static fart_token fart_lexer_collect_optimized(fart_lexer *lexer, char value, fart_token_kind kind)
{
    fart_lexer_advance(lexer);
    char current = fart_lexer_current(lexer);

    size_t count = 1;

    while (current == value)
    {
        count++;

        fart_lexer_advance(lexer);
        current = fart_lexer_current(lexer);

        if (count == 2)
        {
            break;
        }
    }

    return (fart_token){.kind = kind, .repeat = count};
}
