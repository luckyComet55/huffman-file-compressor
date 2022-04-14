#include "encoder.h"
#include "btree.h"
#include "decoder.h"
#include <stdio.h>
//<<<<<<< HEAD
#include <time.h>
#include <stdlib.h>


int main() {
    char filename[MAX_CODE_LEN + 5] = { 0 };
    scanf("%s", filename);
    compress_file(filename);
    scanf("%s", filename);
    decompress_file(filename);
    return 0;
}