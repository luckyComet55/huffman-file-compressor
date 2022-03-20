#ifndef BTREE_H
#define BTREE_H

typedef struct node {
    int val;
    char symb;
    char isSymb;
    struct node * left, * right;
} NODE;

NODE * add_elem(NODE * root, int value);
void print_tree(NODE * root);
void delete_tree(NODE * root);

#endif //BTREE_H