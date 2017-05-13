int fixTimerValue (float rawSeconds){
	int miliseconds = rawSeconds*1000;
	if (miliseconds < 250)	miliseconds = 250;
	return miliseconds;
}

void PIDBaseTurn (int target, float waitTime, float maxPower = 1, float Kp = 0.2, float Ki = 0.05, float Kd = 0.5){
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
