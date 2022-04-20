#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "encoder.h"
#include "decoder.h"
#define ALPHABET_CAP 256
#define SINGLE_MEM_USAGE 65536
#define MAX_EXT_LEN 8
#define SEV 0b10000000

int check_ext(char * filename) {
    for (int i = strlen(filename) - 1; i >= 0; --i) {
        if(filename[i] == '.') {
            if(strcmp(filename + i, ".holk") == 0) { return 0; }
        }
        if(filename[i] == '\\') { return 1; }
    }
    return 1;
}

NODE * get_meta(FILE * fr) {
    NODE * freq_tree = NULL;
    unsigned long long freq;
    for (int i = 0; i < ALPHABET_CAP; ++i) {
        fscanf(fr, "%llu ", &freq);
        if(freq != 0) {
            add2list(&freq_tree, freq, i);
        }
    }
    list2tree(&freq_tree);
    //print_tree(freq_tree);
    return freq_tree;
}

void decrypt_enc(FILE * fr, FILE * fw, NODE * root, int tail) {
    unsigned char * buf = (unsigned char*) calloc(SINGLE_MEM_USAGE, sizeof (unsigned char));
    unsigned char * res = (unsigned char*) calloc(SINGLE_MEM_USAGE * BIT8, sizeof (unsigned char));
    unsigned char * t_res = res;
    unsigned long long length = ftell(fr);
    unsigned long long temp_pos = length;
    fseek(fr, 0L, SEEK_END);
    length = ftell(fr) - length;
    fseek(fr, temp_pos, SEEK_SET);
    //printf("%llu %lu\n", temp_pos, ftell(fr));
    unsigned long long main_proc_len = length / SINGLE_MEM_USAGE;
    unsigned long long source_len = length * BIT8 - tail;

    printf("Recognised length of source file: %llu\n", source_len);

    unsigned com_tail = length % SINGLE_MEM_USAGE;
    NODE * t_root = root;
    for (unsigned long long i = 0; i < main_proc_len; ++i) {
        //fread(buf, sizeof(unsigned char), SINGLE_MEM_USAGE, fr);
        for (unsigned j = 0; j < SINGLE_MEM_USAGE; ++j) {
            buf[j] = getc(fr);
        }
        unsigned char t_temp;
        unsigned char sieve = SEV;
        for (unsigned j = 0; j < SINGLE_MEM_USAGE; ++j) {
            t_temp = buf[j];
            for (int k = 0; k < BIT8; ++k) {
                if(t_temp & sieve) {
                    t_root = t_root->right;
                } else {
                    t_root = t_root->left;
                }
                if(t_root->isSymb) {
                    //printf("Found one %c %d\n", t_root->symb, t_root->symb);
                    t_res[0] = t_root->symb;
                    t_res++;
                    t_root = root;
                }
                source_len--;
                t_temp = t_temp << 1;
            }
        }
        //fwrite(res, sizeof(unsigned char), t_res - res, fw);
        fputs(res, fw);
        for (int j = 0; j < SINGLE_MEM_USAGE; ++j) {
            buf[j] = 0;
        }
        for (int j = 0; j < SINGLE_MEM_USAGE * BIT8; ++j) {
            res[j] = 0;
        }
        t_res = res;
    }
    for (unsigned i = 0; i < com_tail; ++i) {
        //fread(buf, sizeof(unsigned char), com_tail, fr);
        buf[i] = getc(fr);
    }
    unsigned char t_temp;
    unsigned char sieve = SEV;
    for (unsigned i = 0; i < com_tail; ++i) {
        t_temp = buf[i];
        for (int j = 0; j < BIT8; ++j) {
            if(t_temp & sieve) {
                t_root = t_root->right;
            } else {
                t_root = t_root->left;
            }
            if(t_root->isSymb) {
                t_res[0] = t_root->symb;
                t_res++;
                t_root = root;
            }
            source_len--;
            t_temp = t_temp << 1;
            if(source_len == 0) { break; }
        }
    }
    //fwrite(res, sizeof(unsigned char), t_res - res, fw);
    fputs(res, fw);
    free(buf);
    free(res);
}

void set_ext(char * filename, char * ext) {
    int temp = strlen(filename);
    for (int i = temp - 1; i >= 0; --i) {
        if(filename[i] == '.') {
            //printf("Ye  %s %s\n", filename, ext);
            strcpy(filename + i + 1, ext);
            break;
        }
    }
    //printf("%s\n", filename);
}

void decompress_file(char * filename) {
    if(check_ext(filename)) { return; }
    FILE * fr = fopen(filename, "rb");
    if(!fr) { exit(-11); }
    char ext_[MAX_EXT_LEN] = { 0 };
    int tail;
    fscanf(fr, "%d %s", &tail, ext_);
    char new_file[FILENAME_MAX] = { 0 };
    strcpy(new_file, filename);
    set_ext(new_file, ext_);
    FILE * fw = fopen(new_file, "wb");
    if(!fw) { exit(99); }
    NODE * code_tree = get_meta(fr);
    decrypt_enc(fr, fw, code_tree, tail);

    printf("Length of decompressed file: %llu\n", ftell(fw));

    fclose(fr);
    fclose(fw);
}
