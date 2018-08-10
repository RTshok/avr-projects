#include "UART.h"

// transmit a char to uart
void uart_transmit( unsigned char data )
{
	
    // wait for empty transmit buffer
    while ( ! ( UCSR1A & ( 1 << UDRE1 ) ) )
        ;
    
    // put data into buffer, sends data
    UDR1 = data;
}

// read a char from uart
unsigned char uart_receive(void)
{
	
    while (!( UCSR1A & ( 1 << RXC1) ))
        ;
    return UDR1;
}

// init uart
void uart_init(void)
{
    // set baud rate   
    unsigned int baud = BAUD_PRESCALE;
    
    UBRR1H = (unsigned char) (baud >> 8 );
    UBRR1L = (unsigned char)baud;
    
    // enable receiver and transmitter
    UCSR1B = ( 1 << RXEN1 ) | ( 1 << TXEN1 );
    
    // set frame format ( 8data, 2stop )
    UCSR1C = ( 1 << USBS1 ) | ( 3 << UCSZ10 );

}



// write a string to the uart
void uart_print(  char data[] )
{	
    uint8_t c = 0;
    
    for ( c = 0; c < strlen(data); c++ )
        uart_transmit(data[c]);
	

}
void uart_flush( void )
{
volatile unsigned char dummy;
while ( UCSR1A & (1<<RXC1) ) 
dummy = UDR1;
}
void uart_read_string(char * DATA){
uint8_t cnt = 0;
char tmp = 0;

while ((tmp = uart_receive())!=13){
DATA[cnt] = tmp;
cnt++;
}
DATA[cnt] = '\0';
uart_print(DATA);
uart_print("\n");
uart_flush();
}

bool comp_func (char *str1,char *str2){
bool if_equal = false;
for (uint8_t i = 0;i<BUFFER;i++)
{	
	if(str1[i]==str2[i])
	{	
		if(str1[i]=='\0')
		{
		if_equal = true;
		return if_equal;
		}
	}else
	break;
}
return if_equal;
}
