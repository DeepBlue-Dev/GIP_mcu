/*
 * HallSensor.c
 *
 * Created: 28/01/2022 11:09:50
 * Author : blue
 */ 

#include <avr/io.h>
#include "HallSensor.h"

//	setup for the ADC for the Hall Sensor
//	freerunning mode, no interrupts, ps: 32, ADC0, digital input disabled
void hall_ADC_setup(void){
	//	use AVCC external voltage (VCC will be attached to it)
	ADMUX &= ~((1 << REFS1));
	ADMUX |= (1 << REFS0);
	//	right adjusted result, we will use the full 10-bits
	ADMUX &= ~(1 << ADLAR);
	//	use single ended input ADC0
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4));
	//	Prescaler (32) for the speed of the conversion.
	ADCSRA |= ((1 << ADPS0) | (1 << ADPS2));
	ADCSRA &= ~(1 << ADPS1);
	//	enable freerunning, we don't want auto triggers
	ADCSRA &= ~(1 << ADATE);
	//	disable the input buffer on the pin we want to use for ADC (pina0)
	DIDR0 |= (1 << ADC0D);
	//	enable adc
	ADCSRA |= (1 << ADEN);
}

//	get the strength of the current.
uint16_t hall_get_current(void){
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADSC)));
	return (((uint16_t)ADCH << 8) | ADCL);
}



