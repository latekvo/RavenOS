#ifndef KERNEL_H
#define KERNEL_H

//vga port address
#define VGA_ADDRESS 0xB8000

//defined white
#define WHITE_COLOR 15

//unsigned 16 bit type
typedef unsigned short UINT16;

//buffer for vga
UINT16 *TERMINAL_BUFFER;

#endif
