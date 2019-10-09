#ifndef __UART_H__
#define __UART_H__
void uart_putchar ( unsigned char c );
unsigned char uart_getchar ( void );
unsigned int read_line( unsigned char * data );
void write_line( unsigned char * data, unsigned int size);
void init_uart ( void );
#endif
