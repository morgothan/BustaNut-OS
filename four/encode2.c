#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
        char *str[] =   {
                                "First Letter down as user: User - FLAG(1)=\"I\"\n",                    //xor c xor 100
                                "Second Letter down as userid: Root - FLAG(2)=\"Wrote\"\n",             //xor o xor 0
                                "Third Letter down as userid: Administrator - FLAG(3)=\"A\"\n",         //xor c xor 500
                                "Now you are getting some place - FLAG(4)=\"Kernel\"\n",                //xor k xor 100
                                "Back to user: User? - FLAG(5)=\"With\"\n",                             //xor s xor 0
                                "Oh I get it, its a loop - FLAG(6)=\"A\"\n",	                        //xor s xor 500
                                "But where did the UIDs come from? - FLAG(7)=\"Shell\"\n",              //xor u xor 100
                                "Windows and Unix? - FLAG(8)=\"What've\"\n",                            //xor c xor 0
                                "So close I can almost taste it - FLAG(9)=\"You\"\n",                	//xor e xor 500
                                "Last of the flags - FLAG(10)=\"Done?\"\n",                             //xor r xor 100
                        };

	char *enc;
	char *key = "cocksucker";
	unsigned int i = 0;
	unsigned int j = 0;
	int uid[3] = {100,0,500};

	for (i=0;i<10;i++){
		enc = malloc(strlen(str[i]));
		for(j=0;j<= strlen(str[i]);j++) {
			enc[j] = str[i][j] ^ key[i] ^ (uid[i % 3]+96);
			printf("\\x%02x", enc[j] & 0xff);
		}
		free(enc);
		putchar('\n');
	}	
	return;
}
