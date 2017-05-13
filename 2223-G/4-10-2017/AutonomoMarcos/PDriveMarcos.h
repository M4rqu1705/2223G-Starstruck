/*
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
*/
#ifndef PDriveMarcos.h
#define PDriveMarcos.h

#pragma systemFile

int errorLeft = 0, outputLeft = 0;
int errorRight = 0, outputRight = 0;

void move(string *direction, byte speed, int pulses){
	//P constant
	float Kp = 1;

	//Preset all values to 0
	errorLeft = 0; outputLeft = 0; errorRight = 0; outputRight = 0;	SensorValue[Left_Encoder] = 0;	SensorValue[Right_Encoder] = 0;

	//While distance not reached
	while(abs(SensorValue[Left_Encoder]) < pulses || abs(SensorValue[Right_Encoder]) < pulses){

		//Calculate error
		errorLeft = pulses - abs(SensorValue[Left_Encoder]);
		errorRight = pulses - abs(SensorValue[Right_Encoder]);

		//Calculate the ouput using the P constant and the calculated error
		outputLeft = Kp * errorLeft;
		outputRight = Kp * errorRight;

		//Make sure the output speed is less than the max speed
		if(outputLeft > speed) outputLeft = speed;
		else if (outputLeft < -speed) outputLeft = -speed;
		if(outputRight > speed) outputRight = speed;
		else if (outputRight < -speed) outputRight = -speed;

		//Move the base based on the direction desired
		if( *direction ==  "forward" || *direction ==  "front" || *direction ==  "f"){
			motor[DriveLF] = outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
			motor[DriveLB] = outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
			motor[DriveRF] = outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
			motor[DriveRB] = outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
		}

		else if (*direction ==  "backward" || *direction ==  "backwards" || *direction == "bacck" || *direction ==  "b"){
			motor[DriveLF] = -(outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1));
			motor[DriveLB] = -(outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1));
			motor[DriveRF] = -(outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1));
			motor[DriveRB] = -(outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1));
		}
		wait1Msec(20);
	}
	wait1Msec(50);
}

#endif
