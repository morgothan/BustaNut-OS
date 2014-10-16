#include <stdbool.h>
#include <stdarg.h>
#include "shell.h" 
#include "kernlib.h"


/* shell.c */
extern unsigned char keyboard_map[128];
extern char * input_buffer;
extern int input_buf_loc;
extern int done_typing;

command_table_t CommandTable[MAX_COMMANDS];
int NumberOfCommands=-1; //start at -1 so initial command is at location 0
int uid=100;
int end=0;

void empty_command(char * arg){
	kprint("Im sorry, I don't know what you mean.\n");
}

void help_command(char * arg){
	int i;
	i=findCommand(arg);
	if(i < 0){
		kprint("Im sorry, I don't know anything about that.\n");
	} else {
		puts(CommandTable[i].description);
	}
		
}

void call_command(char * arg){
	int cmd_index=atoi(arg);
	int i;
	char *argument;
        void (*command_function)(char *);
	
	kprint("Calling : ");
	if(cmd_index < 0){
                empty_command(arg);
	} else if (CommandTable[cmd_index].name ){
		kprint(CommandTable[cmd_index].name);
		kprint("(");	
		argument=arg+strlen(arg)+1;
		kprint(argument);
		kprint(")\n");
		command_function = CommandTable[cmd_index].function;
		command_function(argument);
		argument=memset(argument, '\0' , strlen(argument));
		
	} else {
		i=findCommand(arg);
		if(i < 0){
			kprint("Unindexed function\nABORTING!\n");
		} else {
			kprint(CommandTable[i].name);	
			kprint("(");	
			argument=arg+strlen(arg)+1;
			kprint(argument);
			kprint(")\n");
			command_function = CommandTable[i].function;
			command_function(argument);
			argument=memset(argument, '\0' , strlen(argument));
        	}	
	}
}

void halt_command(char * arg){
	kprint("Shutting down\n");
	end=1; 
}

void clear_command(char * arg){
	clear_screen();
	kprint("Screen Cleared\n"); 
}

void guess_command(char * arg){
	kprint("Did you really think it would be: ");
	puts(arg);
}

void func_command(char * arg){
	int i;
	for (i=0; i<=NumberOfCommands;i++){
		kprint("Function Name: ");
		puts(CommandTable[i].name);
	}
}

void gp_command(char * arg){
	char uidstr[MAXSIZE];
	kprint("Current Acount info; \n");
	kprint("   User ID: ");
	itoa(uid,uidstr);
	puts(uidstr);
	kprint("   User Name: "); 
	switch(uid) {
	
		case 0:
			kprint("Root Account\n");
			break;
		case 100:
			kprint("Default User\n");
			break;
		case 500:
			kprint("Administrator Account\n");
			break;
		default:
			kprint("Unknown User\n");
		}
}

void bu_command(char * arg){
	kprint("UserID has been updated.\n"); 
	uid=atoi(arg);
}

