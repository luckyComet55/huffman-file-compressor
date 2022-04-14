#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "encoder.h"
#define ALPHABET_CAP 256
#define SINGLE_MEM_USAGE 65536
#define MAX_EXT_LEN 8

void print_meta(FILE * fw, char * ext_, int tail, unsigned long long * freq) {
    fprintf(fw, "%s %d ", ext_, tail);
    for (int i = 0; i < ALPHABET_CAP; ++i) {
        fprintf(fw, "%llu ", freq[i]);
    }
}

void _add_encoded_(NODE ** head, char symb, unsigned char code[MAX_CODE_LEN]) {
    NODE * new_node = (NODE*) malloc(sizeof (NODE));
    new_node->symb = symb;
    strcpy(new_node->code, code);
    new_node->isSymb = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = *head;
    *head = new_node;
}

void _encode_(NODE * root, NODE ** encoded_, unsigned char code[MAX_CODE_LEN]) {
    if(!(root->left)) {
        _add_encoded_(encoded_, root->symb, code);
        return;
    }
    unsigned char temp[MAX_CODE_LEN] = { 0 };
    strcpy(temp, code);
    strcat(temp, "0");
    _encode_(root->left, encoded_, temp);
    temp[strlen(temp) - 1] = '1';
    _encode_(root->right, encoded_, temp);
}

void print_encode_table(const NODE * head, FILE * fw, const int tail) {
    fseek(fw, 0L, SEEK_SET);
    fprintf(fw, "SOT\n");
    while(head) {
        fprintf(fw, "%c %s\n", head->symb, head->code);
        head = head->next;
    }
    fprintf(fw, "EOT\nTail %d\n", tail);
}


void encode_text(NODE *head, FILE *fr, FILE * fw, unsigned long long * freq, char * ext) {
    fseek(fr, 0L, SEEK_END);
    unsigned long long un_com_file_len_ = ftell(fr);
    fseek(fr, 0L, SEEK_SET);
    unsigned long long main_com_ser = un_com_file_len_ / SINGLE_MEM_USAGE;
    unsigned com_tail = un_com_file_len_ % SINGLE_MEM_USAGE;
    unsigned char * buf = (unsigned char*) calloc(SINGLE_MEM_USAGE * MAX_CODE_LEN, sizeof (unsigned char));
    unsigned char * res = (unsigned char*) calloc(SINGLE_MEM_USAGE * MAX_CODE_LEN / BIT8, sizeof (unsigned char));
    unsigned char * buf_temp = buf;
    print_meta(fw, ext, 0, freq);
    printf("Series: %llu\n", main_com_ser);
    int loc_tail = 0;
    for (unsigned long long i = 0; i < main_com_ser; ++i) {
        unsigned char symbol;
        NODE * node;
        for (int j = 0; j < SINGLE_MEM_USAGE; ++j) {
            symbol = fgetc(fr);
            node = find_node(head, symbol);
            if (!node) {
                printf("Encoding error\n");
                exit(-7);
            }
            strcpy(buf_temp, node->code);
            buf_temp += strlen(node->code);
        }
        BIT2CHAR symb_bit;
        unsigned long long bufsiz = strlen(buf) / BIT8;
        loc_tail = strlen(buf) % BIT8;
        for (unsigned long long j = 0; j < bufsiz; ++j) {
            symb_bit.mbit.b1 = buf[j*BIT8 + 0];
            symb_bit.mbit.b2 = buf[j*BIT8 + 1];
            symb_bit.mbit.b3 = buf[j*BIT8 + 2];
            symb_bit.mbit.b4 = buf[j*BIT8 + 3];
            symb_bit.mbit.b5 = buf[j*BIT8 + 4];
            symb_bit.mbit.b6 = buf[j*BIT8 + 5];
            symb_bit.mbit.b7 = buf[j*BIT8 + 6];
            symb_bit.mbit.b8 = buf[j*BIT8 + 7];
            res[j] = symb_bit.symb;
        }
        for (unsigned long long j = 0; j < bufsiz; ++j) {
            fputc(res[j], fw);
        }
        bufsiz = bufsiz * BIT8 + loc_tail;
        for (int j = 0; j < loc_tail; ++j) {
            buf[bufsiz - bufsiz + j] = buf[bufsiz - loc_tail + j];
            buf[bufsiz - loc_tail + j] = 0;
        }
        buf_temp = buf + loc_tail;
    }
    for (unsigned long long i = 0; i < SINGLE_MEM_USAGE * MAX_CODE_LEN; ++i) {
        buf_temp[i] = 0;
    }
    printf("Tail from previous sequence is: %d\n", loc_tail);
    unsigned char symbol;
    NODE * node;
    printf("\nRemaining memory under 64KB: %d\n", com_tail);
    for (int i = 0; i < com_tail; ++i) {
        symbol = fgetc(fr);
        node = find_node(head, symbol);
        if (!node) {
            printf("Encoding error\n");
            exit(-7);
        }
        strcpy(buf_temp, node->code);
        buf_temp += strlen(node->code);
    }
    loc_tail = strlen(buf) % BIT8;
    int len = strlen(buf) / BIT8 + (loc_tail != 0);
    printf("Final tail length: %d\n", loc_tail);
    BIT2CHAR symb_bit;
    for (int i = 0; i < len; ++i) {
        symb_bit.mbit.b1 = buf[i*BIT8 + 0];
        symb_bit.mbit.b2 = buf[i*BIT8 + 1];
        symb_bit.mbit.b3 = buf[i*BIT8 + 2];
        symb_bit.mbit.b4 = buf[i*BIT8 + 3];
        symb_bit.mbit.b5 = buf[i*BIT8 + 4];
        symb_bit.mbit.b6 = buf[i*BIT8 + 5];
        symb_bit.mbit.b7 = buf[i*BIT8 + 6];
        symb_bit.mbit.b8 = buf[i*BIT8 + 7];
        res[i] = symb_bit.symb;
    }
    for (int i = 0; i < len; ++i) {
        fputc(res[i], fw);
    }
    fseek(fw, 0L, SEEK_SET);
    print_meta(fw, ext, (8 - loc_tail) % BIT8, freq);
    free(res);
    free(buf);
}

