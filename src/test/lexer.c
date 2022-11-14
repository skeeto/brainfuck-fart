#include "../libfart/lexer.c"
#include <stdio.h>

int main(void)
{
    fart_lexer *lexer = fart_lexer_init("<+++.>.>.");

    for (;;)
    {
        fart_token token = fart_lexer_next(lexer);
        printf("%d %lld\n", token.kind, token.value);

        if (token.kind == FART_TOKEN_EOF)
        {
            break;
        }
    }

    printf("size: %lld\n", lexer->binary_size);
}
