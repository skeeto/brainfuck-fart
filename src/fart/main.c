#include "../libfart/assemb~1.c"
#include "file.c"

int main(int argc, char **argv)
{
    fart_lexer *lexer;
    char *content, *output;
    fart_assembler *assembler;

    if (argc != 3)
    {
        puts("usage: fart input.bf output.com");
        return -1;
    }

    content = read_file(argv[1]);
    output = argv[2];

    if (content == NULL)
    {
        puts("failed to read file.");
        return -1;
    }

    lexer = fart_lexer_init(content);
    if (lexer == NULL)
        return -1;

    assembler = fart_assembler_init(lexer);
    if (assembler == NULL)
        return -1;

    fart_assembler_run(assembler);

    write_file(output, assembler->binary, assembler->lexer->binary_size);
    printf("passed 3, wrote %lld byte.\n", assembler->lexer->binary_size);

    return 0;
}
