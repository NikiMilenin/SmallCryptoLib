#include "YAFC.h"

uint16_t YetAnotherFeistelCipher::F(uint16_t word, uint16_t b)
{
    return left_cycle_rotate(b, 3) ^ (~word);
}

void YetAnotherFeistelCipher::block_encrypt(uint64_t* block, uint64_t word)
{
    uint16_t* m0 = (uint16_t*) block;
    uint16_t* m1 = m0 + 1;
    uint16_t* m2 = m0 + 2;
    uint16_t* m3 = m0 + 3;

    for(int i=0; i<NUM_OF_ROUNDS; ++i)
    {
        uint16_t round_key = (uint16_t)(right_cycle_rotate(word, i*2));
        uint16_t c3 = left_cycle_rotate(*m1, 5);
        uint16_t c1 = c3 ^ *m3;
        uint16_t c2 = c1 ^ *m0;
        uint16_t c0 = F(round_key, *m0) ^ *m2;
        *m0 = c0;
        *m1 = c1;
        *m2 = c2;
        *m3 = c3; 
    }
}

void YetAnotherFeistelCipher::block_decrypt(uint64_t* block, uint64_t word)
{
    uint16_t* c0 = (uint16_t*) block;
    uint16_t* c1 = c0 + 1;
    uint16_t* c2 = c0 + 2;
    uint16_t* c3 = c0 + 3;

    for(int i = NUM_OF_ROUNDS - 1; i >= 0; --i)
    {
        uint16_t round_key = (uint16_t)(right_cycle_rotate(word, i*2));
        uint16_t m1 = right_cycle_rotate(*c3, 5);
        uint16_t m3 = *c1 ^ *c3;
        uint16_t m0 = *c1 ^ *c2;
        uint16_t m2 = F(round_key, m0) ^ *c0;
        *c0 = m0;
        *c1 = m1;
        *c2 = m2;
        *c3 = m3;
    } 
}