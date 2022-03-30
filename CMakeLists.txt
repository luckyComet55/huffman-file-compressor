cmake_minimum_required(VERSION 3.21)
project(huffman-file-compressor)

set(CMAKE_C_STANDARD 99)

add_executable(huffman-file-compressor main.c btree.c btree.h encoder.c encoder.h decoder.h decoder.c) #bits.c bits.h)
