#ifndef BTREE_H
#define BTREE_H
#define MAX_CODE_LEN 128

typedef struct node {
    unsigned val;
    char symb;
    char isSymb;
    char code[MAX_CODE_LEN];
    struct node * left, * right;
    struct node * next;
} NODE;

void add2list(NODE ** head, unsigned value, char ascii);
void combine_lists(NODE ** head1, NODE ** head2);
void list2tree(NODE ** head);
NODE * add2tree(NODE * root, unsigned value);
void print_tree(NODE * root);
NODE * delete_tree(NODE * root);
void print_list(const NODE * head);
NODE * delete_list(NODE * head);

#endif //BTREE_H