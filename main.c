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
    printf("1 - encoding \n2 - decoding\n3 - exit\n");
    int commCode=0;
    while(commCode!=3) {
        printf("Enter command code:\n");
        scanf("%d", &commCode);
        char fileName[ALPH_SIZE*2] = {0};
        if (commCode == 1) {
            printf("Enter path to the source file:\n");
            scanf("%s", &fileName);
            NODE *head1 = NULL;
            FILE *fr = fopen(fileName, "rb");
            if (!fr) {
                return -5;
            }
            printf("Enter path to the destination file:\n");
            scanf("%s", &fileName);
            FILE *fw = fopen(fileName, "wb");
            if (!fw) {
                return -6;
            }

            fseek(fr, 0L, SEEK_END);
            long length = ftell(fr);
            if (length == 0) {
                printf("File is empty\n");
                exit(-8);
            }
            //printf("%lu", length);
            fseek(fr, 0, SEEK_SET);
            for (int i = 0; i < length; ++i) {
                freq[(unsigned char) fgetc(fr)]++;
            }
            for (int i = 0; i < ALPH_SIZE; ++i) {
                if (freq[i] != 0) {
                    add2list(&head1, freq[i], i);
                }
            }
            list2tree(&head1);
            NODE *list_enc = NULL;
            unsigned char code[MAX_CODE_LEN] = {0};
            _encode_(head1, &list_enc, code);
            unsigned char *buf = (unsigned char *) malloc((length) * sizeof(unsigned char) * MAX_CODE_LEN);
            compress_file(buf, list_enc, fr, fw);
            head1 = delete_tree(head1);
            list_enc = delete_list(list_enc);
            free(buf);
            fclose(fr);
            fclose(fw);
        }
//========================================================
        else if (commCode == 2) {
            printf("Enter path to the source file:\n");
            scanf("%s", &fileName);
            NODE *head1 = NULL;
            FILE *fr1 = fopen(fileName, "rb");
            if (!fr1) {
                return -15;
            }
            printf("Enter path to the destination file:\n");
            scanf("%s", &fileName);
            FILE *fw1 = fopen(fileName, "wb");
            if (!fw1) {
                return -16;
            }
            fseek(fr1, 0L, SEEK_END);
            long length1 = ftell(fr1);
            fseek(fr1, 0, SEEK_SET);
            NODE *list_dec = NULL;
            read_table(&list_dec, fr1);
            print_dec_list(list_dec);
            char *buf1 = (char *) malloc((length1) * sizeof(char) * BIT8);
            decrypt_file(list_dec, buf1, fr1, fw1);
            free(buf1);
            fclose(fr1);
            fclose(fw1);
        } else {
            printf("Wrong code format.");

        }
    }
    return 0;
}