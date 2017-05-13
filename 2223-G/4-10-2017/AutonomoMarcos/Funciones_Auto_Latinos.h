#ifndef Funciones_Auto_Latinos.h
#define Funciones_Auto_Latinos.h

#include "PDriveMarcos.h"

int fixTimerValue (float rawSeconds){
	int miliseconds = rawSeconds*1000;
	if (miliseconds < 250)	miliseconds = 250;
	return miliseconds;
}

void CierraPinzaPot(int target, int power){
	int power1;
	int power2;

	if (SensorValue(Pot_Left) > target) power2 = power;
	else power2 = 0;

	if (SensorValue(Pot_Right) > target) power1 = power;
	else power1 = 0;
	while (SensorValue(Pot_Right) > target || SensorValue(Pot_Left) > target){
		motor[port1] = power1;
		motor[port10] = power2;

		if (SensorValue(Pot_Left) > target) power2 = power;
		else power2 = -2;

		if (SensorValue(Pot_Right) > target) power1 = power;
		else power1 = -2;
	}
	motor[port1] = -2;
	motor[port10] = -2;
	wait1Msec(40);
	motor[port1] = -0;
	motor[port10] = -0;
}

void AbrePinzaPot(int target, int power){
	int power1;
	int power2;

	if (SensorValue(Pot_Left) < target)
	{
		power2 = power;
	}
	else
	{
		power2 = 0;
	}
	if (SensorValue(Pot_Right) < target)
	{
		power1 = power;
	}
	else
	{
		power1 = 0;
	}
	while (SensorValue(Pot_Right) < target || SensorValue(Pot_Left) < target)
	{
		motor[port1] = -power1;
		motor[port10] = -power2;

		if (SensorValue(Pot_Left) < target)
		{
			power2 = power;
		}
		else
		{
			power2 = 2;
		}
		if (SensorValue(Pot_Right) < target)
		{
			power1 = power;
		}
		else
		{
			power1 = 2;
		}
	}
	motor[port1] = 2;
	motor[port10] = 2;
	wait1Msec(40);
	motor[port1] = -0;
	motor[port10] = -0;
}

void BW(int power){
	motor[port2] = power;
	motor[port4] = power;
	motor[port6] = power;
	motor[port8] = power;
}

void FW(int power){
	motor[port2] = -power;
	motor[port4] = -power;
	motor[port6] = -power;
	motor[port8] = -power;
}

void StopDrive(){
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}

void LineBW(int power){
	while(SensorValue[Line_FollowR] > 1500 || SensorValue[Line_FollowL] > 1500){
		motor[port2] = -power;
		motor[port4] = -power;
		motor[port6] = -power;
		motor[port8] = -power;

		while(SensorValue[Line_FollowR] < 1500 && SensorValue[Line_FollowL] > 1500){
			motor[port2] = 40;
			motor[port4] = 40;
			motor[port6] = -30;
			motor[port8] = -30;
		}

		while(SensorValue[Line_FollowR] > 1500 && SensorValue[Line_FollowL] < 1500){
			motor[port2] = -30;
			motor[port4] = -30;
			motor[port6] = 40;
			motor[port8] = 40;
		}
	}
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}


void LineFW(int power){
	while(SensorValue[Line_FollowR] > 1500 || SensorValue[Line_FollowL] > 1500){
		motor[port2] = power;
		motor[port4] = power;
		motor[port6] = power;
		motor[port8] = power;

		while(SensorValue[Line_FollowR] < 1500 && SensorValue[Line_FollowL] > 1500){
			motor[port2] = -40;
			motor[port4] = -40;
			motor[port6] =30;
			motor[port8] = 30;
		}

		while(SensorValue[Line_FollowR] > 1500 && SensorValue[Line_FollowL] < 1500){
			motor[port2] = 30;
			motor[port4] = 30;
			motor[port6] = -40;
			motor[port8] = -40;
		}
	}

	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}


void DriveBW(int targetR,int targetL, int power){
	SensorValue(Right_Encoder) = 0;
	SensorValue(Left_Encoder) = 0;
	wait1Msec(10);
	while(abs(SensorValue[Right_Encoder]) < targetR){
		motor[port2] = power;
		motor[port4] = power;
		while(abs(SensorValue[Left_Encoder]) < targetL){
			motor[port6] = power;
			motor[port8] = power;
		}
	}
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}

