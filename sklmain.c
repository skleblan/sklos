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
    //menumain(buffer, chars_read);
    writeln(msg, 9);
  }
}

#if 0

unsigned int rdmnucmd(unsigned char* buffer, unsigned int bufsize)
{
  int i;
  int is_equal = 0;
  for(i = 3; i < menucmdlistsz; i++)
  {
    cmdlen = sklstrlen(menucmdlist[i]);
    
    is_equal = sklstrncmp(

void menumain(unsigned char* buffer, unsigned int bufsize)
{
}

#endif
