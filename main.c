#include "btree.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    srand(time(0));
    NODE * tree = NULL;
    int iterations;
    scanf("%d", &iterations);
    for (int i = 0; i < iterations; ++i) {
        tree = add_elem(tree, (char)(rand() % 128));
    }
    print_tree(tree);
    delete_tree(tree);
    return 0;
}