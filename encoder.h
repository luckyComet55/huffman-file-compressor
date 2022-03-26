#include "btree.h"
#ifndef ENCODER_H
#define ENCODER_H

void _add_encoded_(NODE ** head, char symb, char code[MAX_CODE_LEN]);
void _encode_(NODE * root, NODE ** encoded_, char code[MAX_CODE_LEN]);
void print_encoded(const NODE * head);

#endif //ENCODER_H
