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

void print_encoded(const NODE * head) {
    while(head) {
        printf("Char %c encoded as %s\n", head->symb, head->code);
        head = head->next;
    }
}
