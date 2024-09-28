#ifndef PADDING_H
#define PADDING_H


typedef enum {
    NO_PADDING,
    ISO_IEC_7816,
    ANSI_X9_23
} padding_modes_t;

char add_asnxi_x9_23_padding(char* buffer, char len, char bytes_read);
char get_asnxi_x9_23_padding_len(char* buffer, char len);

char add_iso_iec_7816_padding(char* buffer, char len, char bytes_read);
char get_iso_iec_7816_padding_len(char* buffer, char len);
#endif