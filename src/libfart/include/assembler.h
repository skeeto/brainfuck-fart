#ifndef LIBFART_ASSEMBLER
#define LIBFART_ASSEMBLER

#include "lexer.h"

// this binary data contains cells, reset label and overload label as binary.
extern unsigned char binary_data[519];

/**
 * struct for assembler data.
 */
typedef struct
{
    fart_token *tokens;    // for storing tokens.
    unsigned char *binary; // generated binary.
    size_t binary_size;    // calculated total binary size.
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

#endif /* LIBFART_ASSEMBLER */
