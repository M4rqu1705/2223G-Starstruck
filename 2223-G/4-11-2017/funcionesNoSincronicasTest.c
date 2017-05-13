#pragma config(Sensor, in1,    Pot_Brazo,      sensorPotentiometer)
#pragma config(Sensor, in2,    Pot_Right,      sensorPotentiometer)
#pragma config(Sensor, in3,    Pot_Left,       sensorPotentiometer)
#pragma config(Sensor, in4,    Line_FollowR,   sensorLineFollower)
#pragma config(Sensor, in5,    Line_FollowL,   sensorLineFollower)
#pragma config(Sensor, in6,    gyrosensor,     sensorGyro)
#pragma config(Sensor, dgtl1,  Limit_Switch,   sensorTouch)
#pragma config(Sensor, dgtl4,  Right_Encoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  Left_Encoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           PinzaR,        tmotorVex393, openLoop)
#pragma config(Motor,  port2,           DriveRF,       tmotorVex393, PIDControl, encoder, encoderPort, dgtl4, 1000)
#pragma config(Motor,  port3,           BrazoRB,       tmotorVex393, openLoop)
#pragma config(Motor,  port4,           DriveRB,       tmotorVex393, PIDControl, encoder, encoderPort, dgtl4, 1000)
#pragma config(Motor,  port5,           BrazoRY,       tmotorVex393, openLoop)
#pragma config(Motor,  port6,           DriveLF,       tmotorVex393, PIDControl, reversed, encoder, encoderPort, dgtl6, 1000)
#pragma config(Motor,  port7,           BrazoLY,       tmotorVex393, openLoop)
#pragma config(Motor,  port8,           DriveLB,       tmotorVex393, PIDControl, reversed, encoder, encoderPort, dgtl6, 1000)
#pragma config(Motor,  port9,           BrazoLB,       tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port10,          PinzaL,        tmotorVex393, openLoop, reversed)

#include "funcionesNoSincronicas.h"

task main(){
	int pulses = 0, armPosition = 0, clawPosition = 0;

	//While the base has not reached its destination or the arm has reached its destination or the claw has reached its destination
	while((abs(SensorValue[Left_Encoder]) < abs(pulses) && abs(SensorValue[Right_Encoder]) < abs(pulses)) || SensorValue[Pot_Brazo] < armPosition
		|| (SensorValue[Pot_Left] > clawPosition && SensorValue[Pot_Right] > clawPosition)){
		moveBasePID(100, -1000);
		armPID(100, 1000, -10);
		clawPID(30, 700, 10);
		wait1Msec(16.67);
	}
	stopBase();
	stopArm();
	stopCLaw();
}
