#ifndef BTREE_H
#define BTREE_H
#define MAX_CODE_LEN 256

typedef struct node {
    unsigned long long val;
    unsigned char symb;
    char isSymb;
    char code[MAX_CODE_LEN];
    struct node * left, * right;
    struct node * next;
} NODE;

void add2list(NODE ** head, unsigned long long value, unsigned char ascii);
void combine_lists(NODE ** head1, NODE ** head2);
void list2tree(NODE ** head);
NODE * add2tree(NODE * root, unsigned long long value);
void print_tree(NODE * root);
NODE * delete_tree(NODE * root);
void print_list(const NODE * head);
NODE * delete_list(NODE * head);
NODE * find_node(NODE * head, unsigned char symbol);

#endif //BTREE_H