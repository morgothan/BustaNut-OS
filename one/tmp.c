#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
	char *clr = "I.Wrote.A.Kernel!";
//	char *clr = "\x1b\x2d\x37\x62\x15\x0b\x0c\x63\x62\x0b\x36\x62\x35\x23\x31\x62\x23\x62\x29\x27\x30\x2c\x27\x2e\x62\x23\x2e\x2e\x62\x23\x2e\x2d\x2c\x25\x6c\x62\x16\x2a\x27\x62\x24\x2e\x23\x25\x62\x2b\x31\x78\x62\x09\x27\x30\x2c\x27\x2e\x1d\x12\x30\x2d\x25\x30\x23\x2f\x2b\x2c\x25\x1d\x0b\x31\x1d\x07\x23\x31\x3b\x42";
	char *enc;
	char key = '\x69';
	unsigned int i = 0;
	unsigned int j = 0;

	if(argc > 1){
		clr=argv[1];
	}
	if(argc > 2){
		key=argv[2][0];
	}
	enc = malloc(strlen(clr));
	for(j=0;j<= strlen(clr);j++) {
		enc[j] = clr[j] ^ key;
		printf("\\x%02x", enc[j]);
	}
	putchar('\n');
	
	return;
}
