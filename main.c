#include "btree.h"
#include <stdio.h>
//#include <time.h>
//#include <stdlib.h>

int main() {
    unsigned freq[256] = { 0 };

    FILE* fr = fopen("input", "rb");
    if (!fr) {
        return -1;
    }
    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    printf("File length is %ld symbols\n",length);
    fseek(fr, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char)fgetc(fr)]++;
    }

    for (int j=0; j<256;++j) {
        if (freq[j]!=0) {
            printf("%u,%c\n", freq[j], j);
        }
    }

    NODE * head = NULL;

    for (int i = 0; i < 256; ++i) {
        if (freq[i]!=0) {
            add2list(&head, freq[i], (char) i);
        }
    }

    print_list(head);
    head = delete_list(head);

    fclose(fr);
    return 0;
}