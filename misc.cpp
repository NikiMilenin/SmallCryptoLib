#include "misc.h"


uint64_t random_64bit_message()
{
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    return distribution(gen);
}