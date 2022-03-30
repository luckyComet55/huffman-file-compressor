#include "btree.h"
#include <stdio.h>
#ifndef ENCODER_H
#define ENCODER_H

typedef union bit2char {
    char symb;
    struct bit {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    }mbit;
}BIT2CHAR;


void _add_encoded_(NODE ** head, char symb, char code[MAX_CODE_LEN]);
void _encode_(NODE * root, NODE ** encoded_, char code[MAX_CODE_LEN]);
void print_encode_table(const NODE * head, FILE * fw, const int tail);
void encode_text(NODE * head, FILE * fr, char * buf);
void compress_file(char * buf, const NODE * head, FILE * fr, FILE * fw);

#endif //ENCODER_H
