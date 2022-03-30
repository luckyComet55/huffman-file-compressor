#include "btree.h"
#include <stdio.h>
#include "encoder.h"

#ifndef HUFFMAN_FILE_COMPRESSOR_DECODER_H
#define HUFFMAN_FILE_COMPRESSOR_DECODER_H

void _add_decoded_(NODE ** head, char symb, char code[MAX_CODE_LEN]);
NODE * read_table(NODE ** head, FILE * fr);
void print_dec_list(const NODE * head);
void decode_text(FILE *fr, char * buf);
void decrypt_file(const NODE * head, char * buf, FILE * fr, FILE * fw);

#endif
