#include "encoder.h"
#include "btree.h"
#include "decoder.h"
#include <stdio.h>
//<<<<<<< HEAD
#include <time.h>
#include <stdlib.h>


int main() {
    FILE * fr = fopen("input4", "rb");
    if(!fr) {
        return -5;
    }
    FILE * fw = fopen("output", "wb");
    if(!fw) {
        return -6;
    }

    fseek(fr, 0L, SEEK_END);
    long long int length = ftell(fr);
    if(length == 0) {
        printf("File is empty\n");
        exit(-8);
    }
    printf("%llu\n", length);
    compress_file(fr, fw);
    fclose(fr);
    fclose(fw);
//========================================================
    FILE * fr1 = fopen("output", "rb");
    if(!fr1) {
        return -15;
    }
    FILE * fw1 = fopen("output2", "wb");
    if(!fw1) {
        return -16;
    }
    fseek(fr1, 0L, SEEK_END);
    unsigned long long length1 = ftell(fr);
    fseek(fr1, 0, SEEK_SET);
    NODE * list_dec = NULL;
    read_table(&list_dec, fr1);
    //print_dec_list(list_dec);
    char * buf1 = (char*)calloc((length1) * BIT8, sizeof(char));
    decrypt_file(list_dec, buf1, fr1, fw1);
    free(buf1);
    fseek(fw1, 0L, SEEK_END);
    printf("\nSize of decrypted file: %llu", ftell(fw1));
    fclose(fr1);
    fclose(fw1);
    return 0;
}