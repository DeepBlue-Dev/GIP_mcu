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

struct Battery battery = {
	0,
	0,
	100,	//	dummy value
	50,		//	dummy value
};

int main(void)
{
	RPI_usart_setup();
	hall_ADC_setup();
	DDRC = 0xFF;


	char buffer[16];	//	send buffer
	sei();

	while (1)
	{
		switch(resp_char){
			case 255:
				break;
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
				resp_char = 255;
				break;
			default:
				RPI_send_char((char)Nop);
				resp_char = 255;
		}
		
		
		
	}
	return 1;
}

ISR(USART0_RX_vect){
	resp_char = UDR0;
}

