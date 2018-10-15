#pragma once
#include "pch.h"
#include "CBoard.h"
#include "Game.h"
#include "CConsole.h"

using namespace std;



CBoard::CBoard( Game* currentgame, CConsole* currentconsole)
{
	pCurrentGame = currentgame;
	pConsole = currentconsole;
	ResetBoard();
	RenderBoard();


}


CBoard::~CBoard()
{

}


//getters
int CBoard::GetBoardHeight()		{ return k_iBoardHeight; }
int CBoard::GetSpacesRemaining()	{ return m_iSpacesRemaining;}
void CBoard::SetSpacesRemaining(int value) { m_iSpacesRemaining = value; }
void CBoard::DecrimentSpacesRemaining() { m_iSpacesRemaining--; }
int CBoard::GetBoardWidth()			{ return k_iBoardWidth; }

//updates the board to whatever the board array says
void CBoard::RenderBoard()
{

	system("cls");

	const char	k_chSeparatorVertical = '|';
	const char	k_chSeparatorHorizontal = '-';
	const char	k_chSeparatorJoin = '+';



	for (int iColumnNumber = 0; iColumnNumber < k_iBoardWidth; ++iColumnNumber)
	{
		cout << " " << (iColumnNumber + 1);
	}
	cout << endl;


	for (int iRow = 0; iRow < k_iBoardHeight; ++iRow)
	{

		SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);

		cout << k_chSeparatorJoin;

		// we print 2 chars per column
		for (int iSeparator = 0; iSeparator < k_iBoardWidth; ++iSeparator)
		{
			cout << k_chSeparatorHorizontal << k_chSeparatorJoin;
		}
		cout << endl;


		cout << k_chSeparatorVertical;

		// again, 2 chars per column
		for (int iColumn = 0; iColumn < k_iBoardWidth; ++iColumn)
		{
			// cache board element index
			char i8BoardElementIndex = g_aaiBoard[iColumn][iRow];

			// render board element in appropriate colour
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[i8BoardElementIndex]);
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[i8BoardElementIndex]);
			cout << sGameData.k_achBoardPieces[i8BoardElementIndex];

			// render frame in frame colour
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);
			cout << k_chSeparatorVertical;
		}
		cout << endl;
	}

	// final row at bottom of board
	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);
	cout << k_chSeparatorJoin;
	// we print 2 chars per column
	for (int iSeparator = 0; iSeparator < k_iBoardWidth; ++iSeparator)
	{
		cout << k_chSeparatorHorizontal << k_chSeparatorJoin;
	}
	cout << endl;

	// render column numbers again
	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Empty]);
	for (int iColumnNumber = 0; iColumnNumber < k_iBoardWidth; ++iColumnNumber)
	{
		cout << " " << (iColumnNumber + 1);
	}
	cout << endl;


}

void CBoard::MakeAMove(int iWhatColumn)
{
	// find the 'lowest' available position in iArgColumn
	// N.B. 0,0 is top left of board, so 'lowest' row is highest index
	int iLowestEmptyRow = 0;
	for (int iRowToCheck = 0; iRowToCheck < k_iBoardHeight; ++iRowToCheck)
	{
		if (sGameData.k_iBoardSlot_Empty == g_aaiBoard[iWhatColumn][iRowToCheck])
		{
			iLowestEmptyRow = iRowToCheck;
		}
		else
		{
			break;
		}
	}

	// insert current player's piece there
	// N.B. g_iActivePlayer is always either k_iBoardSlot_Player1 or k_iBoardSlot_Player2
	g_aaiBoard[iWhatColumn][iLowestEmptyRow] = pCurrentGame->GetActivePlayer();

	// if this finds a winning row it sets g_iGameState and g_aaiWinningRow
	Update_CheckForWin();
	RenderBoard();
}

void CBoard::Update_CheckForWin()
{

	if (Update_CheckRows() || Update_CheckColumns() ||
		Update_CheckDiagonalLURD() || Update_CheckDiagonalRDLU() )
	{
		pCurrentGame->SetGameState(pCurrentGame->GetActivePlayer());
	}
}

bool CBoard::Update_CheckRows()
{
	int iMaxColumnToCheck = (k_iBoardWidth - (k_iWinLineLength - 1));

	for (int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn)
	{
		for (int iRow = 0; iRow < k_iBoardHeight; ++iRow)
		{
			// N.B. set the winning row as we go
			if (g_aaiBoard[iColumn][iRow] == pCurrentGame->GetActivePlayer())
			{
				g_aaiWinningRow[0][0] = iColumn;
				g_aaiWinningRow[0][1] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 becuase the 0th was the one we've already found at the start of the line
				int iLinePosIndex = (k_iWinLineLength - 1);

				for (; iLinePosIndex > 0; --iLinePosIndex)
				{
					int iCheckColumn = iColumn + iLinePosIndex;

					if (g_aaiBoard[iCheckColumn][iRow] == pCurrentGame->GetActivePlayer())
					{
						g_aaiWinningRow[iLinePosIndex][0] = iCheckColumn;
						g_aaiWinningRow[iLinePosIndex][1] = iRow;
					}
					else
					{
						break;
					}
				}

				// if iLinePosIndex == 0 at this point it means we found a whole line
				if (0 == iLinePosIndex)
				{
					return true;
				}
				
			}
		}
	}
	return false;
}

