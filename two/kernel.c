#include "keyboard_map.h"

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

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);

/* current cursor location */
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;
unsigned int randcolour = 0;
unsigned int letter =0;


void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	struct IDT_entry{
	   unsigned short int offset_lowerbits;
	   unsigned short int selector;
	   unsigned char zero;
	   unsigned char type_attr;
	   unsigned short int offset_higherbits;
	};

	struct IDT_entry IDT[IDT_SIZE];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler; 
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);  
	write_port(0xA1 , 0x00);  

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);
}

void kprint(const char *str)
{
	char *falseflag="This isn't the flag you are looking for...\n";
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = 0x07;
	}
}

void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07; 
	}
}
void kwin(char key)
{
	unsigned int i = 0;
	char *secret = "\x0b\x1c\x1d\x1d\x01\x06\x05\x0c\x69"; //butthole XORed with \x69
	char secret_decode[9];
	char secret_key = '\x69';
	
	char *str[] = {	"\x3b\x0d\x17\x4c\x25\x0d\x16\x4c\x36\x0a\x07\x4c\x24\x0b\x10\x11\x16\x4c\x2e\x07\x16\x16\x07\x10\x62",
			"\x34\x1b\x11\x5b\x21\x1d\x10\x5b\x26\x10\x16\x1a\x1b\x11\x75",
			"\x3c\x1b\x03\x5a\x39\x15\x1a\x0d\x5a\x35\x06\x11\x5a\x20\x1c\x11\x06\x11\x4b\x74",
			"\x39\x1b\x06\x11\x5a\x20\x1c\x11\x1a\x5a\x20\x1c\x1d\x07\x5a\x39\x15\x1a\x0d\x74",
			"\x2f\x0d\x1c\x1c\x01\x06\x0f\x46\x2b\x04\x07\x1b\x0d\x1a\x68",
			"\x28\x0a\x1b\x1b\x06\x01\x08\x41\x38\x0e\x1d\x02\x0a\x1d\x4e\x6f",
			"\x22\x09\x14\x18\x42\x23\x02\x09\x42\x05\x1f\x42\x18\x04\x09\x42\x2a\x20\x2d\x2b\x6c", 
			"\x2e\x20\x37\x2b\x20\x29\x4b\x35\x17\x0a\x02\x17\x04\x08\x08\x0c\x0b\x02\x4b\x0c\x16\x4b\x23\x30\x2b\x65"};
	
	while(secret[i] != '\0'){
		secret_decode[i]=secret[i]^secret_key;  // XOR the secert key to get the plaintext
		i++;
	}
	current_loc=0;			//set screen pointer back to the start of the screen
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint("Trying with key: ");
	secret_decode[letter]='\0';	//ends the secret key string where the user has guest up to.
	kprint(secret_decode);		//prints the decoded secret string up to where the user guessed
	vidptr[current_loc++]=key;	//prints the guessed character
	vidptr[current_loc++]=0x0f;	//prints it in white
	kprint("        ");		//prints spaces to clear out any residual charecters after the guessed character
	kprint_newline();
	kprint("Is this the flag: ");	
	
	i=0;
        while (str[letter][i] != '\0') {	//takes the flag stored at the letter index and XORs it with guess character.
                vidptr[current_loc++] = str[letter][i++]^key;
                vidptr[current_loc++] = randcolour;	//prints the decrypted flag in a random color
	}
	while(current_loc<SCREENSIZE){		//cleares out the rest of the screen
		vidptr[current_loc++]=' ';
		vidptr[current_loc++]=0x07;
	}
	secret_decode[letter]=secret[letter]^secret_key;	
	if(secret_decode[letter] == key){	//checks if the key entered was correct
		letter++;			//if so move onto the next letter
	}
	if(letter==8){				//if we are done start over.
		letter=0;
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
		/*vidptr[current_loc++] = keyboard_map[keycode];
		vidptr[current_loc++] = 0x07;	*/
		kwin(keyboard_map[keycode]);
	}
}
void kmain(void)
{
	clear_screen();
	kprint("Congrats on booting the BUSTANUT kernel. ");
	kprint_newline();
	kprint("(BUSTANUT is Undergoing Sustained Testing And is Not a Unix Terminal)");
	kprint_newline();
	kprint("Now get the flag out of it!");

	idt_init();
	kb_init();

	while(1){
		randcolour = ((randcolour + 1) % 16) + 1;  //used to pick the random color. 
	}
}
