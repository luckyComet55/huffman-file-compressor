#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "encoder.h"



void _add_encoded_(NODE ** head, char symb, char code[MAX_CODE_LEN]) {
    NODE * new_node = (NODE*) malloc(sizeof (NODE));
    new_node->symb = symb;
    strcpy(new_node->code, code);
    new_node->isSymb = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = *head;
    *head = new_node;
}

void _encode_(NODE * root, NODE ** encoded_, char code[MAX_CODE_LEN]) {
    if(!(root->left)) {
        _add_encoded_(encoded_, root->symb, code);
        return;
    }
    char temp[MAX_CODE_LEN] = { 0 };
    strcpy(temp, code);
    strcat(temp, "0");
    _encode_(root->left, encoded_, temp);
    temp[strlen(temp) - 1] = '1';
    _encode_(root->right, encoded_, temp);
}

void print_encode_table(const NODE * head, FILE * fw, const int tail) {
    fseek(fw, 0L, SEEK_SET);
    fprintf(fw, "SOT\n");
    while(head) {
        //printf("Char %c encoded as %s\n", head->symb, head->code);
        fprintf(fw, "%c %s\n", head->symb, head->code);
        head = head->next;
    }
    fprintf(fw, "EOT\nTail %d\n", tail);
}


void encode_text(NODE *head, FILE *fr, char * buf) {
    fseek(fr, 0L, SEEK_SET);
    char symbol = fgetc(fr);
    NODE * node;
    while (symbol != EOF) {
        node = find_node(head, symbol);
        if (!node) {
            printf("Encoding error\n");
            exit(-7);
        }
        strcpy(buf, node->code);
        buf += strlen(node->code);
        symbol = fgetc(fr);
    }
}

void compress_file(char *buf, const NODE *head, FILE *fr, FILE *fw) {
    fseek(fr, 0L, SEEK_SET);
    fseek(fw, 0L, SEEK_SET);
    encode_text(head, fr, buf);
    int count = strlen(buf) / BIT8;
    int tail = strlen(buf) % BIT8; //остаток из нулей
    int len = count + 1;           //длина результирующей строки
    if (tail == 0) len--;
    BIT2CHAR symb;
    print_encode_table(head, fw, tail);
    unsigned char * res = (char*)malloc((len) * sizeof(char));
    for (int i = 0; i < len; ++i)
    {
        symb.mbit.b1 = buf[i*BIT8 + 0];
        symb.mbit.b2 = buf[i*BIT8 + 1];
        symb.mbit.b3 = buf[i*BIT8 + 2];
        symb.mbit.b4 = buf[i*BIT8 + 3];
        symb.mbit.b5 = buf[i*BIT8 + 4];
        symb.mbit.b6 = buf[i*BIT8 + 5];
        symb.mbit.b7 = buf[i*BIT8 + 6];
        symb.mbit.b8 = buf[i*BIT8 + 7];
        res[i] = symb.symb;
        fputc(res[i], fw);
    }
}
