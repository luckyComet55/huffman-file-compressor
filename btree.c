#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

NODE * add_elem(NODE * root, int value, char asci) {
    if(root != NULL) {
        if(value < root->val) {
            root->left = add_elem(root->left, value, asci);
        } else {
            root->right = add_elem(root->right, value, asci);
        }
    } else {
        root = (NODE *) malloc(sizeof(NODE));
        root->val = value;
        root->symb=asci;
        root->left = NULL;
        root->right = NULL;
    }
    return root;
}

void print_tree(NODE * root) {
    if(root == NULL) {
        return;
    }
    print_tree(root->left);
    printf("%5c", root->symb);
    print_tree(root->right);
}

void delete_tree(NODE * root) {
    if(root == NULL) {
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    free(root);
}
