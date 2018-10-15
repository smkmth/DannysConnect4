#pragma once
#include "GameData.h"
#include "CBoard.h"
#include "CConsole.h"
#include <Windows.h>
#include <iostream>




class Game
{
public:

	Game();
	~Game();


	int	GetPlayerInput(int);

	int GetActivePlayer();
	int GetGameState();

	void SetActivePlayer(int);
	void SetGameState( int );
	void GameLoop();
	void Reset();
	//int PrintDialogue(const char* _message, bool _yesOrNoOption, bool _clearScreen);



private:

	int g_iActivePlayer;

	CBoard* pCurrentBoard;

	CConsole* pConsole;

	GameData sGameData;



	int g_iGameState = sGameData.k_iState_Playing;


	// special values returned by (and passed to) fn GetPlayerInput()
	// n.b. all valid rows are in range 0 to ( k_iBoardWidth - 1 )

	const int k_iPlayerInput_Quit		= pCurrentBoard->GetBoardWidth();

	// n.b. -ve values from -1 to -k_iBoardWidth are used to return input if the chosen column is full
	const int k_iPlayerInput_None		= -(pCurrentBoard->GetBoardWidth() + 1 );
	const int k_iPlayerInput_Invalid	= -(pCurrentBoard->GetBoardWidth() + 3 );

};







