#include "./include/file.h"
#include <stdio.h>

char *read_file(char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *content = calloc(size + 1, sizeof(char));
    if (content == NULL)
    {
        fclose(file);
        return NULL;
    }

    fread(content, size, 1, file);
    fclose(file);

    content[size] = 0;

    return content;
}

char write_file(char *path, unsigned char *bytes, size_t size)
{
    FILE *file = fopen(path, "wb");

    if (file == NULL)
        return 0;

    fwrite(bytes, 1, size, file);
    fclose(file);

    return 1;
}
