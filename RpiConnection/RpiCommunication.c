/*
 * RpiConnection.c
 *
 * Created: 28/01/2022 11:21:23
 * Author : blue
 */ 

#include <avr/io.h>
#include "RpiCommunication.h"

//	setup for USART
//	8 bit frames, 9600 Baud, 1 stop bit, no flow control,interrupts enabled
void RPI_usart_setup(void){
	
	//	set RX(PD0) as input, TX(PD1) as output and high
	DDRD &= ~(1 << DDRD0);
	DDRD |= (1 << DDRD1);
	PORTD |= (1 << PORTD1);
	//	normal speed
	UCSR0A &= ~(1 << U2X0);
	//	set baud rate to 9600 (23 for the register)
	UBRR0 = 23;
	//	mode select: asynchronous USART
	UCSR0C &= ~((1 << UMSEL00) | (1 << UMSEL01));
	//	No parity
	UCSR0C &= ~((1 << UPM00) | (1 << UPM01));
	//	1 stop bit
	UCSR0C &= ~(1 << USBS0);
	//	8 databits
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |=((1 << UCSZ00) | (1 << UCSZ01));
	//	we want to trigger an interrupt when the receive buffer is full
	UCSR0B |= (1 << RXCIE0);
	//	enable receive and transmit port
	UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));
}

//	send a single char to the rpi
void RPI_send_char(char msg){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = msg;
}

//	send a string to the rpi
void RPI_send_string(char msg[]){
	//	variable to loop over the character array
	uint16_t index = 0;
	while(index < 64){
		//	reached end of the string
		if(msg[index] == '\0'){
			RPI_send_char(msg[index]);
			return;
		}
		
		RPI_send_char(msg[index]);
		index++;
	}
}

void RPI_receive_string(char *response[]){
	char received_char;
	
	for(unsigned char x = 0; x < 64; x++){
		received_char = RPI_receive_char();
		if(received_char == '\0'){
			*response[x] = '\0';
			break;
		}
		*response[x] = received_char;
	}
}

char RPI_receive_char(void){
	while(!(UCSR0A & (1 << UDRE0)));
	return UDR0;
}