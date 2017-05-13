#ifndef funcionesNoSincronicas.h
#define funcionesNoSincronicas.h

//##############################################################--Variables--##############################################################//
//Initialize the arm's variables
int errorArm = 0, lastErrorArm = 0, integralArm = 0, outputArm = 0;

//Initialize the base's variables
int baseErrorLeft = 0, baseLastErrorLeft = 0, baseIntegralLeft = 0, baseDerivativeLeft = 0, baseOutputLeft = 0;
int baseErrorRight = 0, baseLastErrorRight = 0, baseIntegralRight = 0, baseDerivativeRight = 0, baseOutputRight = 0;
bool timerBool = true; int lastError = 0;

//Initialize the claw's variables
int clawErrorLeft = 0, clawLastErrorLeft = 0, clawIntegralLeft = 0, clawDerivativeLeft = 0, clawOutputLeft = 0;
int clawErrorRight = 0, clawLastErrorRight = 0, clawIntegralRight = 0, clawDerivativeRight = 0, clawOutputRight = 0;
//##############################################################--Variables--##############################################################//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#################################################################--Arm--#################################################################//
//-----------------------------------------------------------------stopArm-----------------------------------------------------------------//
void stopArm(){
	//Preset varialbles to 0
	errorArm = 0; lastErrorArm = 0; integralArm = 0; outputArm = 0;
	//Stop arm
	motor[BrazoRB] = 0;
	motor[BrazoRY] = 0;
	motor[BrazoLB] = 0;
	motor[BrazoLY] = 0;
}

//------------------------------------------------------------------armPID-----------------------------------------------------------------//
void armPID(byte speed, int position, byte holdPower = 0){
	//Iitialize the local constants each time
	float Kp = 2, Ki = 0.025, Kd = 3, integralLimit = 50;

	//If position not reached with a 10 "tick" error
	if(SensorValue[Pot_Brazo] < position-10 && SensorValue[Pot_Brazo] > position + 10){
		errorArm = position - SensorValue[Pot_Brazo];
		if(errorArm != 0 && integralArm < integralLimit && integralArm > -integralLimit) integralArm += errorArm;	else integralArm = 0;
		outputArm = (Kp * errorArm) + (Ki * integralArm) + (Kd * (errorArm != 0 ? errorArm - lastErrorArm : 0));
		lastErrorArm = errorArm;

		motor[BrazoRB] = outputArm;
		motor[BrazoRY] = outputArm;
		motor[BrazoLB] = outputArm;
		motor[BrazoLY] = outputArm;
	}
	//If position reached, hold arm with desired power
	else{
		motor[BrazoRB] = holdPower;
		motor[BrazoRY] = holdPower;
		motor[BrazoLB] = holdPower;
		motor[BrazoLY] = holdPower;
	}
}

//#################################################################--Arm--#################################################################//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################################--Base--#################################################################//
//----------------------------------------------------------stopBase-----------------------------------------------------------------------//
void stopBase(){

	//Preset all values to 0
	baseErrorLeft = 0; baseLastErrorLeft = 0; baseIntegralLeft = 0; baseDerivativeLeft = 0; baseOutputLeft = 0;
	baseErrorRight = 0; baseLastErrorRight = 0; baseIntegralRight = 0; baseDerivativeRight = 0; baseOutputRight = 0;
	SensorValue[Left_Encoder] = 0;	SensorValue[Right_Encoder] = 0;

	//Reset Timer for turning
	clearTimer(T1);

	//Stop motors
	motor[DriveLF] = 0;
	motor[DriveLB] = 0;
	motor[DriveRF] = 0;
	motor[DriveRB] = 0;
}


//-------------------------------------------------------moveBasePID-----------------------------------------------------------------------//
void moveBasePID(byte speed, int pulses){

	float Kp = 1.75, Ki = 0.025, Kd = 3, integralLimit = 20;

	//If distance not reached
	if(abs(SensorValue[Left_Encoder]) < abs(pulses) || abs(SensorValue[Right_Encoder]) < abs(pulses)){

		//Calculate error
		baseErrorLeft = pulses - SensorValue[Left_Encoder];
		baseErrorRight = pulses - SensorValue[Right_Encoder];

		//Calculate the integral only if it is less than its limit and 9/10 of the total distance travelled
		if(baseErrorLeft != 0 && baseIntegralLeft < integralLimit && baseIntegralLeft > -integralLimit) baseIntegralLeft += baseErrorLeft;	else baseIntegralLeft = 0;
		if(baseErrorRight != 0 && baseIntegralRight < integralLimit && baseIntegralRight > -integralLimit) baseIntegralRight += baseErrorRight;	else baseIntegralRight = 0;

		//Calculate derivative
		baseOutputLeft = (Kp * baseErrorLeft) + (Ki * baseIntegralLeft) + (Kd * (baseErrorLeft != 0 ? baseErrorLeft - baseLastErrorLeft : 0));
		baseOutputRight = (Kp * baseErrorRight) + (Ki * baseIntegralRight) + (Kd * (baseErrorRight != 0 ? baseErrorRight - baseLastErrorRight : 0));

		baseLastErrorLeft = baseErrorLeft;
		baseLastErrorRight = baseErrorRight;

		//Make sure the output speed is less than the max speed
		if(baseOutputLeft > speed) baseOutputLeft = speed; else if (baseOutputLeft < -speed) baseOutputLeft = -speed;
		if(baseOutputRight > speed) baseOutputRight = speed; else if (baseOutputRight < -speed) baseOutputRight = -speed;

		motor[DriveLF] = baseOutputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
		motor[DriveLB] = baseOutputLeft / ((abs(SensorValue[Left_Encoder]) - abs(SensorValue[Right_Encoder])) * 0.01 + 1);
		motor[DriveRF] = baseOutputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
		motor[DriveRB] = baseOutputRight / ((abs(SensorValue[Right_Encoder]) - abs(SensorValue[Left_Encoder])) * 0.01 + 1);
	}
	else{
		motor[DriveLF] = 0;
		motor[DriveLB] = 0;
		motor[DriveRF] = 0;
		motor[DriveRB] = 0;
	}
}

