#pragma once
#include "pch.h"
#include "Game.h"





using namespace std;
//So this one is a game manager which is holding all the data
//the other classes might need.

int Game::GetActivePlayer() { return g_iActivePlayer; }
int Game::GetGameState() { return g_iGameState; }
//setters
void Game::SetActivePlayer(int value) { g_iActivePlayer = value; }
void Game::SetGameState(int value) { g_iGameState = value; }

Game::Game()
{

	//HANDLE g_hwndConsole = NULL;
	//g_hwndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CConsole console;
	pConsole = &console;

	CBoard board(this, pConsole);
	pCurrentBoard = &board;
	GameLoop();

}

Game::~Game()
{
}

void Game::GameLoop() {

	bool bQuitGame = false;
	while (!bQuitGame)
	{
		Reset();

		while (GetGameState() == sGameData.k_iState_Playing)
		{
			g_iActivePlayer = (sGameData.k_iBoardSlot_Player1 == g_iActivePlayer) ? sGameData.k_iBoardSlot_Player2 : sGameData.k_iBoardSlot_Player1;

			int iPlayerInput = k_iPlayerInput_None;

			//TODO this takes multiple inputs at once - usefull for testing 
			while (iPlayerInput < 0 && iPlayerInput < pCurrentBoard->GetBoardWidth())
			{
				pCurrentBoard->RenderBoard();
				// we pass the last input so this can print an appropriate message if it was invalid
				iPlayerInput = GetPlayerInput(iPlayerInput);
			}

			// if the player wants to quit make sure they do
			if (k_iPlayerInput_Quit == iPlayerInput)
			{
				int iConfirmQuitGame = 0;
				iConfirmQuitGame = pConsole->PrintDialogue( pConsole->pcConfirmQuit, true);
				if (iConfirmQuitGame == 1)
				{
					bQuitGame = true;
					break;
				}
				else
				{
					continue;
				}
								
			}

			pCurrentBoard->MakeAMove(iPlayerInput);

			// check for a draw
			if (pCurrentBoard->GetSpacesRemaining() == 0 )
			{
				
				g_iGameState = sGameData.k_iState_Draw;
				
			}
			else 
			{
				pCurrentBoard->DecrimentSpacesRemaining();

			}
		}
		if (sGameData.k_iState_Draw == g_iGameState)
		{
			if ((pConsole->PrintDialogue(pConsole->pcItsADraw, true) == pConsole->k_iConfirm))
			{
				int iConfirmQuitGame;
				
				iConfirmQuitGame = pConsole->PrintDialogue(pConsole->pcConfirmQuit, true);
				if (iConfirmQuitGame == pConsole->k_iConfirm)
				{
					bQuitGame = true;
					break;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			// congratulations! play again?
			if (pConsole->PrintDialogue(pConsole->pcCongratulations, true) == pConsole->k_iDeny)
			{
				// no thanks?
				int iConfirmQuitGame;
				iConfirmQuitGame = pConsole->PrintDialogue(pConsole->pcConfirmQuit, true);
				if (iConfirmQuitGame == pConsole->k_iConfirm)
				{
					bQuitGame = true;
					break;
				}
				else
				{
					continue;
				}
			}
			else {
				continue;
			}
		}

		g_iGameState = sGameData.k_iState_Playing;
		pCurrentBoard->ResetBoard();
		// (pCurrentBoard->GetBoardWidth() * pCurrentBoard->GetBoardHeight);

	}//while( !bQuitGame ){...

}

int	Game::GetPlayerInput(int iArgLastInput)
{
	int iReturnValue = k_iPlayerInput_Invalid;

	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[ g_iActivePlayer ]);
	std::cout << "## '" << sGameData.k_achBoardPieces[ g_iActivePlayer ] << "' player's turn ##";
	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[ sGameData.k_iBoardSlot_Empty ]);

	// all valid inputs are +ve 
	if(		( k_iPlayerInput_None != iArgLastInput )
		&&	( 0 > iArgLastInput ) )
	{
		if( k_iPlayerInput_Invalid == iArgLastInput )
		{
			cout << "\n### Your last input was invalid! Please try again. ###";
		}	
		// otherwise it was a full column
		else
		{
			cout << "\n### Column " << abs( iArgLastInput ) << " is full. Please choose another. ###";
		}
	}

	std::cout << "\nPlease input a column number (1 to " << pCurrentBoard->GetBoardWidth() << "), or 'q' to quit, then press return:\n";
		
	// get first character from stdin and throw the rest away.
	// this is necessary otherwise the input requires one cin.getch() 
	// for each character on the line and another for <enter> on the end
	char chInput;
	std::cin >> chInput;
	std::cin.sync();

	// validate input and break from loop when valid
	if( ('q' == chInput ) || ('Q' == chInput ))
	{
		// set input value and break from loop
		iReturnValue = k_iPlayerInput_Quit;
	}
	else
	{
		// numerals in ascii encoding start at 0 and go up to 9 in sequential order.
		// so, to get the column index we subtract '1' from chInput and check the value's in the range 
		// 0 < value < k_iBoardWidth 
		int iInputColumn = chInput - '1';
		if( ( 0 <= iInputColumn ) && ( pCurrentBoard->GetBoardWidth() > iInputColumn ) )
		{
			// check this column isn't full (0,0 is top left)

			if(sGameData.k_iBoardSlot_Empty == pCurrentBoard->g_aaiBoard[ iInputColumn ][ 0 ] )
			{
	 			// set input value and break from loop
				iReturnValue = iInputColumn;
			}
			else
			{
				// full column - return value in range -1 to -k_iBoardWidth
				iReturnValue = -( iInputColumn + 1 );
			}
		}

	}
	//if( quit ){ ... }else{...

	return iReturnValue;
}

void Game::Reset()
{
	// reset board
	//memset( pCurrentBoard->g_aaiBoard, sGameData.k_iBoardSlot_Empty, sizeof( pCurrentBoard->g_aaiBoard) );
	pCurrentBoard->ResetBoard();
	// randomise 1st player
	// N.B. ( rand() & 1) will give 0 or 1, so add 1 to get 1 or 2
	g_iActivePlayer = ( rand() & 1 ) + 1;

	// neither player has won yet
	g_iGameState = sGameData.k_iState_Playing;

	// reset winning position array
	//memset( pCurrentBoard->g_aaiWinningRow, 0, sizeof( g_aaiWinningRow ) );
}
