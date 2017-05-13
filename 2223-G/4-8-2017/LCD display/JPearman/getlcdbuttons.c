#ifndef  _GETLCDBUTTONS
#define  _GETLCDBUTTONS

#define LEFT_ARROW  247		//char for left arrow
#define RIGHT_ARROW 246		//char for right arrow
static  char l_arr_str[4] = {LEFT_ARROW,  LEFT_ARROW,  LEFT_ARROW,  0};
static  char r_arr_str[4] = {RIGHT_ARROW, RIGHT_ARROW, RIGHT_ARROW, 0};

TControllerButtons getLcdButtons(){
	TControllerButtons  buttons;

	while(nLCDButtons != kButtonNone) wait1Msec(10);	//if button pressed since beginning, wait until it is released

	do{
		buttons = nLCDButtons;
		wait1Msec(10);
	}while(buttons == kButtonNone);

	return(buttons);
}

#endif