void win_command(char * arg){
	unsigned int i = 0;
	unsigned int j = 0;
        char *secret = "\xc3\xcf\xc3\xcb\xd3\xd5\xc3\xcb\xc5\xd2\xa0"; //"cocksucker" XORed with \xa0
        char secret_decode[12];
        char secret_key = '\xa0';
	char decoded[100];
	int uids[3]={100,0,500};

        char *str[] = 	{ 
/*				
                                "First Letter down as user: User - FLAG(1)=\"I\"\n",                    //xor c xor 100
                                "Second Letter down as userid: Root - FLAG(2)=\"Wrote\"\n",             //xor o xor 0
                                "Third Letter down as userid: Administrator - FLAG(3)=\"A\"\n",         //xor c xor 500
                                "Now you are getting some place - FLAG(4)=\"Kernel\"\n",                //xor k xor 100
                                "Back to user: User? - FLAG(5)=\"With\"\n",                             //xor s xor 0
                                "Oh I get it, its a loop - FLAG(6)=\"A\"\n",                            //xor s xor 500
                                "But where did the UIDs come from? - FLAG(7)=\"Shell\"\n",              //xor u xor 100
                                "Windows and Unix? - FLAG(8)=\"What've\"\n",                            //xor c xor 0
                                "So close I can almost taste it - FLAG(9)=\"You\"\n",                   //xor e xor 500
                                "Last of the flags - FLAG(10)=\"Done?\"\n",                             //xor r xor 100
*/

"\xe1\xce\xd5\xd4\xd3\x87\xeb\xc2\xd3\xd3\xc2\xd5\x87\xc3\xc8\xd0\xc9\x87\xc6\xd4\x87\xd2\xd4\xc2\xd5\x9d\x87\xf2\xd4\xc2\xd5\x87\x8a\x87\xe1\xeb\xe6\xe0\x8f\x96\x8e\x9a\x85\xee\x85\xa7\x00",
"\x5c\x6a\x6c\x60\x61\x6b\x2f\x43\x6a\x7b\x7b\x6a\x7d\x2f\x6b\x60\x78\x61\x2f\x6e\x7c\x2f\x7a\x7c\x6a\x7d\x66\x6b\x35\x2f\x5d\x60\x60\x7b\x2f\x22\x2f\x49\x43\x4e\x48\x27\x3d\x26\x32\x2d\x58\x7d\x60\x7b\x6a\x2d\x0f\x00",
"\x63\x5f\x5e\x45\x53\x17\x7b\x52\x43\x43\x52\x45\x17\x53\x58\x40\x59\x17\x56\x44\x17\x42\x44\x52\x45\x5e\x53\x0d\x17\x76\x53\x5a\x5e\x59\x5e\x44\x43\x45\x56\x43\x58\x45\x17\x1a\x17\x71\x7b\x76\x70\x1f\x04\x1e\x0a\x15\x76\x15\x37\x00",
"\xe1\xc0\xd8\x8f\xd6\xc0\xda\x8f\xce\xdd\xca\x8f\xc8\xca\xdb\xdb\xc6\xc1\xc8\x8f\xdc\xc0\xc2\xca\x8f\xdf\xc3\xce\xcc\xca\x8f\x82\x8f\xe9\xe3\xee\xe8\x87\x9b\x86\x92\x8d\xe4\xca\xdd\xc1\xca\xc3\x8d\xaf\x00",
"\x51\x72\x70\x78\x33\x67\x7c\x33\x66\x60\x76\x61\x29\x33\x46\x60\x76\x61\x2c\x33\x3e\x33\x55\x5f\x52\x54\x3b\x26\x3a\x2e\x31\x44\x7a\x67\x7b\x31\x13\x00",
"\x6e\x49\x01\x68\x01\x46\x44\x55\x01\x48\x55\x0d\x01\x48\x55\x52\x01\x40\x01\x4d\x4e\x4e\x51\x01\x0c\x01\x67\x6d\x60\x66\x09\x17\x08\x1c\x03\x60\x03\x21\x00",
"\xe5\xd2\xd3\x87\xd0\xcf\xc2\xd5\xc2\x87\xc3\xce\xc3\x87\xd3\xcf\xc2\x87\xf2\xee\xe3\xd4\x87\xc4\xc8\xca\xc2\x87\xc1\xd5\xc8\xca\x98\x87\x8a\x87\xe1\xeb\xe6\xe0\x8f\x90\x8e\x9a\x85\xf4\xcf\xc2\xcb\xcb\x85\xa7\x00",
"\x5c\x62\x65\x6f\x64\x7c\x78\x2b\x6a\x65\x6f\x2b\x5e\x65\x62\x73\x34\x2b\x26\x2b\x4d\x47\x4a\x4c\x23\x33\x22\x36\x29\x5c\x63\x6a\x7f\x2c\x7d\x6e\x29\x0b\x00",
"\x62\x5e\x11\x52\x5d\x5e\x42\x54\x11\x78\x11\x52\x50\x5f\x11\x50\x5d\x5c\x5e\x42\x45\x11\x45\x50\x42\x45\x54\x11\x58\x45\x11\x1c\x11\x77\x7d\x70\x76\x19\x08\x18\x0c\x13\x68\x5e\x44\x13\x31\x00",
"\xfa\xd7\xc5\xc2\x96\xd9\xd0\x96\xc2\xde\xd3\x96\xd0\xda\xd7\xd1\xc5\x96\x9b\x96\xf0\xfa\xf7\xf1\x9e\x87\x86\x9f\x8b\x94\xf2\xd9\xd8\xd3\x89\x94\xb6\x00"
			};

        while(secret[i] != '\0'){
                secret_decode[i]=secret[i]^secret_key;  // XOR the secert key to get the plaintext
                i++;
        }
	clear_screen();
	kprint("Awesome You figured out how to call the win function.\nBut are you the right user?\nAnd do you know the secret code?\n\n");
	for (i=0;i<strlen(arg);i++){
		memset(decoded, '\0', 100);
		j=0;
                while(str[i][j] != '\0' && i<10) {
			decoded[j]=str[i][j] ^ arg[i] ^ (uid+96); 
			j++;
                }
		if(arg[i]==secret_decode[i] && uid == uids[i % 3]){ 
			kprint_green(decoded);
		} else {
			kprint(decoded);
		}
		kprint_newline();
        }
	kprint_newline();

}
 
void add_new_command(char *name, char* description, void *function)
   {
     if(NumberOfCommands + 1 < MAX_COMMANDS)
       {
               NumberOfCommands++;
               CommandTable[NumberOfCommands].name = name;
               CommandTable[NumberOfCommands].description = description;
               CommandTable[NumberOfCommands].function = function;
       }
      return;
  }
int findCommand(char* command)
   {
     int i;
     int ret;
 
     for(i = 0; i < NumberOfCommands + 1; i++)
       {
           ret = strcmp(command, CommandTable[i].name);
         
           if(ret == 0)
             {
               return i;
             }
	}
       return -1;
   }

 
 void init_shell()
   {
	add_new_command("CATCH-ALL", "Catch-All", empty_command);
	add_new_command("help", "Ha. You need Help!", help_command);
	add_new_command("clear", "Clear the Screen", clear_command);
	add_new_command("guess", "Make a Guess", guess_command);
	add_new_command("func", "Print Function Table", func_command);
	add_new_command("gp", "Get current Privlidges", gp_command);
	add_new_command("bu", "Become User XXX", bu_command);
	add_new_command("call", "call function number XXX", call_command);
	add_new_command("halt", "SHUT IT DOWN!", halt_command);
	add_new_command("WIN", "Call this function and Win the game", win_command);
     
   }
 char * parseinput(char * input){
	int i=0;
	
	while(input[i] != '\0'){
		if(input[i]==' '){
			input[i]='\0';
		}
		i++;
	}
	return input;
 }
 
 void shell(){
	int index;
	int j;
	char *input;
	char *argument;
	void (*command_function)(char *);

	while(end!=1){

		kprint("BUSTANUT ");
		kprint( uid == 0 ? "# " : "$ ");
		while(done_typing==0){
			//loop doing nothing until the user presses return	
		}
		done_typing=0;
		input_buffer=parseinput(input_buffer);
		input=input_buffer;
		argument=input_buffer+strlen(input)+1;
		index=findCommand(input);
		if(index < 0){
			empty_command(argument);
		} else {
			command_function = CommandTable[index].function;
			command_function(argument);
		}
		input_buffer=memset(input_buffer, '\0' , MAXSIZE);
		input_buf_loc=0;
	}
	
return;
}
