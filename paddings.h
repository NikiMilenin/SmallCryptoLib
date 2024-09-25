#ifndef PADDING_H
#define PADDING_H

int add_asnxi_x9_23_padding(char* buffer, int len, int bytes_read);
int get_asnxi_x9_23_padding_len(char* buffer, int len);

int add_iso_iec_7816_padding(char* buffer, int len, int bytes_read);
int get_iso_iec_7816_padding_len(char* buffer, int len);
#endif