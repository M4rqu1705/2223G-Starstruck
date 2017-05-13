//PID Summary
void move(byte speed, int ticks){
	int error = 0, lastError = 0, integral = 0, output = 0; float Kp = 1.75, Ki = 0.025, Kd = 3, integralLimit = 20;

	//While ticks not reached
	while(SensorValue[dgtl1] < ticks){
		error = ticks - SensorValue[dgtl1];		//error = ticks - (abs(SensorValue[dgtl1]) + abs(SensorValue[dgtl2]))/2;
		if(error != 0 && integral < integralLimit && integral > -integralLimit) integral += error;	else integral = 0;
		output = (Kp * error) + (Ki * integral) + (Kd * (error != 0 ? error - lastError : 0));
		lastError = error;
		//motor[port1] = output / ((abs(SensorValue[dgtl1]) - abs(SensorValue[dgtl2])) * 0.01 + 1);	//Make sure both sides run at the same rate
		//motor[port2] = output / ((abs(SensorValue[dgtl2]) - abs(SensorValue[dgtl1])) * 0.01 + 1);	//Make sure both sides run at the same rate
		motor[port1] = output;	//Or just move the motor
	}
}

task main(){
	move(50,360);
}
