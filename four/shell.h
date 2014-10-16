 /* shell.h */
 #define MAX_COMMANDS 100
 
 typedef struct
   {
     char *name;
     char *description;
     void (*function)(char *);
   } command_table_t;


