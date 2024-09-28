#include <fstream>
#include "misc.h"
#include "YAFC.h"
#include "modes.h"


int main()
{
    std::ifstream input("input.txt", std::ios::binary);
    std::ofstream output("encrypted.txt", std::ios::binary);
    
    uint64_t key = random_64bit_message();
    uint64_t iv = random_64bit_message();

    padding_modes_t padding_mode = ISO_IEC_7816;

    ECB<YetAnotherFeistelCipher> cipher;
    cipher.encryption(&input, &output, key, padding_mode);

    input.close();
    output.close();
    input.open("encrypted.txt", std::ios::binary);
    output.open("decrypted.txt", std::ios::binary);

    cipher.decryption(&input, &output, key, padding_mode);

    return 0;
}