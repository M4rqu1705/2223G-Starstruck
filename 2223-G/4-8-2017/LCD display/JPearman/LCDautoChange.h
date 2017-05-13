#ifndef LCDautoChange.h
#define LCDautoChange.h

#pragma systemFile

typedef enum vexAlliance {kAllianceBlue, kAllianceRed};
typedef enum vexStartSide {kStartLeft, kStartRight};
typedef enum vexLcdMenus {kMenuStart=0,kMenuAlliance=1, kMenuStartSide=2, kMenuAutonSelect=3};

static vexAlliance alliance = kAllianceBlue;
static vexStartSide side = kStartLeft;
static short vAuton = 0;
static vexLcdMenus menu = 0;

TControllerButtons getLcdButtons(){
	TControllerButtons  buttons;

	while(nLCDButtons != kButtonNone) wait1Msec(10);	//if button pressed since beginning, wait until it is released

	do{
		buttons = nLCDButtons;
		wait1Msec(10);
	}while(buttons == kButtonNone);
	return(buttons);
}

void LcdAutonomousDisplay(vexLcdMenus menu){
	clearLCDLine(0);
	clearLCDLine(1);

	displayLCDString(1,  0, "<");	// Display the selection arrow (left)
	displayLCDString(1, 15, ">");	// Display the selection arrows	(right)
	displayLCDString(1, 5, "Change");	// Display the

	// Show the autonomous names
	switch(menu){

	case kMenuAlliance: if(alliance == kAllianceBlue) displayLCDString(0, 1, "Alliance - BLUE"); else displayLCDString(0, 1, "Alliance - RED"); break;

	case kMenuStartSide: if(side == kStartLeft) displayLCDString(0, 2, "Start - LEFT"); else displayLCDString(0, 2, "Start - RIGHT"); break;

	case kMenuAutonSelect:
		switch(vAuton){
		case 0:	displayLCDString(0, 3, "Autonomous 1"); break;
		case 1:	displayLCDString(0, 3, "Autonomous 2");	break;
		case 2: displayLCDString(0, 3, "Autonomous 3"); break;
		default:displayLCDString(0, 3, "Undefined"); break;
		} break;

		//default: displayLCDString(0, 5, "2223-G"); break;
	default:
		char str[16];
		sprintf(str,"P: %d,%d,%d", SensorValue[in1], SensorValue[in2], SensorValue[in3]);
		displayLCDString(0,0,str);
		break;
	}
}

void LcdAutonomousSelection(){
	TControllerButtons  button;
	// Turn on backlight
	bLCDBacklight = true;

	// display default choice
	LcdAutonomousDisplay(menu);

	while(1){
		button = getLcdButtons();

		// Display and select the autonomous routine
		if(button == kButtonLeft || button == kButtonRight) {
			// previous choice
			if(button == kButtonLeft) if(--menu < kMenuStart) menu = kMenuAutonSelect;	//move to the end if at beginning
				//next choice
			if(button == kButtonRight) if(++menu > kMenuAutonSelect) menu = kMenuStart;	//move to beginning if already at the end
		}

		else if( button == kButtonCenter )	{
			switch(menu) {
			case kMenuAlliance:
				alliance = (alliance == kAllianceBlue) ? kAllianceRed : kAllianceBlue;
				break;
			case kMenuStartSide:
				side = (side == kStartLeft) ? kStartRight : kStartLeft;
				break;
			case kMenuAutonSelect:
				if(++vAuton == 4) vAuton = 0;
				break;
			}
		}

		LcdAutonomousDisplay(menu);
		wait1Msec(15);
	}
}

/*
void pre_auton(){
bStopTasksBetweenModes = true;
LcdAutonomousSelection();
}


void autonomousRed(){
//the same as autonomous blue only that some values are reversed or different
}

void autonomousBlue(){
if(side == kStartLeft){
switch(vAuton){
case 0: //some autonomous code
break;
case 1: //run some other autonomous code
break;
case 2: //run some other autonomous code
break;
default: break;
}
}
else if(side == kStartRight) {
switch(vAuton){
case 0: //some autonomous code
break;
case 1: //run some other autonomous code
break;
case 2: //run some other autonomous code
break;
default: break;
}
}
}

task autonomous(){
if(alliance == kAllianceBlue) autonomousBlue();
else autonomousRed();
}

task usercontrol(){
}
*/

#endif
