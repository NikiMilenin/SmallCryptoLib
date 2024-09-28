#ifndef MODES_H
#define MODES_H


#include <fstream>
#include <cstring>
#include <stdint.h>
#include <block_cipher.hpp>
#include <YAFC.hpp>
#include <paddings.hpp>


template<class Cipher> class Mode
{
protected:
    Cipher alghoritm;
    char block_len;
    char get_block(std::ifstream* input, char* buffer, padding_modes_t padding_mode)
    {
        std::fill(&buffer[0], &buffer[this->block_len], 0);
        (*input).read(buffer, this->block_len);
        std::streamsize bytes_read = (*input).gcount();
        if (bytes_read == 0 ) return bytes_read;
        if (bytes_read < this->block_len) this->add_padding(buffer, bytes_read, padding_mode);
        return bytes_read;
    }
    char procede_block(std::ofstream* output, char* buffer, padding_modes_t padding_mode)
    {
        int del = get_padding_len(buffer, padding_mode);
        (*output).write(reinterpret_cast<char*>(buffer), this->block_len - del);
        return 0;
    }
    char add_padding(char* buffer, char bytes_read, padding_modes_t padding_mode)
    {
        switch(padding_mode)
        {
            case NO_PADDING:
                return 0;
            case ISO_IEC_7816:
                add_iso_iec_7816_padding(buffer, this->block_len, bytes_read);
                return 0;
            case ANSI_X9_23:
                add_asnxi_x9_23_padding(buffer, this->block_len, bytes_read);
        }
        return 0;
    }
    char get_padding_len(char* buffer, padding_modes_t padding_mode)
    {
        switch(padding_mode)
        {
            case NO_PADDING:
                return 0;
            case ISO_IEC_7816:
                return get_iso_iec_7816_padding_len(buffer, this->block_len);
            case ANSI_X9_23:
                return get_asnxi_x9_23_padding_len(buffer, this->block_len);
        }
        return 0;
    }
};


template<class Cipher> class ECB : public Mode<Cipher>
{
public:
    ECB() 
    {
        this->block_len =this->alghoritm.get_block_len();
    }

    ~ECB() {}

    void encryption(std::ifstream* input, std::ofstream* output, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        while (true) {
            char bytes_read = this->get_block(input, buffer, padding_mode);
            if (bytes_read == 0 ) break;
            this->alghoritm.block_encrypt(reinterpret_cast<uint64_t*>(buffer), key);
            this->procede_block(output, buffer, NO_PADDING);
            if (bytes_read < this->block_len) break;
        }
    }

    void decryption(std::ifstream* input, std::ofstream* output, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        while (true) {
            char bytes_read = this->get_block(input, buffer, NO_PADDING);
            if (bytes_read == 0 ) break;
            this->alghoritm.block_decrypt(reinterpret_cast<uint64_t*>(buffer), key);
            this->procede_block(output, buffer, padding_mode);
            if (bytes_read < this->block_len) break;
        }
    }
};


template<class Cipher> class CBC : public Mode<Cipher>
{
public:
    CBC() 
    {
        this->block_len =this->alghoritm.get_block_len();
    }
    ~CBC() {}

    void encryption(std::ifstream * input, std::ofstream * output, uint64_t iv,uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t prev_block = iv;
        while (true) {
            char bytes_read = this->get_block(input, buffer, padding_mode);
            if (bytes_read == 0 ) break;
            uint64_t block = 0;
            memcpy(&block, buffer, 8);
            block = block ^ prev_block;
            memcpy(buffer, &block, 8);
            this->alghoritm.block_encrypt(reinterpret_cast<uint64_t*>(buffer), key);
            memcpy(&prev_block, buffer, 8);
            this->procede_block(output, buffer, NO_PADDING);
            if (bytes_read < this->block_len) break;
        }
    }

    void decryption(std::ifstream * input, std::ofstream * output, uint64_t iv, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t prev_block = iv;
        uint64_t decrypt_block = iv;
        while (true) {
            char bytes_read = this->get_block(input, buffer, NO_PADDING);
            if (bytes_read == 0 ) break;
            memcpy(&decrypt_block, &prev_block, 8);
            memcpy(&prev_block, buffer, 8);
            this->alghoritm.block_decrypt(reinterpret_cast<uint64_t*>(buffer), key);
            uint64_t block = 0;
            memcpy(&block, buffer, 8);
            block = block ^ decrypt_block;
            memcpy(buffer, &block, 8);
            this->procede_block(output, buffer, padding_mode);
            if (bytes_read < this->block_len) break;
        }
    }
};


