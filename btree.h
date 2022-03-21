#ifndef BTREE_H
#define BTREE_H

typedef struct node {
    int val;
    char symb;
    char isSymb;
    struct node * left, * right;
    struct Node* next;
} NODE;

void Add2List(NODE** pphead, int value, char ascii);
NODE * add_elem(NODE * root, int value);
void print_tree(NODE * root);
void delete_tree(NODE * root);
void PrintList(const NODE* phead);

#endif //BTREE_H