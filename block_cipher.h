#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H


#include <cstdint>
#include <iostream>


class BlockCipher
{
public:
    void block_encrypt(uint64_t* block, uint64_t word) {}
    void block_decrypt(uint64_t* block, uint64_t word) {}
    short get_block_len() {return 0;}
};


#endif