#include "include/checker.h"
#include <stdio.h>

int run_checker(fart_lexer *lexer, fart_token *tokens)
{
    size_t index = 0, temp_index = 0;
    fart_jump_table temp_jump_table[2048];

    for (;;)
    {
        fart_token *token = &tokens[index];
        if (token->kind == FART_TOKEN_EOF)
        {
            break;
        }
        else if (token->kind == FART_TOKEN_LOOP_OPEN)
        {
            temp_jump_table[temp_index].end = index;
            temp_jump_table[temp_index++].start = token->value;
        }
        else if (token->kind == FART_TOKEN_LOOP_CLOSE)
        {
            if (temp_index == 0)
                return token->value;

            size_t matched_token_index = temp_jump_table[--temp_index].end;

            temp_jump_table[temp_index].end = token->value;

            token->value = lexer->jump_table_length;
            tokens[matched_token_index].value = lexer->jump_table_length;

            lexer->jump_table[lexer->jump_table_length++] = temp_jump_table[temp_index];
        }

        index++;
    }

    return -1;
}
