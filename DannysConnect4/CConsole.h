#pragma once
#include <Windows.h>
#include <iostream>
class CConsole
{
public:

	
	CConsole();
	~CConsole();


	int PrintDialogue(const char* _message, bool _yesOrNoOption);

	const enum Dialogue {
			k_iConfirm = 1,
			k_iDeny
		};

	const char* pcConfirmQuit = "######################################\n ### are you sure you want to quit? ###\n ######################################\n'y' followed by return to confirm, any other input will cancel.\n";
	const char* pcCongratulations = "########################\n ### Congratulations! ###\n ########################\n\n Press y to play again, or n to quit. \n";
	const char* pcItsADraw = "In the end, it was a draw. Do you want to quit? \n";

	HANDLE g_hwndConsole;


};