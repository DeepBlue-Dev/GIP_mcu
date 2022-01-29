/*
 * RpiCommunication.h
 *
 * Created: 28/01/2022 13:29:25
 *  Author: blue
 */ 


#ifndef RPICOMMUNICATION_H_
#define RPICOMMUNICATION_H_
#endif /* RPICOMMUNICATION_H_ */

enum InstructionCodes{
	nop,
	CreateConnection,
	CloseConnection,
	SendDataFromMemory,
	UnlockHandbrake,
	LockHandbrake,
	StartCalibration,
	StopCalibration,
	GetCalibrationResult,
	GetTotalCapacity,
	GetRemainingBatteryCharge,
};

enum ResponseCodes{
	Nop,
	CreatedConnection,
	ClosedConnection,
	SendingMemData,
	HandbrakeUnlocked,
	HandbrakeLocked,
	CalibrationStarted,
	CalibrationStopped,
	SendingCalibrationResult,
	SendingTotalCapacity,
	ConnectionError,
	SendingRemainingBatteryCharge,
};

extern void RPI_usart_setup(void);
extern void RPI_send_char(char msg);
extern void RPI_send_string(char msg[]);
extern void RPI_receive_string(char *response[]);
extern char RPI_receive_char(void);
