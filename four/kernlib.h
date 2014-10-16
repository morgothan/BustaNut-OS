#ifndef _KERNLIB_H
#define _KERNLIB_H 1
#include <stddef.h> 
#include "cdefs.h"
 
#ifdef __cplusplus
extern "C" {
#endif

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define MAXSIZE 1024


 
int kprint(const char*);
void kprint_newline();
void keyboard_handler_main();
void clear_screen();
void gets(char *);
int puts(const char*);
int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
void itoa(int, char *);
void reverse(char *);
 
#ifdef __cplusplus
}
#endif
 
#endif
