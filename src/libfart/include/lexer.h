#ifndef LIBFART_LEXER
#define LIBFART_LEXER

#include "token.h"

#define IS_USEFUL(c) (c == '+' || c == '-' || c == '>' || c == '<' || c == '[' || c == ']' || c == '.' || c == ',')
#define CALCULATE_CURRENT_POS(v) (v - 4100)

/**
 * struct for storing jumps.
 */
typedef struct
{
    size_t start; /* jump start position. */
    size_t end;   /* jump end position. */
} fart_jump_table;

/**
 * struct for lexer data.
 */
typedef struct
{
    fart_jump_table jump_table[2048]; /* storing jumps. */
    char *source;                     /* for storing given source. */
    size_t source_length;             /* storing given source length. */
    size_t position;                  /* current read position. */
    size_t binary_size;               /* calculated total binary size while lexing. */
    size_t jump_table_length;         /* jump table index for calculating. */
} fart_lexer;

/**
 * initialize lexer.
 *
 * @param source data to lex.
 * @returns pointer to lexer.
 */
fart_lexer *fart_lexer_init(char *source);

/**
 * increase lexer position.
 * @param lexer lexer to advance.
 */
void fart_lexer_advance(fart_lexer *lexer);

/**
 * increase lexer position until something useful.
 * @param lexer lexer to advance.
 */
void fart_lexer_skip_useless(fart_lexer *lexer);

/**
 * get current character from source or null terminator.
 * @param lexer lexer to read.
 * @returns current character.
 */
char fart_lexer_current(fart_lexer *lexer);

/**
 * get next token or eof.
 * @param lexer lexer to process.
 * @returns collected token.
 */
fart_token fart_lexer_next(fart_lexer *lexer);

/**
 * token collector and optimizer for fart.
 * @param lexer lexer to process.
 * @param value character to collect.
 * @param kind token kind to return.
 * @returns collected token.
 */
fart_token fart_lexer_collect_optimized(fart_lexer *lexer, char value, fart_token_kind kind);

/**
 * wrapper function for lexer.
 * @param lexer lexer to process.
 * @returns collected tokens.
 */
fart_token *fart_lexer_run(fart_lexer *lexer);

/**
 * deallocte lexer from memory.
 * @param lexer lexer to free.
 */
void fart_lexer_free(fart_lexer *lexer);

#endif /* LIBFART_LEXER */
