/*
*  kernel.c
*/
void kmain(void)
{
	char *str = "\x1b\x2d\x37\x62\x15\x0b\x0c\x63\x62\x0b\x36\x62\x35\x23\x31\x62\x23\x62\x29\x27\x30\x2c\x27\x2e\x62\x23\x2e\x2e\x62\x23\x2e\x2d\x2c\x25\x6c\x62\x16\x2a\x27\x62\x24\x2e\x23\x25\x62\x2b\x31\x78\x62\x09\x27\x30\x2c\x27\x2e\x1d\x12\x30\x2d\x25\x30\x23\x2f\x2b\x2c\x25\x1d\x0b\x31\x1d\x07\x23\x31\x3b\x42";
	char *vidptr = (char*)0xb8000; 	//video mem begins here.
	char key = '\x42';
	unsigned int i = 0;
	unsigned int j = 0;

	char *falseflag = "This isn't the flag you are looking for";
	

	//clear all
	while(j < 80 * 25 * 2) {
		//blank character
		vidptr[j] = ' ';
		//attribute-byte: light grey on black screen	
		vidptr[j+1] = 0x07; 		
		j = j + 2;
	}
	j = 0;
	while(str[j] != '\0') {
		vidptr[i] = str[j]^key;
		vidptr[i+1] = 0x02;
		++j;
		i = i + 2;
	}	
	return;
}

//\x1b\x2d\x37\x62\x15\x0b\x0c\x63\x62\x0b\x36\x62\x35\x23\x31\x62\x23\x62\x29\x27\x30\x2c\x27\x2e\x62\x23\x2e\x2e\x62\x23\x2e\x2d\x2c\x25\x6c\x62\x16\x2a\x27\x62\x24\x2e\x23\x25\x62\x2b\x31\x78\x62\x09\x27\x30\x2c\x27\x2e\x1d\x12\x30\x2d\x25\x30\x23\x2f\x2b\x2c\x25\x1d\x0b\x31\x1d\x07\x23\x31\x3b\x42
//key = \x42