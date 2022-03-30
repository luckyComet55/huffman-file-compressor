#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "encoder.h"
#include "decoder.h"

void _add_decoded_(NODE ** head, char symb, char code[MAX_CODE_LEN]) {
    NODE * new_node = (NODE*) malloc(sizeof (NODE));
    new_node->symb = symb;
    strcpy(new_node->code, code);
    new_node->isSymb = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = *head;
    *head = new_node;
}

NODE *read_table(NODE ** head, FILE *fr) {
    fseek(fr, 0L, SEEK_SET);
    char buf[MAX_CODE_LEN + 3] = { 0 };
    fgets(buf, MAX_CODE_LEN + 3, fr);
    if (feof(fr) || !buf) {
        printf("File is empty\n");
        exit(-8);
    }
    if (strcmp(buf, "SOT\n") != 0){
        printf("No table for decoding\n");
        exit(-9);
    }
    fgets(buf, MAX_CODE_LEN + 3, fr);
    char symb;
    char * code;
    while (strcmp(buf, "EOT\n") != 0){
        if (feof(fr) || !buf) {
            printf("File is empty\n");
            exit(-10);
        }
        symb = buf[0];
        code = strtok(buf + 1, " \n");
        _add_decoded_(head, symb, code);
        fgets(buf, MAX_CODE_LEN + 3, fr);
    }
}

void print_dec_list(const NODE *head) {
    while(head) {
        printf("Char %c encoded as %s\n", head->symb, head->code);
        head = head->next;
    }
}

void decode_text(FILE *fr, char * buf) {
    char temp = fgetc(fr);
    BIT2CHAR symb;
    char byte[BIT8-1] = { 0 };
    int count = 0;
    while (!feof(fr)) {
        symb.symb = temp;
        byte[0] = symb.mbit.b1 + '0';
        byte[1] = symb.mbit.b2 + '0';
        byte[2] = symb.mbit.b3 + '0';
        byte[3] = symb.mbit.b4 + '0';
        byte[4] = symb.mbit.b5 + '0';
        byte[5] = symb.mbit.b6 + '0';
        byte[6] = symb.mbit.b7 + '0';
        byte[7] = symb.mbit.b8 + '0';
        strcpy(buf + count, byte);
        count += BIT8;
        temp = fgetc(fr);
    }
}

void decrypt_file(const NODE *head, char * buf, FILE *fr, FILE *fw) {
    fseek(fw, 0L, SEEK_SET);
    fgets(buf, 10, fr);
    int tail = (int)buf[5] - '0';
    decode_text(fr, buf);
    int buflen = strlen(buf) - 8 + tail;
    buf[buflen] = '\0';
    printf("Encoded text:\n%s\n", buf);
}
