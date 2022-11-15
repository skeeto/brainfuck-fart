#include "include/lexer.h"
#include <stdio.h>
#include <string.h>

fart_lexer *fart_lexer_init(char *source)
{
    fart_lexer *lexer = malloc(sizeof(fart_lexer));

    if (lexer == NULL)
    {
        puts("alloc failed for lexer.");
        return NULL;
    }

    lexer->source = source;
    lexer->source_length = strlen(source);
    lexer->position = 0;
    lexer->jump_table_length = 0;

    /* xor bx (2 byte) + offset (4096 byte) + exit interrupt (4 byte) */
    lexer->binary_size = 2 + 4096 + 4;

    return lexer;
}

void fart_lexer_advance(fart_lexer *lexer)
{
    lexer->position++;
}

void fart_lexer_skip_useless(fart_lexer *lexer)
{
    char current = fart_lexer_current(lexer);

    while (!IS_USEFUL(current))
    {
        if (current == '\0')
        {
            break;
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
    fart_token token = {0, 0};
    fart_lexer_skip_useless(lexer);
    switch (fart_lexer_current(lexer))
    {
    case '\0':
        token.kind = FART_TOKEN_EOF;
        return token;
    case '+':
        lexer->binary_size += 5;
        return fart_lexer_collect_optimized(lexer, '+', FART_TOKEN_PLUS);
    case '-':
        lexer->binary_size += 5;
        return fart_lexer_collect_optimized(lexer, '-', FART_TOKEN_MINUS);
    case '>':
        fart_lexer_advance(lexer);
        lexer->binary_size += 9;
        token.kind = FART_TOKEN_NEXT;
        return token;
    case '<':
        fart_lexer_advance(lexer);
        lexer->binary_size += 9;
        token.kind = FART_TOKEN_BACK;
        return token;
    case '.':
        fart_lexer_advance(lexer);
        lexer->binary_size += 8;
        token.kind = FART_TOKEN_OUTPUT;
        return token;
    case ',':
        fart_lexer_advance(lexer);
        lexer->binary_size += 8;
        token.kind = FART_TOKEN_INPUT;
        return token;
    case '[': {
        fart_lexer_advance(lexer);
        lexer->binary_size += 9;
        token.kind = FART_TOKEN_LOOP_OPEN;
        token.value = CALCULATE_CURRENT_POS(lexer->binary_size);
        return token;
    }
    case ']': {
        fart_lexer_advance(lexer);
        lexer->binary_size += 3;
        token.kind = FART_TOKEN_LOOP_CLOSE;
        token.value = CALCULATE_CURRENT_POS(lexer->binary_size);
        return token;
    }
    default:
        return fart_lexer_next(lexer);
    }
}

fart_token fart_lexer_collect_optimized(fart_lexer *lexer, char value, fart_token_kind kind)
{
    char current;
    fart_token token;
    size_t count = 1;

    fart_lexer_advance(lexer);
    current = fart_lexer_current(lexer);

    while (current == value)
    {
        count++;

        fart_lexer_advance(lexer);
        current = fart_lexer_current(lexer);

        if (count == 250)
        {
            break;
        }
    }

    token.kind = kind;
    token.value = count;
    return token;
}

fart_token *fart_lexer_run(fart_lexer *lexer)
{
    size_t capacity = sizeof(fart_token), length = 0;
    fart_token *tokens = malloc(capacity);

    if (tokens == NULL)
    {
        puts("alloc failed for tokens.");
        return NULL;
    }

    for (;;)
    {
        fart_token token;

        if ((capacity - (sizeof(fart_token) * length)) < sizeof(fart_token))
        {
            capacity *= 2;
            tokens = realloc(tokens, capacity);
        }

        token = fart_lexer_next(lexer);

        tokens[length++] = token;
        if (token.kind == FART_TOKEN_EOF)
        {
            break;
        }
    }

    return tokens;
}

void fart_lexer_free(fart_lexer *lexer)
{
    free(lexer);
}
