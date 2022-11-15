#ifndef LIBFART_ASSEMBLER
#define LIBFART_ASSEMBLER

#include "lexer.h"

/**
 * struct for assembler data.
 */
typedef struct
{
    fart_token *tokens;    // for storing tokens.
    fart_lexer *lexer;     // for storing lexer.
    unsigned char *binary; // generated binary.
} fart_assembler;

/**
 * initialize assembler.
 *
 * @param source lexer to assemble.
 * @returns initialized assembler.
 */
fart_assembler *fart_assembler_init(fart_lexer *lexer);

/**
 * execute assembler.
 *
 * @param assembler assembler data to run.
 */
void fart_assembler_run(fart_assembler *assembler);

/**
 * deallocte assembler from memory.
 * it also dealloctes lexer, tokens and generated binary.
 *
 * @param assembler assembler to free.
 */
void fart_assembler_free(fart_assembler *assembler);

#endif /* LIBFART_ASSEMBLER */
