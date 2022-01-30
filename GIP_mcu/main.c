/*
* GIP_mcu.c
*
* Created: 28/01/2022 7:58:52
* Author : blue
*/

#define F_CPU 3686400L


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <RpiCommunication.h>
#include <HallSensor.h>

#define HANDBRAKE PORTC7

char resp_char = 255;

enum InstructionCodes ICodes;
enum ResponseCodes RCodes;

Battery battery = {
	1,
	0,
	0,		//	
	100,	//	dummy value for total capacity
	50,		//	dummy value	for remaining battery capacity
	0,		//	no charging current yet
};

int main(void)
{
	RPI_usart_setup();	//	initalize UART0
	hall_timer_setup();	//	initialize timer
	hall_ADC_setup();	//	initialize adc
	DDRC = 0xFF;		//	portc all outputs
	char buffer[64];	//	send buffer
	sei();				//	set Global Interrupt Enable '1'

	while (1)
	{

	if(battery.AdcActivated && !(ADCSRA & (1 << ADSC))){	//	ADC has been activated, and is no longer active, so there is a result
		hall_interpret_result(&battery, hall_get_result());
		sprintf(buffer, "%lu",battery.RemainingBatteryCapacity);
		RPI_send_string(buffer);
		battery.AdcActivated = 0;
		PORTC ^= (1 << PORTC6);
	}

		if(resp_char != 255){	//	An instruction arrived
			switch(resp_char){
				case CreateConnection:
				RPI_send_char((char)CreatedConnection);
				resp_char = 255;
				break;
				case CloseConnection:
				RPI_send_char((char)ClosedConnection);
				resp_char = 255;
				break;
				case SendDataFromMemory:
				RPI_send_char((char)SendingMemData);
				resp_char = 255;
				break;
				case UnlockHandbrake:
				PORTC &= ~(1 << HANDBRAKE);
				RPI_send_char((char)HandbrakeUnlocked);
				resp_char = 255;
				break;
				case LockHandbrake:
				PORTC |= (1 << HANDBRAKE);
				RPI_send_char((char)HandbrakeLocked);
				resp_char = 255;
				break;
				case StartCalibration:
				RPI_send_char((char)CalibrationStarted);
				resp_char = 255;
				break;
				case StopCalibration:
				RPI_send_char((char)CalibrationStopped);
				resp_char = 255;
				break;
				case GetCalibrationResult:
				RPI_send_char((char)SendingCalibrationResult);
				resp_char = 255;
				break;
				case GetTotalCapacity:
				RPI_send_char((char)SendingTotalCapacity);
				resp_char = 255;
				break;
				case GetRemainingBatteryCharge:
				RPI_send_char((char)SendingRemainingBatteryCharge);
				sprintf(buffer, "%lu",battery.RemainingBatteryCapacity);
				RPI_send_string(buffer);
				resp_char = 255;
				break;
				default:
				RPI_send_char((char)Nop);
				resp_char = 255;
			}
		}

			
	}
	return 1;
}

ISR(USART0_RX_vect){
	resp_char = UDR0;
}

ISR(TIMER0_COMPA_vect){
	static unsigned char cntr = 0;	//	counter that will be used to count the number of times we passed 25ms

	if(cntr < 20){
		cntr++;
		
	} else {
		cntr = 0;
		ADCSRA |= (1 << ADSC);	//	activate ADC
		battery.AdcActivated = 1;
	}

	
}

