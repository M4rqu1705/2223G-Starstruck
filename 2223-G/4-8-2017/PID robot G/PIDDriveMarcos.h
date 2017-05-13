//#pragma config(Sensor, dgtl4,  Right_Encoder,  sensorQuadEncoder)
//#pragma config(Sensor, dgtl6,  Left_Encoder,   sensorQuadEncoder)
//#pragma config(Motor,  port2,           DriveRF,       tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, dgtl4)
//#pragma config(Motor,  port4,           DriveRB,       tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, dgtl4)
//#pragma config(Motor,  port6,           DriveLF,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, encoderPort, dgtl6)
//#pragma config(Motor,  port8,           DriveLB,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, encoderPort, dgtl6)

#ifndef PIDDriveMarcos.h
#define PIDDriveMarcos.h

#pragma systemFile

//PID constants
#define Kp 1
#define Ki 0.025
#define Kd 3

//Integral limit
#define integralLimit 25

int errorLeft = 0, lastErrorLeft = 0, integralLeft = 0, derivativeLeft = 0, outputLeft = 0;
int errorRight = 0, lastErrorRight = 0, integralRight = 0, derivativeRight = 0, outputRight = 0;

void move(string *direction, byte speed, int pulses){

	//Preset all values to 0
	errorLeft = 0; lastErrorLeft = 0; integralLeft = 0; derivativeLeft = 0; outputLeft = 0;
	errorRight = 0; lastErrorRight = 0; integralRight = 0; derivativeRight = 0; outputRight = 0;
	SensorValue[Left_Encoder] = 0;	SensorValue[Right_Encoder] = 0;

	//While distance not reached
	while(abs(SensorValue[Left_Encoder]) < pulses || abs(SensorValue[Right_Encoder]) < pulses){

		//Calculate error
		errorLeft = pulses - abs(SensorValue[Left_Encoder]);
		errorRight = pulses - abs(SensorValue[Right_Encoder]);

		//Calculate the integral only if it is less than its limit and 9/10 of the total distance travelled
		integralLeft += (integralLeft < integralLimit && integralLeft > -integralLimit && abs(SensorValue[Left_Encoder]) < pulses / 10) ? errorLeft : 0;
		integralRight += (integralRight < integralLimit && integralLeft > -integralLimit  && abs(SensorValue[Right_Encoder]) < pulses / 10) ? errorRight : 0;

		//Calculate derivative
		derivativeLeft = errorLeft - lastErrorLeft;	derivativeRight = errorRight - lastErrorRight;

		//Retrieve  the last error
		lastErrorLeft = errorLeft;	lastErrorRight = errorRight;

		//Calculate the ouput using the PID constants and the calculated error, integral and derivative
		outputLeft = (Kp * errorLeft) + (Ki * integralLeft) + (Kd * derivativeLeft);
		outputRight = (Kp * errorRight) + (Ki * integralRight) + (Kd * derivativeRight);

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

		else if (*direction ==  "left" || *direction ==  "rotateLeft" || *direction ==  "pointTurnLeft" ||
			*direction ==  "pointLeft" || *direction ==  "pLeft" || *direction ==  "pointL" || *direction ==  "pL"){
			motor[DriveLF] = -(outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1));
			motor[DriveLB] = -(outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1));
			motor[DriveRF] = outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
			motor[DriveRB] = outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
		}

		else if (*direction ==  "right" || *direction ==  "rotateRight" || *direction ==  "pointTurnRight" ||
			*direction ==  "pointRight" || *direction ==  "pRight" || *direction ==  "pointR" || *direction ==  "pR"){
			motor[DriveLF] = outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
			motor[DriveLB] = outputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
			motor[DriveRF] = -(outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1));
			motor[DriveRB] = -(outputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1));
		}
		wait1Msec(20);
	}
	wait1Msec(50);
}

#endif
