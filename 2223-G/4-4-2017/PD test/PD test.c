#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           RightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          LeftMotor,     tmotorVex393_HBridge, openLoop, reversed)


float Kp = 1,Kd = 1000;
int lastErrorLeft = 0, lastErrorRight = 0,		//For now they will be 0
derivativeLeft, derivativeRight,							//No initialization needed
timeToWait = 20,															//After each cycle
outputLeft = 0, outputRight = 0,							//To be determined
errorBand=0,																	//Modify for precision
errorLeft = errorBand, errorRight = errorBand;//These values have to be equal or grater than errorBand !!!!

void moveForward(int speed = 100, long pulses = 360){																						//Use predetermined values of speed and pulses
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;
	for(int c = 1; SensorValue[rightEncoder]<=pulses || SensorValue[leftEncoder]<=pulses; c++){	//Only run if there is still distance needed to travel

		errorLeft = errorBand <= errorLeft ? pulses - SensorValue[leftEncoder] : 0;									//There is only error while value is bigger than errorBand
		errorRight = errorBand <= errorRight ? pulses - SensorValue[rightEncoder] : 0;							//There is only error while value is bigger than errorBand

		derivativeLeft = (lastErrorLeft-errorLeft)/(timeToWait*c);
		derivativeRight = (lastErrorRight-errorRight)/(timeToWait*c);

		outputLeft = (Kp*errorLeft) + (Kd*derivativeLeft);
		outputRight = (Kp*errorRight) + (Kd*derivativeRight);

		motor[LeftMotor] = outputLeft <= speed ? outputLeft : speed;
		motor[RightMotor] = outputRight <= speed ? outputRight : speed;

		lastErrorLeft = errorLeft;
		lastErrorRight = errorRight;

		datalogDataGroupStart();

		datalogAddValue(0, SensorValue[leftEncoder]);
		datalogAddValue(1,SensorValue[rightEncoder]);
		datalogDataGroupEnd();

		wait1Msec(timeToWait);
	}
	motor[LeftMotor] = -50;
	motor[RightMotor] = -50;
	wait1Msec(10);
	motor[LeftMotor] = 0;
	motor[RightMotor] = 0;
}

task main(){
	datalogClear();
	moveForward(127,360);

	for(int c = 0; c<100;c++){
		datalogDataGroupStart();
		datalogAddValue(0, SensorValue[leftEncoder]);
		datalogAddValue(1,SensorValue[rightEncoder]);
		datalogDataGroupEnd();
		wait1Msec(timeToWait);
	}
}
