#include "uart.h"
#include "util.h"
#include "vectors.h"

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028

#define BUFF_SIZE 100
unsigned char buffer[BUFF_SIZE];
unsigned char msg[] = "Thank You";

#define MENUCMD_EMPTY      0x00
#define MENUCMD_BKSP_ABRT  0x01
#define MENUCMD_UNKNOWN    0X02
#define MENUCMD_ECHO       0x03
#define MENUCMD_BOOTLDR    0x04
#define MENUCMD_MARCO      0x05
#define MENUCMD_LED        0x06

unsigned int menucmdlistsz = 7;
#define MAX_CMD_STRING_LENGTH 0xF
const char* menucmdlist[] = 
{
  "\0", //empty
  "\0", //abort
  "\0", //unknown
  "echo\0",
  "bootldr\0",
  "marco\0",
  "led\0"
};

char led_status = 0;

void init_led(void)
{
  unsigned int ra;

  ra=GET32(GPFSEL1);
  ra&=~(7<<18);
  ra|=1<<18;
  PUT32(GPFSEL1,ra);

  writeln("led init complete", 17);
}

void toggle_led(void)
{
  led_status = ~led_status;
  if(led_status)
  {
    PUT32(GPSET0,1<<16);
    writeln("led off", 7);
  }
  else
  {
    PUT32(GPCLR0,1<<16);
    writeln("led on", 6);
  }
}

unsigned int rdmnucmd(char* buffer, unsigned int bufsize)
{
  int i;
  int is_equal = 0;
  int cmdlen = 0;

  for(i = 3; i < menucmdlistsz; i++)
  {
    cmdlen = strlen(menucmdlist[i], MAX_CMD_STRING_LENGTH);
    
    is_equal = strncmp(menucmdlist[i], buffer, cmdlen);
    if(is_equal == 0)
    {
      return i;
    }
  }
  return MENUCMD_UNKNOWN;
}

void menumain(char* buffer, unsigned int bufsize)
{
  unsigned int menu_cmd;

  menu_cmd = rdmnucmd(buffer, bufsize);

  switch(menu_cmd)
  {
    case MENUCMD_MARCO:
      writeln("polo", 4);
      break;
    case MENUCMD_LED:
      toggle_led();
      break;
    case MENUCMD_ECHO:
    case MENUCMD_EMPTY:
    case MENUCMD_BOOTLDR:
    case MENUCMD_UNKNOWN:
    case MENUCMD_BKSP_ABRT:
      writeln("unknown cmd", 11);
      break;
  }
}

void sklmain(void)
{
  unsigned int chars_read = 0;
  init_uart();
  init_led();

  while(1)
  {
    chars_read = readln((char*)buffer, BUFF_SIZE);
    menumain((char*)buffer, chars_read);
    writeln((char*)msg, 9);
  }
}


