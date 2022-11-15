#ifndef LIBFART_CHECKER
#define LIBFART_CHECKER

#include "lexer.h"

/**
 * matches brackets, checks bracket errors.
 *
 * @param lexer lexer to process.
 * @param tokens tokens to process.
 * @returns negative one (-1) or bracket error position.
 */
int run_checker(fart_lexer *lexer, fart_token *tokens);

#endif /* LIBFART_CHECKER */
