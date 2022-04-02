#include "encoder.h"
#include "btree.h"
#include "decoder.h"
#include <stdio.h>
//<<<<<<< HEAD
#include <time.h>
#include <stdlib.h>

#define ALPH_SIZE 256
unsigned freq[ALPH_SIZE];

int main() {
    NODE * head1 = NULL;
    FILE * fr = fopen("C:\\Users\\kurik\\CLionProjects\\huffman-file-compressor\\input", "rb");
    if(!fr) {
        return -5;
    }
    FILE * fw = fopen("C:\\Users\\kurik\\CLionProjects\\huffman-file-compressor\\output", "wb");
    if(!fw) {
        return -6;
    }

    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    if(length == 0) {
        printf("File is empty\n");
        exit(-8);
    }
    printf("%lu", length);
    fseek(fr, 0, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        freq[(unsigned char) fgetc(fr)]++;
    }
    for (int i = 0; i < ALPH_SIZE; ++i) {
        if(freq[i] != 0) {
            add2list(&head1, freq[i], i);
        }
    }
    list2tree(&head1);
    NODE * list_enc = NULL;
    unsigned char code[MAX_CODE_LEN] = { 0 };
    _encode_(head1, &list_enc, code);
    unsigned char * buf = (unsigned char*)malloc((length) * sizeof(unsigned char) * MAX_CODE_LEN);
    compress_file(buf, list_enc, fr, fw);
    head1 = delete_tree(head1);
    list_enc = delete_list(list_enc);
    free(buf);
    fclose(fr);
    fclose(fw);
//========================================================
    FILE * fr1 = fopen("C:\\Users\\kurik\\CLionProjects\\huffman-file-compressor\\output", "rb");
    if(!fr1) {
        return -15;
    }
    FILE * fw1 = fopen("C:\\Users\\kurik\\CLionProjects\\huffman-file-compressor\\output2", "wb");
    if(!fw1) {
        return -16;
    }
    fseek(fr1, 0L, SEEK_END);
    long length1 = ftell(fr);
    fseek(fr1, 0, SEEK_SET);
    NODE * list_dec = NULL;
    read_table(&list_dec, fr1);
    print_dec_list(list_dec);
    char * buf1 = (char*)malloc((length1) * sizeof(char) * BIT8);
    decrypt_file(list_dec, buf1, fr1, fw1);
    free(buf1);
    fclose(fr1);
    fclose(fw1);
    return 0;
}