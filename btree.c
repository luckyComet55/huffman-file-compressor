#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
void Add2List(NODE** pphead, int value, char ascii)
{
    while (*pphead)
    {
        if ((*pphead)->val > value)
            break;
        pphead = &((*pphead)->next);
    }
    NODE* pnew = (NODE*)malloc(sizeof(NODE));
    pnew->val = value;
    pnew->next = *pphead;
    pnew->symb = ascii;
    *pphead = pnew;
}
void PrintList(const NODE* phead)
{
    while (phead)
    {
        printf("%5c", phead->symb);
        phead = phead->next;
    }
    printf("\n");
}

NODE * add_elem(NODE * root, int value) {
    if(root != NULL) {
        if(value < root->val) {
            root->left = add_elem(root->left, value);
        } else {
            root->right = add_elem(root->right, value);
        }
    } else {
        root = (NODE *) malloc(sizeof(NODE));
        root->val = value;
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
    printf("%5d", root->val);
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
