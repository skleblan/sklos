#include "uart.h"

unsigned char buffer[100];
//volatile unsigned int buffer_size = 0;
unsigned char msg[] = "Thank You";

void sklmain(void)
{

  init_uart();

  while(1)
  {
    read_line( buffer );
    write_line( msg, 9 );
  }
}
