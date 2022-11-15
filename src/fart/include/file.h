#ifndef FART_FILE
#define FART_FILE

#include <stdlib.h>

/**
 * read given file.
 *
 * @param path file to read.
 * @returns null-terminated string or null.
 */
char *read_file(char *path);

/**
 * write bytes to given file.
 *
 * @param path file to write.
 * @param bytes bytes to write into file.
 * @param size byte size.
 * @returns boolean value.
 */
char write_file(char *path, unsigned char *bytes, size_t size);

#endif /* FART_FILE */
