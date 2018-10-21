#pragma once
#include "GameData.h"
#include "CBoardActions.h"
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



private:

	int g_iActivePlayer;

	CBoardActions* pCurrentBoardActions;

	CConsole*		pConsole;
	CBoard*			pBoard;

	GameData sGameData;

	int g_iGameState;


	// special values returned by (and passed to) fn GetPlayerInput()
	// n.b. all valid rows are in range 0 to ( k_iBoardWidth - 1 )

	int k_iPlayerInput_Quit;

	// n.b. -ve values from -1 to -k_iBoardWidth are used to return input if the chosen column is full
	int k_iPlayerInput_None;
	int k_iPlayerInput_Invalid;

};







