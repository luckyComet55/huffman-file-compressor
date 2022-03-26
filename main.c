#include "encoder.h"
#include "btree.h"
#include <stdio.h>
#define ALPH_SIZE 256

int main() {
    NODE * head1 = NULL;
    int freq[ALPH_SIZE] = { 0 };
    FILE * fr = fopen("input", "rb");
    if(!fr) {
        return -5;
    }
    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    fseek(fr, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char) fgetc(fr)]++;
    }
    for (int i = 0; i < ALPH_SIZE; ++i) {
        if(freq[i] != 0) {
            add2list(&head1, freq[i], i);
        }
    }
    printf("Current list of symbols:\n");
    print_list(head1);
    list2tree(&head1);
    printf("Made into tree:\n");
    print_tree(head1);
    printf("\nEncoded symbols:\n");
    NODE * list_enc = NULL;
    char code[MAX_CODE_LEN] = { 0 };
    _encode_(head1, &list_enc, code);
    print_encoded(list_enc);
    head1 = delete_tree(head1);
    list_enc = delete_list(list_enc);
    fclose(fr);
    return 0;
}