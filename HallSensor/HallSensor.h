/*
 * HallSensor.h
 *
 * Created: 28/01/2022 14:10:30
 *  Author: arthu
 */ 


#ifndef HALLSENSOR_H_
#define HALLSENSOR_H_





#endif /* HALLSENSOR_H_ */

extern struct Hall_Battery_Status{
	unsigned char IsMeasuring:1,	//	hall sensor is counting
	unsigned char IsCharging:1,		//	battery is charging -> 1, battery is depleting -> 0
	unsigned long Full_capacity,
	unsigned long RemainingBatteryCapacity,	//	the remaining charge in the battery

}Battery;

extern uint16_t hall_get_current(void);
extern void hall_ADC_setup(void);