bool CBoard::Update_CheckColumns()
{
	// check for vertical line:
	// * start at [0][0]
	// * end at [ k_iBoardWidth ][ iMaxRowToCheck ]
	// * always check for possible end of line before the middle pieces
	int iMaxRowToCheck	= ( k_iBoardHeight - ( k_iWinLineLength - 1 ) );

	for( int iColumn = 0; iColumn < k_iBoardWidth; ++iColumn )
	{
		for( int iRow = 0; iRow < iMaxRowToCheck; ++iRow )
		{
			// N.B. set the winning row as we go
			if( g_aaiBoard[ iColumn ][ iRow ] == pCurrentGame->GetActivePlayer())
			{
				g_aaiWinningRow[ 0 ][ 0 ] = iColumn;
				g_aaiWinningRow[ 0 ][ 1 ] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 becuase the 0th was the one we've already found at the start of the line
				int iLinePosIndex = ( k_iWinLineLength - 1 );
				
				for( ; iLinePosIndex > 0; --iLinePosIndex )
				{
					int iCheckRow = iRow + iLinePosIndex;

					if( g_aaiBoard[ iColumn ][ iCheckRow ] == pCurrentGame->GetActivePlayer())
					{												
						g_aaiWinningRow[ iLinePosIndex ][ 0 ] = iColumn;
						g_aaiWinningRow[ iLinePosIndex ][ 1 ] = iCheckRow;						
					}
					else
					{
						break;
					}
				}

				// if iLinePosIndex == 0 at this point it means we found a whole line
				if(	0 == iLinePosIndex )
				{
					return true;
				}
			}
		}
	}

	// didn't find a vertical line
	return false;
}

bool CBoard::Update_CheckDiagonalLURD( )
{
	
	int iMaxColumnToCheck	= ( k_iBoardWidth - ( k_iWinLineLength - 1 ) );
	int iMaxRowToCheck		= ( k_iBoardHeight - ( k_iWinLineLength - 1 ) );

	for( int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn )
	{
		for( int iRow = 0; iRow < iMaxRowToCheck; ++iRow )
		{
			// N.B. set the winning row as we go
			if( g_aaiBoard[ iColumn ][ iRow ] == pCurrentGame->GetActivePlayer() )
			{
				g_aaiWinningRow[ 0 ][ 0 ] = iColumn;
				g_aaiWinningRow[ 0 ][ 1 ] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 becuase the 0th was the one we've already found at the start of the line
				int iLinePosIndex = ( k_iWinLineLength - 1 );
				
				for( ; iLinePosIndex > 0; --iLinePosIndex )
				{
					int iCheckColumn	= iColumn + iLinePosIndex;
					int iCheckRow		= iRow + iLinePosIndex;

					if( g_aaiBoard[ iCheckColumn ][ iCheckRow ] == pCurrentGame->GetActivePlayer())
					{												
						g_aaiWinningRow[ iLinePosIndex ][ 0 ] = iCheckColumn;
						g_aaiWinningRow[ iLinePosIndex ][ 1 ] = iCheckRow;						
					}
					else
					{
						break;
					}
				}

				// if iLinePosIndex == 0 at this point it means we found a whole line
				if(	0 == iLinePosIndex )
				{
					return true;
				}
			}
		}
	}

	// didn't find a line
	return false;
}

bool CBoard::Update_CheckDiagonalRDLU()
{
	
	int iMaxColumnToCheck	= ( k_iBoardWidth - ( k_iWinLineLength - 1 ) );
	int iMinRowToCheck		= ( k_iWinLineLength - 1 );

	for( int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn )
	{
		for( int iRow = iMinRowToCheck; iRow < k_iBoardHeight; ++iRow )
		{
			// N.B. set the winning row as we go
			if( g_aaiBoard[ iColumn ][ iRow ] == pCurrentGame->GetActivePlayer())
			{
				g_aaiWinningRow[ 0 ][ 0 ] = iColumn;
				g_aaiWinningRow[ 0 ][ 1 ] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 because the 0th was the one we've already found at the start of the line
				int iLinePosIndex = ( k_iWinLineLength - 1 );
				
				for( ; iLinePosIndex > 0; --iLinePosIndex )
				{
					int iCheckColumn	= iColumn + iLinePosIndex;
					int iCheckRow		= iRow - iLinePosIndex;

					if( g_aaiBoard[ iCheckColumn ][ iCheckRow ] == pCurrentGame->GetActivePlayer())
					{												
						g_aaiWinningRow[ iLinePosIndex ][ 0 ] = iCheckColumn;
						g_aaiWinningRow[ iLinePosIndex ][ 1 ] = iCheckRow;						
					}
					else
					{
						break;
					}
				}

				// if iLinePosIndex == 0 at this point it means we found a whole line
				if(	0 == iLinePosIndex )
				{
					return true;
				}
			}
		}
	}

	// didn't find a line
	return false;
}

void CBoard::ResetBoard() {

	for (int iRowToReset = 0; iRowToReset < k_iBoardHeight; ++iRowToReset)
	{
		for (int iColToReset = 0; iColToReset < k_iBoardWidth; ++iColToReset) {
			g_aaiBoard[iColToReset][iRowToReset] = sGameData.k_iBoardSlot_Empty;

		}
		
	}
	RenderBoard();


}






