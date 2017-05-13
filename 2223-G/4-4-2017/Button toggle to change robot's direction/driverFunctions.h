#ifndef driverFunctions.h
#define driverFunctions.h

#pragma systemFile

int threshold = 20;

void normalBase(){
	//make sure small movements of the joystick do not move the robot
	motor[LeftMotor] = (vexRT[Ch2]>threshold  || vexRT[Ch2]< -threshold || vexRT[Ch1]>threshold || vexRT[Ch1]< -threshold) ? vexRT[Ch2] - vexRT[Ch1] : 0;
	motor[RightMotor] = (vexRT[Ch2]>threshold  || vexRT[Ch2]< -threshold || vexRT[Ch1]>threshold || vexRT[Ch1]< -threshold) ? vexRT[Ch2] + vexRT[Ch1] : 0;
}
void oppositeBase(){
	//make sure small movements of the joystick do not move the robot
	motor[LeftMotor] = (vexRT[Ch2]>threshold  || vexRT[Ch2]< -threshold || vexRT[Ch1]>threshold || vexRT[Ch1]< -threshold) ? -vexRT[Ch2] - vexRT[Ch1] : 0;
	motor[RightMotor] = (vexRT[Ch2]>threshold  || vexRT[Ch2]< -threshold || vexRT[Ch1]>threshold || vexRT[Ch1]< -threshold) ? -vexRT[Ch2] + vexRT[Ch1] : 0;
}
#endif
