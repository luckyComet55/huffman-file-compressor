#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

void add2list(NODE** head, unsigned value, char ascii) {
    while (*head)
    {
        if ((*head)->val > value)
            break;
        head = &((*head)->next);
    }
    NODE* newEl = (NODE*)malloc(sizeof(NODE));
    newEl->val = value;
    newEl->next = *head;
    newEl->symb = ascii;
    *head = newEl;
}

void print_list(const NODE* head) {
    while (head)
    {
        printf("%5c", head->symb);
        head = head->next;
    }
    printf("\n");
}

NODE * add2tree(NODE * root, unsigned value) {
    if(root != NULL) {
        if(value < root->val) {
            root->left = add2tree(root->left, value);
        } else {
            root->right = add2tree(root->right, value);
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

NODE * delete_list(NODE * head) {
    if(head) {
        head->next = delete_list(head->next);
        free(head);
    }
    return NULL;
}
