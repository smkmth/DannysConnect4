#pragma once
#include <Windows.h>
#include <iostream>
#include "GameData.h"



class Game;
class CConsole;

class CBoard
{
private:



	Game* pCurrentGame;

	CConsole* pConsole;

	GameData sGameData;

	static const int k_iBoardWidth = 7;
	static const int k_iBoardHeight = 6;
	static const int k_iWinLineLength = 4;
	int m_iSpacesRemaining = k_iBoardWidth * k_iBoardHeight;


	int g_aaiWinningRow[k_iWinLineLength][2];
	

public:


	CBoard(Game*, CConsole*);

	~CBoard();

	//store the board array here cos basically everyone needs it
	int g_aaiBoard[k_iBoardWidth][k_iBoardHeight];

	void RenderBoard();
	void MakeAMove(int);
	void Update_CheckForWin();
	void ResetBoard();
	bool Update_CheckRows();
	bool Update_CheckDiagonalLURD();
	bool Update_CheckDiagonalRDLU();
	bool Update_CheckColumns();


	int GetBoardWidth();
	int GetBoardHeight();
	int GetSpacesRemaining();
	void SetSpacesRemaining(int);
	void DecrimentSpacesRemaining();


};

