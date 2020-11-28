#ifndef KERNEL_H
#define KERNEL_H

#define VGA_ADDRESS 0xB8000
#define WHITE_COLOR 15
typedef unsigned short UINT16;//16bit value

//VGA
static UINT16 VGA_DefaultEntry(unsigned char ch_to_print);
UINT16 *TERMINAL_BUFFER;

//UTILS
int countString(string str);
string splitString(string str, int index);

//PARSER
bool programParser(string program, string wholeString);

#endif
