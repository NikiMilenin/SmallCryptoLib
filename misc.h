#ifndef MISC_H
#define MISC_H


#include <cstdint>
#include <random>


uint64_t random_64bit_message();


template<typename T> T right_cycle_rotate(T num, int shift)
{
    int n = sizeof(num) * 8;
    return (num >> shift) | (num << (n - shift));
}


template<typename T> T left_cycle_rotate(T num, int shift)
{
    int n = sizeof(num) * 8;
    return (num << shift) | (num >> (n - shift));
}


#endif