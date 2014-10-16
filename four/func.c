#include "kernlib.h"

extern unsigned char keyboard_map[128];

extern unsigned int current_loc; //current location of write pointer:  defined in kernel.c
/* video memory begins at address 0xb8000 */
extern char *vidptr;  //memory of screen buffer: defined in kernel.c

extern void write_port(char,char);
extern char read_port(char);
extern char *input_buffer;
extern int input_buf_loc;
extern void shell();
extern int done_typing;

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if ( dst < src )
		for ( size_t i = 0; i < size; i++ )
			dst[i] = src[i];
	else
		for ( size_t i = size; i != 0; i-- )
			dst[i-1] = src[i-1];
	return dstptr;
}

size_t strlen(const char* string)
{
	size_t result = 0;
	while ( string[result] )
		result++;
	return result;
}

int strcmp(const void* aptr, const void* bptr)
{
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	int i=0;
	while(a[i] != '\0' || b[i] != '\0'){
		if ( a[i] < b[i] )
			return -1;
		else if ( b[i] < a[i] )
			return 1;
		i++;
	}
	return 0;
}

int memcmp(const void* aptr, const void* bptr, size_t size)
{
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for ( size_t i = 0; i < size; i++ )
		if ( a[i] < b[i] )
			return -1;
		else if ( b[i] < a[i] )
			return 1;
	return 0;
}

void* memset(void* bufptr, int value, size_t size)
{
	unsigned char* buf = (unsigned char*) bufptr;
	for ( size_t i = 0; i < size; i++ )
		buf[i] = (unsigned char) value;
	return bufptr;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for ( size_t i = 0; i < size; i++ )
		dst[i] = src[i];
	return dstptr;
}

int puts(const char* string)
{
	int i=0;
	i = kprint(string);
	kprint_newline();
	i++; 
	return i;
}

int kprint_green(const char *str)
{
        char *falseflag="This really isn't the flag you are looking for...\n";
        unsigned int i = 0;
	unsigned int j = 0;
        while (str[i] != '\0') {
                switch(str[i]){
			case '\n':
                        	kprint_newline();
				i++;
				break;
			default:
				vidptr[current_loc++] = str[i++];
				vidptr[current_loc++] = 0x0a;
		}
	}
	return i;
}

int kprint(const char *str)
{
        char *falseflag="This isn't the flag you are looking for...\n";
        unsigned int i = 0;
	unsigned int j = 0;
        while (str[i] != '\0') {
                switch(str[i]){
			case '\n':
                        	kprint_newline();
				i++;
				break;
			default:
				vidptr[current_loc++] = str[i++];
				vidptr[current_loc++] = 0x07;
		}
	}
	return i;
}

void kprint_newline(void)
{
	char buff[SCREENSIZE];
        unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
        current_loc = current_loc + (line_size - current_loc % (line_size));
	if(current_loc >= SCREENSIZE){
		clear_screen();
	}
		
}

void keyboard_handler_main(void) {
        unsigned char status;
        char keycode;

        /* write EOI */
        write_port(0x20, 0x20);

        status = read_port(KEYBOARD_STATUS_PORT);
        /* Lowest bit of status will be set if buffer is not empty */
        if (status & 0x01) {
                keycode = read_port(KEYBOARD_DATA_PORT);
                if(keycode < 0)
                        return;
		input_buffer[input_buf_loc++]=keyboard_map[keycode];	
		if(keyboard_map[keycode]=='\n'){
			input_buffer[input_buf_loc-1]='\0';
			kprint_newline();
			done_typing=1;
		} else {
                	vidptr[current_loc++] = keyboard_map[keycode];
                	vidptr[current_loc++] = 0x07;  
		}
        }
}

void gets(char * input){
	int i;
		
}

void clear_screen(void)
{       
        unsigned int i = 0;
        while (i < SCREENSIZE) {
                vidptr[i++] = ' ';
                vidptr[i++] = 0x07;
        }
	current_loc=0;
}  


int atoi(char *str)
{
    int res = 0; // Initialize result
 
    // Iterate through all characters of input string and update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}


void reverse(char *s){
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

void itoa(int n, char *s) {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}
