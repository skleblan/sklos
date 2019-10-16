
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include "uart.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270
//9600 baud = 3254

//#define LINE_BUFFER_SIZE 100

#define INIT_MSG_SIZE 27
unsigned char init_msg[] = "UART has been initialized\r\n";

void uart_putchar ( unsigned char c )
{
  unsigned int word;
  word = (unsigned int)c;
  while(1)
  {
    if(GET32(AUX_MU_LSR_REG)&0x20) break;
  }
  PUT32(AUX_MU_IO_REG, word);
}

unsigned char uart_getchar ( void )
{
	unsigned int ra;
	unsigned char retval;
	retval = 0;
	ra = 0;
	if(GET32(AUX_MU_LSR_REG)&0x01)
	  ra=GET32(AUX_MU_IO_REG);
        retval = ra & 0xFF;
	return retval;
}

void init_uart ( void )
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,3254);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,3);

    writeln(init_msg, INIT_MSG_SIZE);
}

unsigned int readln(char * data, unsigned int size)
{
  unsigned int idx = 0;
  unsigned char temp;

  temp = uart_getchar();
  while(idx < size - 1 && 
    temp != '\n' && temp != '\r')
  {
    if(temp != 0)
    {
      uart_putchar(temp); //echo's what the user is typing
      data[idx] = (signed char)temp;
      idx++;
    }
    temp = uart_getchar();
  }
  data[idx] = 0;
  idx++;
  uart_putchar('\r');
  uart_putchar('\n');
  return idx;
}

//preparing for ability to send update firmware via RS-232
unsigned int read(unsigned char* data, unsigned int buf_size)
{
  unsigned int idx = 0;
  unsigned char temp;

  temp = uart_getchar();
  while(idx < buf_size)
  {
    data[idx] = temp;
    idx++;
    temp = uart_getchar();
  }
  return idx;
}

void writeln( unsigned char * data, unsigned int size)
{
  unsigned int idx = 0;
  while(idx < size)
  {
    uart_putchar(data[idx]);
    idx++;
  }
  uart_putchar('\r');
  uart_putchar('\n');
}

