#pragma once
#include <Windows.h>
#include <iostream>
#include "GameData.h"
#include "CBoard.h"



class Game;
class CConsole;

class CBoardActions
{
private:



	Game* pCurrentGame;

	CConsole* pConsole;

	CBoard   cBoard;

	GameData sGameData;

	static const int k_iWinLineLength = 4;
	int g_aaiWinningRow[k_iWinLineLength][2];

	int m_iSpacesRemaining; 


	

public:


	CBoardActions(Game*, CConsole*);

	~CBoardActions();


	void RenderBoard();
	void MakeAMove(int);
	void Update_CheckForWin();
	void ResetBoard();
	bool Update_CheckRows();
	bool Update_CheckDiagonalLURD();
	bool Update_CheckDiagonalRDLU();
	bool Update_CheckColumns();

	CBoard* GetCurrentBoard();

	int GetSpacesRemaining();
	void SetSpacesRemaining(int value); 
	void DecrimentSpacesRemaining();


};