void DriveFW(int targetR,int targetL, int power) {
	SensorValue(Right_Encoder) = 0;
	SensorValue(Left_Encoder) = 0;
	wait1Msec(10);
	while(abs(SensorValue[Right_Encoder]) < targetR){
		motor[port2] = -power;
		motor[port4] = -power;
		while(abs(SensorValue[Left_Encoder]) < targetL){
			motor[port6] = -power;
			motor[port8] = -power;
		}
	}
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}

void Turn(int targetR,int targetL, int power){
	SensorValue(Right_Encoder) = 0;
	SensorValue(Left_Encoder) = 0;
	wait1Msec(10);
	while(abs(SensorValue[Right_Encoder]) < targetR){
		motor[port2] = -power;
		motor[port4] = -power;
		while(abs(SensorValue[Left_Encoder]) < targetL){
			motor[port6] = power;
			motor[port8] = power;
		}
	}
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}

void SubeBrazo(int Target, int power, int corrector){
	while(SensorValue[Pot_Brazo] > Target){
		motor[port3] = power;
		motor[port5] = power;
		motor[port7] = power;
		motor[port9] = power;
	}
	motor[port3] = -corrector;
	motor[port5] = -corrector;
	motor[port7] = -corrector;
	motor[port9] = -corrector;
}

void BajaBrazo(int Target, int power){
	while(SensorValue[Pot_Brazo] < Target){
		motor[port3] = -power;
		motor[port5] = -power;
		motor[port7] = -power;
		motor[port9] = -power;
	}
	motor[port3] = 0;
	motor[port5] = 0;
	motor[port7] = 0;
	motor[port9] = 0;
}

void HoldBrazo(int power){
	motor[port3] = power;
	motor[port5] = power;
	motor[port7] = power;
	motor[port9] = power;
}

void ParaBrazo(){
	motor[port3] = 0;
	motor[port5] = 0;
	motor[port7] = 0;
	motor[port9] = 0;
}

void HoldPinza(int power){
	motor[port1] = power;
	motor[port10] = power;
}

void ParaPinza(){
	motor[port1] = 0;
	motor[port10] = 0;
}

void CierraPinza(int power){ //760 es el bumper
	motor[port1] = power;
	motor[port10] = power;
}

void AbrePinza(int power){
	motor[port1] = -power;
	motor[port10] = -power;
}

void pidBaseTurn(int target, float waitTime, float maxPower, float Kp, float Ki, float Kd){
	//	float Kp = 0.2; // Para girar con el brazo abajo sin peso: Kp = 0.2, Ki = 0.05, Kd = 0.5
	//	float Ki = 0.05;// Para girar con el brazo a mitad sin peso: Kp = 0.15, Ki = 0.075, Kd = 0.6
	//	float Kd = 0.5;

	int error, lastError, integralRaw, derivative, finalPower;

	float integralActiveZone = 30, integralPowerLimit = 50/Ki;

	bool timerBool = true;

	clearTimer(T1);

	while (time1[T1] < fixTimerValue(waitTime)){
		error = target-(SensorValue[in6]);

		integralRaw = abs(error) < integralActiveZone && error != 0 ? integralRaw + error : 0;
		if (integralRaw > integralPowerLimit)	integralRaw = integralPowerLimit;
		if (integralRaw < -integralPowerLimit)	integralRaw = -integralPowerLimit;

		derivative = Kd*(error-lastError);
		lastError = error;
		if (error == 0)	derivative = 0;

		finalPower = (Kp*error) + (Ki*integralRaw) + derivative; //proportion+derivative+integral

		if (finalPower > maxPower*127)	finalPower = maxPower*127;
		else if (finalPower < -maxPower*127)	finalPower = -maxPower*127;

		motor[port2] = finalPower;
		motor[port4] = finalPower;
		motor[port6] = -finalPower;
		motor[port8] = -finalPower;

		wait1Msec(40);

		if (error < 30)	timerBool = false;
		if (timerBool)	clearTimer(T1);
	}
	motor[port2] = 0;
	motor[port4] = 0;
	motor[port6] = 0;
	motor[port8] = 0;
}

#endif