void get_frequency_(unsigned long long * freq, FILE * fr) {
    fseek(fr, 0L, SEEK_END);
    unsigned long long length = ftell(fr);
    fseek(fr, 0L, SEEK_SET);
    for (unsigned long long i = 0; i < length; ++i) {
        (freq)[(unsigned char)fgetc(fr)]++;
    }
    fseek(fr, 0L, SEEK_SET);
}

NODE * freq_tree_assembly_(FILE * fr) {
    unsigned long long * FREQUENCY = (unsigned long long*) calloc(ALPHABET_CAP, sizeof (unsigned long long));
    get_frequency_(FREQUENCY, fr);
    NODE * freq_tree = NULL;
    int catch_err = 0;
    for (int i = 0; i < ALPHABET_CAP; ++i) {
        if(FREQUENCY[i] != 0) {
            catch_err++;
            add2list(&freq_tree, FREQUENCY[i], (unsigned char)i);
        }
    }
    if(catch_err == 1) {
        strcpy(freq_tree->code, "0");
    }
    list2tree(&freq_tree);
    free(FREQUENCY);
    return freq_tree;
}

NODE * gc_seq(FILE * fr) {
    NODE * code_tree = freq_tree_assembly_(fr);
    NODE * code_list = NULL;
    unsigned char code[MAX_CODE_LEN] = { 0 };
    _encode_(code_tree, &code_list, code);
    if(code_list->next == NULL) {
        strcpy(code_list->code, "0");
    }
    code_tree = delete_tree(code_tree);
    //print_list(code_list);
    return code_list;
}

void get_suffix(char * filename, char * suf_buf) {
    int len = strlen(filename);
    for (int i = len - 1; i >= 0; --i) {
        if(filename[i] == '.') {
            strcpy(suf_buf, filename + i + 1);
            strcpy(filename + i + 1, "holk");
            for (int j = i + 5; j < len; ++j) {
                filename[j] = 0;
            }
            break;
        }
        if(filename[i] == '\\') {
            strcpy(suf_buf, "txt");
            strcpy(filename + len, ".holk");
            for (int j = filename + len + 5; j < len; ++j) {
                filename[j] = 0;
            }
            break;
        }
    }
}

void compress_file(char * filename) {
    char ext_[MAX_EXT_LEN] = { 0 };
    printf("%s\n", filename);
    FILE * fr = fopen(filename, "rb");
    if(fr) {
        printf("YES\n");
    } else {
        printf("AT");
        exit(-1);
    }
    get_suffix(filename, ext_);
    unsigned long long * FREQUENCY = (unsigned long long*) calloc(ALPHABET_CAP, sizeof (unsigned long long));
    get_frequency_(FREQUENCY, fr);
    FILE * fw = fopen(filename, "wb");
    if(fw) {
        printf("YYYES\n");
    }
    print_meta(fw, ext_, 0, FREQUENCY);
    printf("%s\n", ext_);
    printf("%s\n", filename);
    NODE * seq_list = gc_seq(fr);
    encode_text(seq_list, fr, fw, FREQUENCY, ext_);
    seq_list = delete_list(seq_list);
    free(FREQUENCY);
    fclose(fr);
    fclose(fw);
}
