/*
 * HallSensor.c
 *
 * Created: 28/01/2022 11:09:50
 * Author : blue
 */ 

#include <avr/io.h>
#include "HallSensor.h"

//	uses timer0 to generate an interrupt every 25 ms
void hall_timer_setup(void){
	//	timer0 mode 2 (CTC)
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
	TCCR0A |= (1 << WGM01);
	//	prescaler of 1024.
	TCCR0B &= ~(1 << CS01);
	TCCR0B |= ((1 << CS00) | (1 << CS02));
	//	activate interrupt on compare match
	TIMSK0 |= (1 << OCIE0A);
	//	set compare value ( (0.025s) / (1 / 3686400) = 92160 => 92160 / 1024 = 90 - 1 )
	OCR0A = 89;

}

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

uint16_t hall_get_result(void){
	return ((((uint16_t)ADCH) << 8) | ADCL);
}

//	get the strength of the current.
uint16_t hall_get_current(void){
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADSC)));
	
	return hall_get_result();
}

void hall_interpret_result(Battery *p_battery, uint16_t result){
	
	if(result < 512){	//	if true: the battery is charging
		if(! p_battery->IsCharging){
			p_battery->IsCharging = 1;	//	battery is charging
			p_battery->ChargeCurrent = 0;	//	reset the current that went into the battery
			
		} else {
			if(p_battery->ChargeCurrent + result < 18446744073709551615ul)	//	overflow protectin
				p_battery->ChargeCurrent += result;
		}
	} else {
		if(p_battery->IsCharging){
			p_battery->IsCharging = 0;
			p_battery->RemainingBatteryCapacity = p_battery->ChargeCurrent;
			p_battery->ChargeCurrent = 0;
		} else {
			if(p_battery->RemainingBatteryCapacity - result >= 0){	
				p_battery->RemainingBatteryCapacity -= result;
			}
			
		}

	}

}