//-------------------------------------------------------turnBasePID-----------------------------------------------------------------------//
void turnBasePID(int position, float timeLimit, float speed = 127, bool armUp = false){
	//Depeding on the arm's position the constants will be assigned
	float Kp = SensorValue[Pot_Brazo]>1000 ? 0.15 : 0.2,
	Ki = SensorValue[Pot_Brazo]>1000 ? 0.075 : 0.05,
	Kd = SensorValue[Pot_Brazo]>1000 ? 0.6 : 0.5,
	integralLimit = 50;

	if(time1[T1] < timeLimit){
		int error = position- floor((abs(SensorValue[Left_Encoder]) + abs(SensorValue[Right_Encoder]))/2);
		if(error != 0 && baseIntegralLeft < integralLimit && baseIntegralLeft > -integralLimit) baseIntegralLeft += error;	else baseIntegralLeft = 0;
		int baseOutput = (Kp * error) + (Ki * baseIntegralLeft) + (Kd * (error != 0 ? error - lastError : 0));
		lastError = error;

		//Make sure the output speed is less than the max speed
		if(baseOutputLeft > speed) baseOutputLeft = speed; else if (baseOutputLeft < -speed) baseOutputLeft = -speed;

		motor[DriveRB] = baseOutput;
		motor[DriveRF] = baseOutput;
		motor[DriveLB] = -baseOutput;
		motor[DriveLF] = -baseOutput;

		if (abs(error) < 30 || abs(error) < 30)	timerBool = false;
		if (timerBool)	clearTimer(T1);
	}
	motor[DriveRB] = 0;
	motor[DriveRF] = 0;
	motor[DriveLB] = 0;
	motor[DriveLF] = 0;
}
//################################################################--Base--#################################################################//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################################--Claw--#################################################################//
//----------------------------------------------------------stopClaw-------------------------------------------------------------------------//
void stopCLaw(){

	//Preset all values to 0
	clawErrorLeft = 0; clawLastErrorLeft = 0; clawIntegralLeft = 0; clawDerivativeLeft = 0; clawOutputLeft = 0;
	clawErrorRight = 0; clawLastErrorRight = 0; clawIntegralRight = 0; clawDerivativeRight = 0; clawOutputRight = 0;

	//Stop claw
	motor[PinzaL] = 0;
	motor[PinzaR] = 0;
}

//------------------------------------------------------moveClawPID-----------------------------------------------------------------------//
void clawPID(byte speed, int potValue, byte holdPower){

	float Kp = 0.5, Ki = 0.025, Kd = 1, integralLimit = 10;

	//If distance not reached
	if(abs(SensorValue[Pot_Left]) < abs(potValue) || abs(SensorValue[Pot_Right]) < abs(potValue)){

		//Calculate error
		clawErrorLeft = potValue - SensorValue[Pot_Left];
		clawErrorRight = potValue - SensorValue[Pot_Right];

		//Calculate the integral only if it is less than its limit and 9/10 of the total distance travelled
		if(clawErrorLeft != 0 && clawIntegralLeft < integralLimit && clawIntegralLeft > -integralLimit) clawIntegralLeft += clawErrorLeft;	else clawIntegralLeft = 0;
		if(clawErrorRight != 0 && clawIntegralRight < integralLimit && clawIntegralRight > -integralLimit) clawIntegralRight += clawErrorRight;	else clawIntegralRight = 0;

		//Calculate derivative
		clawOutputLeft = (Kp * clawErrorLeft) + (Ki * clawIntegralLeft) + (Kd * (clawErrorLeft != 0 ? clawErrorLeft - clawLastErrorLeft : 0));
		clawOutputRight = (Kp * clawErrorRight) + (Ki * clawIntegralRight) + (Kd * (clawErrorRight != 0 ? clawErrorRight - clawLastErrorRight : 0));

		clawLastErrorLeft = clawErrorLeft;
		clawLastErrorRight = clawErrorRight;

		//Make sure the output speed is less than the max speed
		if(clawOutputLeft > speed) clawOutputLeft = speed; else if (clawOutputLeft < -speed) clawOutputLeft = -speed;
		if(clawOutputRight > speed) clawOutputRight = speed; else if (clawOutputRight < -speed) clawOutputRight = -speed;

		motor[PinzaL] = clawOutputLeft / ((abs(SensorValue[Pot_Left]) - abs(SensorValue[Pot_Right])) * 0.01 + 1);
		motor[PinzaR] = clawOutputRight / ((abs(SensorValue[Pot_Right]) - abs(SensorValue[Pot_Left])) * 0.01 + 1);
	}
	//If position reached, hold claw with desired power
	else{
		motor[PinzaL] = holdPower;
		motor[PinzaR] = holdPower;
	}
}
//################################################################--Claw--#################################################################//

#endif
