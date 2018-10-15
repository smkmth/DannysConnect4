#pragma once
#include "pch.h"
#include "CConsole.h"



CConsole::CConsole()
{
	HANDLE g_hwndConsole = NULL;
	g_hwndConsole = GetStdHandle(STD_OUTPUT_HANDLE);

}


CConsole::~CConsole()
{
}


int CConsole::PrintDialogue(const char* _message, bool _yesOrNoOption) {

	std::cout << std::endl;
	// get 1st input char and flush the input buffer to prevent unwanted immediate returns from cin.get() later
	if (_yesOrNoOption == true) {
		std::cout << _message;
		std::cin.clear();
		std::cin.sync();
		char chInput;
		std::cin >> chInput;

		if (chInput == 'y' || chInput == 'Y')
		{
			return 1;
		}
		else {
			return 2;
		}
	}
	else {
		return 1;
	}
}
