/*
 * HallSensor.h
 *
 * Created: 28/01/2022 14:10:30
 *  Author: arthu
 */ 


#ifndef HALLSENSOR_H_
#define HALLSENSOR_H_

typedef struct Hall_Battery_Status{
	unsigned char IsMeasuring:1;	//	hall sensor is counting
	unsigned char IsCharging:1;		//	battery is charging -> 1, battery is depleting -> 0
	unsigned char AdcActivated:1;	//	is '1' when adc has been activated
	unsigned long Full_capacity;	//	Measured full capacity of the battery
	unsigned long RemainingBatteryCapacity;	//	the remaining charge in the battery
	unsigned long ChargeCurrent;	//	The charge current.

}Battery;


#endif /* HALLSENSOR_H_ */

extern void hall_timer_setup(void);
extern uint16_t hall_get_current(void);
extern void hall_ADC_setup(void);
extern void hall_interpret_result(Battery *p_battery, uint16_t result);
extern uint16_t hall_get_result(void);