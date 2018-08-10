#define F_CPU 16000000UL
#include<avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
# define BUFFER (64)
# define USART_BAUDRATE 9600
# define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16UL))) - 1)
void uart_transmit( unsigned char data );
unsigned char uart_receive(void);
void uart_init(void);
void uart_print(  char data[] );
void uart_flush( void );
void uart_read_string(char *DATA);
bool comp_func (char *str1,char *str2);
