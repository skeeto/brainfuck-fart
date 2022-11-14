#include "./include/assembler.h"
#include "lexer.c"

unsigned char binary_data[263] = {
    // 256 byte cell
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // xor bx, bx
    // ret
    0x31, 0xDB, 0xC3,
    // mov bx, 255
    // ret
    0xBB, 0xFF, 0x00, 0xC3};

fart_assembler *fart_assembler_init(fart_lexer *lexer)
{
    fart_token *tokens = fart_lexer_run(lexer);
    fart_assembler *assembler = malloc(sizeof(fart_assembler));

    assembler->binary_size = lexer->binary_size;
    assembler->binary = malloc(assembler->binary_size);
    assembler->tokens = tokens;

    // xor bx, bx
    assembler->binary[0] = 0x31;
    assembler->binary[1] = 0xDB;

    return assembler;
}

void fart_assembler_run(fart_assembler *assembler)
{
    size_t index = 0, byte_index = 2;
    unsigned char cell_offset[2] = {(assembler->binary_size - 7) & 0xFF, (assembler->binary_size - 7) >> 8};

    for (;;)
    {
        fart_token token = assembler->tokens[index];

        switch (token.kind)
        {
        case FART_TOKEN_PLUS: {
            // 80 87 <little endian absolute offset> <value>
            // add [offset + bx], <value>
            unsigned char opcodes[5] = {0x80, 0x87, 0x00, 0x00, 0x00};
            opcodes[2] = cell_offset[0];
            opcodes[3] = cell_offset[1];
            opcodes[4] = token.value;

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            break;
        }
        case FART_TOKEN_MINUS: {
            // 80 AF <little endian absolute offset> <value>
            // sub [offset + bx], <value>
            unsigned char opcodes[5] = {0x80, 0xAF, 0x00, 0x00, 0x00};
            opcodes[2] = cell_offset[0];
            opcodes[3] = cell_offset[1];
            opcodes[4] = token.value;

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            break;
        }
        default: {
            for (size_t opcode_index = 0; opcode_index < 263; opcode_index++)
            {
                assembler->binary[byte_index] = binary_data[opcode_index];
                byte_index += 1;
            }

            return;
        }
        }

        index++;
    }
}
