#include "../libfart/assembler.c"
#include <stdio.h>

int main(void)
{
    fart_lexer *lexer = fart_lexer_init(",,,+.+.+.");
    fart_assembler *assembler = fart_assembler_init(lexer);

    fart_assembler_run(assembler);

    for (size_t index = 0; index < assembler->binary_size; index++)
    {
        printf("[%lld]: 0x%X\n", index, assembler->binary[index]);
    }

    FILE *test = fopen("TEST.COM", "wb");
    fwrite(assembler->binary, 1, assembler->binary_size, test);
    fclose(test);

    printf("size: %lld\n", assembler->binary_size);
}
