#include "btree.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

unsigned freq[256];

int main() {

    FILE* fr = fopen("input.txt", "rb");
    if (!fr)
        return -1;
    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    printf("%ld\n",length);
    fseek(fr, 0, SEEK_SET);
    for (int i = 0; i < length; ++i)
    {
        freq[(unsigned char)fgetc(fr)] ++;
    }

    for (int j=0; j<256;++j){
        if (freq[j]!=0)
            printf("%u,%c\n",freq[j],j);
    }

    NODE* phead = NULL;

    for (int i = 0; i < 256; ++i)
    {
        if (freq[i]!=0)
            Add2List(&phead, freq[i],i);
    }

    NODE * tree = NULL;

    for (int i = 0; i < 256; ++i) {
        if (freq[i]!=0)
            tree = add_elem(tree, freq[i]);
    }
    print_tree(tree);
    PrintList(phead);
    delete_tree(tree);

    fclose(fr);
    return 0;
}