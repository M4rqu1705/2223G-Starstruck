#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartUserControl, baudRate4800, IOPins, None, None)

#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


#define LEFT_ARROW  247		//char for left arrow
#define RIGHT_ARROW 246		//char for right arrow
static  char l_arr_str[2] = {LEFT_ARROW,  0};
static  char r_arr_str[2] = {RIGHT_ARROW, 0};

static string vexAlliance[2] = {"kAllianceRed", "kAllianceBlue"};
static string vexStartSide[2] = {"kStartLeft", "kStartRight"};
static string vexLCDMenus[3] = {"kMenuAlliance", "kMenuStartSide", "kMenuAutonSelect"};

static byte alliance = 0, side = 1, vAuton = 0, menu = 0;


TControllerButtons getLcdButtons();
void LCDAutonomousDisplay(byte menu);
void LcdAutonomousSelection();

void autonomousRed(){
	//the same as autonomous blue only that some values are reversed or different
}

void autonomousBlue(){
	if(vexStartSide[side] == "kStartLeft"){
		switch(vAuton){
		case 0: /*some autonomous code*/ break;
		case 1: /*run some other autonomous code*/ break;
		case 2: /*run some other autonomous code*/ break;
		default: break;
		}
	}
	else if(vexStartSide[side] == "kStartRight") {
		switch(vAuton){
		case 0: /*some autonomous code*/ break;
		case 1: /*run some other autonomous code*/ break;
		case 2: /*run some other autonomous code*/ break;
		default: break;
		}
	}
}

void pre_auton(){
	LcdAutonomousSelection();
}

task autonomous(){
	if(vexAlliance[alliance] == "kAllianceBlue") autonomousBlue();
	else autonomousRed();
}

task usercontrol(){

}

//--------------------------------------------------------getLCDButtons--------------------------------------------------------------------//
TControllerButtons getLcdButtons(){
	TControllerButtons  buttons;

	while(nLCDButtons != kButtonNone) wait1Msec(10);	//if button pressed since beginning, wait until it is released

	do{
		buttons = nLCDButtons;
		wait1Msec(10);
	}while(buttons == kButtonNone);
	return(buttons);
}

//------------------------------------------------------LCDAutonomousDisplay---------------------------------------------------------------//

void LCDAutonomousDisplay(byte menu){
	clearLCDLine(0);
	clearLCDLine(1);

	displayLCDString(1,  0, l_arr_str);	// Display the selection arrow (left)
	displayLCDString(1, 14, r_arr_str);	// Display the selection arrows	(right)
	displayLCDString(1,  7, "Change");	// Display the change label

	// Show the autonomous names
	if(vexLCDMenus[menu] == "kMenuAlliance"){
		if(vexAlliance[alliance] == "kAllianceBlue") displayLCDString(0, 0, "Alliance - BLUE"); else displayLCDString(0, 0, "Alliance - RED");
	}
	else if(vexLCDMenus[menu] == "kMenuStartSide"){
		if(vexStartSide[menu] == "kStartLeft") displayLCDString(0, 0, "Start - LEFT"); else displayLCDString(0, 0, "Start - RIGHT");
	}
	else if(vexLCDMenus[menu] == "kMenuAutonSelect"){
		switch(vAuton){
		case 0:	displayLCDString(0, 0, "Autonomous 1"); break;
		case 1:	displayLCDString(0, 0, "Autonomous 2");	break;
		case 2: displayLCDString(0, 0, "Autonomous 3"); break;
		default: char str[16]; sprintf(str,"Undefined %d", vAuton ); displayLCDString(0, 0, str); break;
		}
	}
	else displayLCDString(0, 0, "Unknown");
}

//-----------------------------------------------------------------------------------------------------------------------------------------//

void LcdAutonomousSelection(){
	TControllerButtons  button;
	bLCDBacklight = true;
	LCDAutonomousDisplay(menu);

	while(1){
		button = getLcdButtons();

		// Display and select the autonomous routine
		if(button == kButtonLeft || button == kButtonRight) {
			// previous choice
			if(button == kButtonLeft) if(--menu < 0) menu = sizeof(vexLCDMenus)/sizeof(byte);	//move to the end if at beginning
				//next choice
			if(button == kButtonRight) if(++menu >= sizeof(vexLCDMenus)/sizeof(byte)) menu = 0;	//move to beginning if already at the end
		}

		else if( button == kButtonCenter )	{
			if(vexLCDMenus[menu] ==  "kMenuAlliance") alliance = (vexAlliance[side] == "kAllianceBlue") ? 1 : 0;
			if(vexLCDMenus[menu] == "kMenuStartSide") side = (vexStartSide[side] == "kStartLeft") ? 1 : 0;
			if(vexLCDMenus[menu] == "kMenuAutonSelect") if(++vAuton == 3) vAuton = 0;
		}
	}

	LCDAutonomousDisplay(menu);
	wait1Msec(10);
}
