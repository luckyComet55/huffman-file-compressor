#include "encoder.h"
#include "btree.h"
#include "decoder.h"
#include <stdio.h>

int main() {
    char filename[MAX_CODE_LEN + 5] = { 0 };
    printf("Enter name of file to be compressed:\n");
    scanf("%s", filename);
    compress_file(filename);
    printf("Enter name of file to be decompressed:\n");
    scanf("%s", filename);
    decompress_file(filename);
    return 0;
}