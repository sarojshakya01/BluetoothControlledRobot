/*
 * BT_controlled_Robot.c
 *
 * Created: 11/6/2015 5:38:30 PM
 *  Author: User
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define MOTOR_PORT PORTD
#define MOTOR_DDR DDRD

unsigned char received_char;

void port_initialize()
{
	MOTOR_DDR = 0xFF;
	MOTOR_PORT = 0x00;
}

void usart_initialize()//USART initialization//
{
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE); //enable tx and rx pin
	UCSRC = (1<<UCSZ0) | (1<<UCSZ1) | (1<<URSEL); //character size 8, 1 stop bit and reg select bit = 1
	UBRRL = 0x67; //baud rate 9600
}
void usart_send_char(unsigned char txdata)//Function to send single character serially//
{
	while(!(UCSRA&(1<<UDRE)));
	UDR = txdata;
}

void usart_send_string(char *str)//Function to send string serially//
{
	unsigned char i=0;
	while(str[i] != 0)
	{
		usart_send_char(str[i]);
		i++;
		_delay_ms(5);
	}
}

unsigned char usart_receive_char()
{
	while( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void BOT_MOVE_FORWARD(double speed)
{
	MOTOR_PORT &= 0b00000000;
	if (speed == 100) MOTOR_PORT |= 0b11000000; 
	else if (speed == 5)
	{	
		MOTOR_PORT |= 0b11000000;
		_delay_ms(5);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
	else if (speed == 12)
	{
		MOTOR_PORT |= 0b11000000;
		_delay_ms(12);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
}
void BOT_MOVE_BACKWARD(double speed)
{
	MOTOR_PORT &= 0b00000000;
	if (speed == 100) MOTOR_PORT |= 0b00110000; 
	else if (speed == 5)
	{
		MOTOR_PORT |= 0b00110000;
		_delay_ms(5);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
	else if (speed == 12)
	{
		MOTOR_PORT |= 0b00110000;
		_delay_ms(12);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
}
void BOT_MOVE_RIGHT(unsigned char speed)
{
	MOTOR_PORT &= 0b00000000;
	if (speed == 100) MOTOR_PORT |= 0b10010000;
	else if (speed == 5)
	{
		MOTOR_PORT |= 0b10010000;
		_delay_ms(5);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
	else if (speed == 12)
	{
		MOTOR_PORT |= 0b10010000;
		_delay_ms(12);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
}
void BOT_MOVE_LEFT(unsigned char speed)
{
	MOTOR_PORT &= 0b00000000;
	if (speed == 100) MOTOR_PORT |= 0b01100000;
	else if (speed == 5)
	{
		MOTOR_PORT |= 0b01100000;
		_delay_ms(5);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
	else if (speed == 12)
	{
		MOTOR_PORT |= 0b01100000;
		_delay_ms(12);
		MOTOR_PORT &= 0b00000000;
		_delay_ms(5);
	}
}

void BOT_STOP()
{
	MOTOR_PORT &= 0x03;
}
int main(void)
{
	port_initialize();
	usart_initialize();
	_delay_ms(10);
	//sei();
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	OCR1A = 255;
	OCR1B = 255;
	usart_send_string("A");
	while(1)
	{
		received_char = usart_receive_char();
		if (received_char == 'f')
		{
			BOT_MOVE_FORWARD(5);
		}
		if (received_char == 'g')
		{
			BOT_MOVE_FORWARD(12);
		}
		else if (received_char == 'h')
		{
			BOT_MOVE_FORWARD(100);
		}
		
		else if (received_char == 'b')
		{
			BOT_MOVE_BACKWARD(5);
		}
		else if (received_char == 'c')
		{
			BOT_MOVE_BACKWARD(12);
		}
		else if (received_char == 'd')
		{
			BOT_MOVE_BACKWARD(100);
		}
		else if (received_char == 'l')
		{
			BOT_MOVE_LEFT(5);
		}
		else if (received_char == 'm')
		{
			BOT_MOVE_LEFT(12);
		}
		else if (received_char == 'n')
		{
			BOT_MOVE_LEFT(100);
		}
		else if (received_char == 'r')
		{
			BOT_MOVE_RIGHT(5);
		}
		else if (received_char == 's')
		{
			BOT_MOVE_RIGHT(12);
		}
		else if (received_char == 't')
		{
			BOT_MOVE_RIGHT(100);
		}
		else if (received_char == 'z')
		{
			BOT_STOP();
		}
		//sei();
	}
	
}
/*
ISR(USART_RXC_vect)
{
	received_char = UDR;
	if (received_char == 'a')
	{
		cli();
		BOT_MOVE_FORWARD(0);
	}
	if (received_char == 'A')
	{
		cli();
		BOT_MOVE_BACKWARD(0);
	}
	if (received_char == 'f')
	{
		cli();
		BOT_MOVE_LEFT(0);
	}
	if (received_char == 'l')
	{
		cli();
		BOT_MOVE_RIGHT(0);
	}
	if (received_char == 'F')
	{
		cli();
		BOT_STOP();
	}
	if (received_char == 'L')
	{
		BOT_STOP();
	}
	else
	{
		cli();
		sei();
	}
}


int main(void)
{
	port_initialize();
	usart_initialize();
	sei();
	usart_send_string("A");
	//TCCR1A = 0x00;
	//TCCR1B = 0x00;
	//OCR1A = 255;
	//OCR1B = 255;
	while(1)
    {
		MOTOR_PORT = 0x55;
		_delay_ms(1000);
		MOTOR_PORT = 0xAA;
		_delay_ms(1000);
		sei();
    }
}

ISR(USART_RXC_vect)
{
	received_char = UDR;
	if (received_char == 'f')
	{
		//cli();
		BOT_MOVE_FORWARD(120);
	}
	if (received_char == 'g')
	{
		cli();
		BOT_MOVE_FORWARD(60);
	}
	else if (received_char == 'h')
	{
		cli();
		BOT_MOVE_FORWARD(0);
	}
	
	else if (received_char == 'b')
	{
		cli();
		BOT_MOVE_BACKWARD(120);
	}
	else if (received_char == 'c')
	{
		cli();
		BOT_MOVE_BACKWARD(60);
	}
	else if (received_char == 'd')
	{
		cli();
		BOT_MOVE_BACKWARD(0);
	}
	else if (received_char == 'l')
	{
		cli();
		BOT_MOVE_LEFT(120);
	}
	else if (received_char == 'm')
	{
		cli();
		BOT_MOVE_LEFT(60);
	}
	else if (received_char == 'n')
	{
		cli();
		BOT_MOVE_LEFT(0);
	}
	else if (received_char == 'r')
	{
		cli();
		BOT_MOVE_RIGHT(120);
	}
	else if (received_char == 's')
	{
		cli();
		BOT_MOVE_RIGHT(60);
	}
	else if (received_char == 't')
	{
		cli();
		BOT_MOVE_RIGHT(0);
	}
	else if (received_char == 'z')
	{
		cli();
		BOT_MOVE_RIGHT(0);
	}
	else
	{
		cli();
		sei();
	}	
}*/