template<class Cipher> class CFB : public Mode<Cipher>
{
public:
    CFB() 
    {
        this->block_len =this->alghoritm.get_block_len();
    }
    ~CFB() {}

    void encryption(std::ifstream* input, std::ofstream* output, uint64_t iv,uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t prev_block = iv;
        while (true) {
            char bytes_read = this->get_block(input, buffer, padding_mode);
            if (bytes_read == 0 ) break;
            this->alghoritm.block_encrypt(&prev_block, key);
            uint64_t block = 0;
            memcpy(&block, buffer, 8);
            block = block ^ prev_block;
            memcpy(buffer, &block, 8);
            memcpy(&prev_block, buffer, 8);
            this->procede_block(output, buffer, NO_PADDING);
            if (bytes_read < this->block_len) break;
        }
    }

    void decryption(std::ifstream * input, std::ofstream * output, uint64_t iv, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t decrypt_block = iv;

        while (true) {
            char bytes_read = this->get_block(input, buffer, NO_PADDING);
            if (bytes_read == 0 ) break;
            this->alghoritm.block_encrypt(&decrypt_block, key);
            uint64_t block = 0;
            memcpy(&block, buffer, 8);
            block = block ^ decrypt_block;
            memcpy(&decrypt_block, buffer, 8);
            memcpy(buffer, &block, 8);
            this->procede_block(output, buffer, padding_mode);
            if (bytes_read < this->block_len) break;
        }
    }
};


template<class Cipher> class OFB : public Mode<Cipher>
{
public:
    OFB() 
    {
        this->block_len =this->alghoritm.get_block_len();
    }
    ~OFB() {}

    void encryption(std::ifstream* input, std::ofstream* output, uint64_t iv,uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t prev_block = iv;
        while (true) {
            char bytes_read = this->get_block(input, buffer, padding_mode);
            if (bytes_read == 0 ) break;
            this->OFB_mode_policy(buffer, prev_block, key);
            this->procede_block(output, buffer, NO_PADDING);
            if (bytes_read < this->block_len) break;
        }
    }

    void decryption(std::ifstream * input, std::ofstream * output, uint64_t iv, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t decrypt_block = iv;
        while (true) {
            char bytes_read = this->get_block(input, buffer, NO_PADDING);
            if (bytes_read == 0 ) break;
            this->OFB_mode_policy(buffer, decrypt_block, key);
            this->procede_block(output, buffer, padding_mode);
            if (bytes_read < this->block_len) break;
        }
    }
private:
    void OFB_mode_policy(char* buffer, uint64_t cryptoblock, uint64_t key)
    {
        this->alghoritm.block_encrypt(&cryptoblock, key);
        uint64_t block = 0;
        memcpy(&block, buffer, 8);
        block = block ^ cryptoblock;
        memcpy(buffer, &block, 8);
    }
};


template<class Cipher> class CTR : public Mode<Cipher>
{
public:
    CTR() 
    {
        this->block_len =this->alghoritm.get_block_len();
    }
    ~CTR() {}

    void encryption(std::ifstream* input, std::ofstream* output, uint64_t iv,uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t counter = iv > 1000 ? iv / 1000 : iv % 750;
        while (true) {
            char bytes_read = this->get_block(input, buffer, padding_mode);
            if (bytes_read == 0 ) break;
            this->CTR_mode_policy(buffer, key, counter++);
            this->procede_block(output, buffer, NO_PADDING);
            if (bytes_read < this->block_len) break;
        }
    }

    void decryption(std::ifstream * input, std::ofstream * output, uint64_t iv, uint64_t key, padding_modes_t padding_mode)
    {
        char buffer[this->block_len];
        uint64_t counter = iv > 1000 ? iv / 1000 : iv % 750;
        while (true) {
            char bytes_read = this->get_block(input, buffer, NO_PADDING);
            if (bytes_read == 0 ) break;
            this->CTR_mode_policy(buffer, key, counter++);
            this->procede_block(output, buffer, padding_mode);
            if (bytes_read < this->block_len) break;
        }
    }
private:
    void CTR_mode_policy(char* buffer, uint64_t key, uint64_t counter)
    {
        uint64_t enc_block = counter;
        this->alghoritm.block_encrypt(&enc_block, key);
        uint64_t block = 0;
        memcpy(&block, buffer, 8);
        block = block ^ enc_block;
        memcpy(buffer, &block, 8);
    }
};
#endif