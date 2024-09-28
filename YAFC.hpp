#ifndef YAFC_H
#define YAFC_H


#include <iostream>
#include "misc.hpp"
#include "block_cipher.hpp"


#define NUM_OF_ROUNDS 8


class YetAnotherFeistelCipher : public BlockCipher
{
public:
    YetAnotherFeistelCipher() {}
    virtual ~YetAnotherFeistelCipher() {}
    void block_decrypt(uint64_t* block, uint64_t word);
    void block_encrypt(uint64_t* block, uint64_t word);
    char get_block_len() {return 8;};
    uint16_t F(uint16_t word, uint16_t b);
};


#endif