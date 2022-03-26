#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

void add2list(NODE ** head, unsigned value, char ascii) {
    while (*head) {
        if ((*head)->val > value) {
            break;
        }
        head = &((*head)->next);
    }
    NODE* newEl = (NODE*)malloc(sizeof(NODE));
    newEl->val = value;
    newEl->next = *head;
    newEl->symb = ascii;
    newEl->isSymb = 1;
    newEl->left = NULL;
    newEl->right = NULL;
    *head = newEl;
}

void combine_lists(NODE ** head1, NODE ** head2) {
    NODE * temp;
    while (*head2) {
        while (*head1) {
            if ((*head1)->val > (*head2)->val) {
                break;
            }
            head1 = &((*head1)->next);
        }
        temp = (*head2)->next;
        (*head2)->next = *head1;
        *head1 = *head2;
        *head2 = temp;
    }
}

void list2tree(NODE ** head) {
    NODE * root = NULL;
    while((*head) && (*head)->next) {
        root = (NODE*) malloc(sizeof(NODE));
        root->next = NULL;
        root->isSymb = 0;
        root->val = (*head)->val + (*head)->next->val;
        root->left = *head;
        root->right = (*head)->next;
        *head = (*head)->next->next;
        root->left->next = NULL;
        root->right->next = NULL;
        combine_lists(head, &root);
    }
}

void print_list(const NODE * head) {
    while (head)
    {
        printf("Symbol %d on ASCII %d times in text\n", head->symb, head->val);
        head = head->next;
    }
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
    if(root->isSymb) {
        printf("%5d", root->val);
    }
    print_tree(root->right);
}

NODE * delete_tree(NODE * root) {
    if(root) {
        delete_tree(root->left);
        delete_tree(root->right);
        free(root);
    }
    return NULL;
}

NODE * delete_list(NODE * head) {
    if(head) {
        head->next = delete_list(head->next);
        free(head);
    }
    return NULL;
}
