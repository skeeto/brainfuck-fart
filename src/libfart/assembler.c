#include "./include/assembler.h"
#include "lexer.c"
#include <stdio.h>

fart_assembler *fart_assembler_init(fart_lexer *lexer)
{
    fart_token *tokens = fart_lexer_run(lexer);
    fart_assembler *assembler = malloc(sizeof(fart_assembler));

    assembler->binary_size = lexer->binary_size;
    assembler->binary = calloc(assembler->binary_size, 1);
    assembler->tokens = tokens;

    // xor bx, bx
    assembler->binary[0] = 0x31;
    assembler->binary[1] = 0xDB;

    return assembler;
}

void fart_assembler_run(fart_assembler *assembler)
{
    size_t index = 0, byte_index = 2;

    size_t cell_start = assembler->binary_size - 256;
    unsigned char cell_offset[2] = {cell_start & 0xFF, cell_start >> 8};

    for (;;)
    {
        fart_token token = assembler->tokens[index];

        switch (token.kind)
        {
        case FART_TOKEN_PLUS: {
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
        case FART_TOKEN_NEXT: {
            //    inc bx
            //    cmp bx, 512
            //    jne L
            //    xor bx, bx
            // L:
            unsigned char opcodes[9] = {0x43, 0x81, 0xFB, 0x00, 0x02, 0x75, 0x02, 0x31, 0xDB};

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            assembler->binary[byte_index++] = opcodes[5];
            assembler->binary[byte_index++] = opcodes[6];
            assembler->binary[byte_index++] = opcodes[7];
            assembler->binary[byte_index++] = opcodes[8];
            break;
        }
        case FART_TOKEN_BACK: {
            //    cmp bx, 0
            //    jne L
            //    mov bx, 512
            // L: dec bx
            unsigned char opcodes[9] = {0x83, 0xFB, 0x00, 0x75, 0x03, 0xBB, 0x00, 0x02, 0x4B};

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            assembler->binary[byte_index++] = opcodes[5];
            assembler->binary[byte_index++] = opcodes[6];
            assembler->binary[byte_index++] = opcodes[7];
            assembler->binary[byte_index++] = opcodes[8];
            break;
        }
        case FART_TOKEN_OUTPUT: {
            // mov dl, [off + bx]
            // mov ah, 2
            // int 21h
            unsigned char opcodes[8] = {0x8A, 0x97, 0x00, 0x00, 0xB4, 0x02, 0xCD, 0x21};

            opcodes[2] = cell_offset[0];
            opcodes[3] = cell_offset[1];

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            assembler->binary[byte_index++] = opcodes[5];
            assembler->binary[byte_index++] = opcodes[6];
            assembler->binary[byte_index++] = opcodes[7];
            break;
        }
        case FART_TOKEN_INPUT: {
            // mov ah, 7
            // int 21h
            // mov byte [off + bx], al
            unsigned char opcodes[8] = {0xB4, 0x07, 0xCD, 0x21, 0x88, 0x87, 0x00, 0x00};

            opcodes[6] = cell_offset[0];
            opcodes[7] = cell_offset[1];

            assembler->binary[byte_index++] = opcodes[0];
            assembler->binary[byte_index++] = opcodes[1];
            assembler->binary[byte_index++] = opcodes[2];
            assembler->binary[byte_index++] = opcodes[3];
            assembler->binary[byte_index++] = opcodes[4];
            assembler->binary[byte_index++] = opcodes[5];
            assembler->binary[byte_index++] = opcodes[6];
            assembler->binary[byte_index++] = opcodes[7];
            break;
        }
        default: {
            // mov ah, 4ch
            // int 21h
            assembler->binary[byte_index++] = 0xB4;
            assembler->binary[byte_index++] = 0x4C;
            assembler->binary[byte_index++] = 0xCD;
            assembler->binary[byte_index++] = 0x21;

            // cells
            for (size_t opcode_index = 0; opcode_index < 512; opcode_index++)
                assembler->binary[byte_index++] = 0x00;

            return;
        }
        }

        index++;
    }
}
