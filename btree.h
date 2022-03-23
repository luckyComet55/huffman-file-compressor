#ifndef BTREE_H
#define BTREE_H

typedef struct node {
    unsigned val;
    char symb;
    char isSymb;
    struct node * left, * right;
    struct node* next;
} NODE;

void add2list(NODE ** head, unsigned value, char ascii);
NODE * add2tree(NODE * root, unsigned value);
void print_tree(NODE * root);
void delete_tree(NODE * root);
void print_list(const NODE * head);
NODE * delete_list(NODE * head);

#endif //BTREE_H