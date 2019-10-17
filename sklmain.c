#include "uart.h"
#include "util.h"


#define BUFF_SIZE 100
unsigned char buffer[BUFF_SIZE];
unsigned char msg[] = "Thank You";

#define MENUCMD_EMPTY      0x00
#define MENUCMD_BKSP_ABRT  0x01
#define MENUCMD_UNKNOWN    0X02
#define MENUCMD_ECHO       0x03
#define MENUCMD_BOOTLDR    0x04
#define MENUCMD_MARCO      0x05

#if 1
unsigned int menucmdlistsz = 6;
const char* menucmdlist[] = 
{
  "\0", //empty
  "\0", //abort
  "\0", //unknown
  "echo\0",
  "bootldr\0",
  "marco\0"
};
#endif

void sklmain(void)
{
  //unsigned int chars_read = 0;
  init_uart();

  while(1)
  {
    readln(buffer, BUFF_SIZE);
    menumain(buffer, chars_read);
    writeln(msg, 9);
  }
}

#if 1

unsigned int rdmnucmd(unsigned char* buffer, unsigned int bufsize)
{
  int i;
  int is_equal = 0;
  for(i = 3; i < menucmdlistsz; i++)
  {
    cmdlen = sklstrlen(menucmdlist[i]);
    
    is_equal = sklstrncmp(menucmdlist[i], buffer, cmdlen);
    if(is_equal)
    {
      return i;
    }
  }
  return MENUCMD_UNKNOWN;
}

void menumain(unsigned char* buffer, unsigned int bufsize)
{
  unsigned int menu_cmd;

  menu_cmd = rdmnucmd(buffer, bufsize);

  switch(menu_cmd)
  {
    case MENUCMD_MARCO:
      writeln("polo");
      break;
    case MENUCMD_ECHO:
    case MENUCMD_EMPTY:
    case MENUCMD_BOOTLDR:
    case MENUCMD_UNKNOWN:
    case MENUCMD_BKSP_ABRT:
      writeln("unknown command");
      break;
  }
}
}

#endif
