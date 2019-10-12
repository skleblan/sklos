#ifndef __UART_H__
#define __UART_H__
void uart_putchar ( unsigned char c );
unsigned char uart_getchar ( void );
unsigned int read(unsigned char * data, unsigned int bufsize);
unsigned int readln(unsigned char * data, unsigned int bufsize);
void writeln(unsigned char * data, unsigned int size);
void init_uart ( void );
#endif
