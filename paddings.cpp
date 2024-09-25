#include "paddings.h"


int add_asnxi_x9_23_padding(char* buffer, int len, int bytes_read)
{
    buffer[len - 1] = len - bytes_read;
    return 0;
}


int get_asnxi_x9_23_padding_len(char* buffer, int len)
{
    int del = 0;
    if (buffer[len - 2] == '\000')
    {
        del = buffer[len - 1]; 
    }
    if (buffer[len - 1] == 1)
    {
        del = 1;
    }
    return del;
}


int add_iso_iec_7816_padding(char* buffer, int len, int bytes_read)
{
    buffer[bytes_read] = 0x80;
    for (int i = bytes_read + 1; i < len; i++)
    {
        buffer[i] = 0x00;
    }
    return 0;
}


int get_iso_iec_7816_padding_len(char* buffer, int len)
{
    for (int i = len - 1; i > 0; i--)
    {
        if (buffer[i] == '\000') continue;
        if (buffer[i] == '\200') return len - i;
    }
    return 0;
}