#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ,               sensorQuadEncoder)
#pragma config(Motor,  port1,           LeftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, dgtl3)
#pragma config(Motor,  port10,          RightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, dgtl1)

#include "driverFunctions.h"

int buttonToggleState = 0, buttonPressed = 0;

task main(){
	while(true){
		if( vexRT[ Btn8R ] == 1 ){
			if(!buttonPressed){														//if buttonPressed was previously 0 when Btn8R pressed then
				buttonToggleState = 1 - buttonToggleState;		// change the toggle state
				buttonPressed = 1;														// buttonPressed = 1 so the toggle state doesnt change again
			}
		}
		else buttonPressed = 0;														// the button is not pressed so buttonPressed = 0

		if(buttonToggleState)	oppositeBase();
		else	normalBase();
	}
}
