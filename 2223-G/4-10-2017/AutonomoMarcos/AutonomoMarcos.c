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

#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"
#include "Funciones_Auto_Latinos.h"

void pre_auton(){
	bStopTasksBetweenModes = true;
}

task autonomous(){
	DriveBW(360, 360, 127);			//Moverse hacia atras para que los standoffs no choquen con la garra
	SubeBrazo(1400, 75, 20);    //sube brazo para flipout y para estar por encima del cubo
	AbrePinzaPot(1500, 40);     //abre pinza completa para flipout
	BajaBrazo(340, 75);					//Baja brazo para luego recoger las estrellas
	DriveFW(1300, 1300, 127);		//Mover base hacia en frente para recoger las estrellas
	CierraPinzaPot(1000, 40);		//Cierra garra, recogiendo las estrellas
	HoldPinza(20);							//Para aguantar las estrellas con fuerza
	SubeBrazo(1400, 75, 20);		// Sube el brazo un poco para rotar sin friccion
	HoldBrazo(-10);							//para que el peso no baje el brazo, mantener los motores corriendo
	wait1Msec(500);							//espera
	pidBaseTurn(1000,1,0.8,0.15,0.075,0.6);		//Rotar 90 grados
	while(SensorValue[Limit_Switch]==1){
		BW(100);									//moverse hacia atrás hasta llegar a la verja
		HoldBrazo(127);						//Subir el brazo hasta llegar a la verja
		HoldPinza(15);						//Abre poco a poco para soltar el contenido de la garra
		wait1Msec(10);
	}
	BajaBrazo(340, 75);

	//Segunda parte
	//pidBaseTurn(1010, 1, 0,8, 0.2, 0.05, 0.5); 	//Rotar un poco mas para que recoger la estrella de la esquina
	//move("f", 1300, 127);												//Moverse hacia en frente hasta llegar a la última estrella
	//HoldPinza(-30);															//Cerrar pinza para recoger la estrella
	//wait1Msec(200);
	//HoldPinza(-10);															//Aguantar la estrella con un poco de fuerza
	//while(SensorValue[Limit_Switch]==1){
	//BW(100);									//moverse hacia atrás hasta llegar a la verja
	//HoldBrazo(127);						//Subir el brazo hasta llegar a la verja
	//HoldPinza(15);						//Abre poco a poco para soltar el contenido de la garra
	//wait1Msec(10);
	//}
}

task usercontrol(){

}
