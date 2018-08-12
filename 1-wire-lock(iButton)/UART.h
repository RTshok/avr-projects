#define F_CPU 16000000UL
#include<avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
# define BUFFER (64)
# define USART_BAUDRATE 9600
/* The formula that calculates number that we need to put into register */
# define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16UL))) - 1)

/**
* uart_transmit - transmiting the data to the COM-port
* @data - the data which you want to be transmitted
*/

void uart_transmit( unsigned char data );

/**
* uart_receive - receiving a char from the COM-port.
*
*/

unsigned char uart_receive(void);

/**
* uart_init - setting the registers to transmit and the receive data. Also configurates the baud rate, the size of package and stop-bits.
*/

void uart_init(void);

/**
* uart_print - transmitting the string to the COM-port.
* @data - the string which we want to transmit
*/

void uart_print(  char data[] );

/**
* uart_flush - used dummy variable to flush the receiving buffer.
*
*/

void uart_flush( void );

/**
* uart_read_string - reading the string from COM-port.
* @DATA - the string that has come from COM-port
*/

void uart_read_string(char *DATA);

/**
* comp_func - compare two strings, if they are equal then return true,if not - false.
* @str1 - the string to compare
* @str2 - the string to compare
*/

bool comp_func (char *str1,char *str2);
