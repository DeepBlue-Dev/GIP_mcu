/*
 * GIP_mcu.c
 *
 * Created: 28/01/2022 7:58:52
 * Author : blue
 */ 

#define F_CPU 3686400L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <RpiCommunication.h>
#include <HallSensor.h>

int main(void)
{    
	RPI_usart_setup();
	hall_ADC_setup();



	RPI_send_char('S');
	char buffer[16];
    while (1) 
    {
		
		sprintf(buffer, "%d", (unsigned int)hall_get_current());
		RPI_send_string(buffer);
		_delay_ms(900);
		
    }
}

