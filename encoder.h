#include "btree.h"
#include <stdio.h>
#ifndef ENCODER_H
#define ENCODER_H

#define BIT8 8

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

void compress_file(FILE * fr, FILE * fw);

#endif //ENCODER_H
