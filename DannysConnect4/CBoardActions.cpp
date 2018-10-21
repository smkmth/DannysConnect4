#pragma once
#include "pch.h"
#include "CBoardActions.h"
#include "Game.h"
#include "CConsole.h"

using namespace std;




CBoardActions::CBoardActions( Game* currentgame, CConsole* currentconsole)  
{
	pCurrentGame = currentgame;
	pConsole = currentconsole;
	CBoard cBoard;

	m_iSpacesRemaining = cBoard.GetBoardWidth() * cBoard.GetBoardHeight();

	ResetBoard();
	RenderBoard();


}


CBoardActions::~CBoardActions()
{
	//destroy the board
	//cBoard.~CBoard();

}


//getters

CBoard* CBoardActions::GetCurrentBoard()	
{
	CBoard* pBoard;
	pBoard = &cBoard;
	return pBoard;

}

int CBoardActions::GetSpacesRemaining()					{ return m_iSpacesRemaining; }
void CBoardActions::SetSpacesRemaining(int value)		{ m_iSpacesRemaining = value; }
void CBoardActions::DecrimentSpacesRemaining()			{ m_iSpacesRemaining--; }



//updates the board to whatever the board array says
void CBoardActions::RenderBoard()
{

	system("cls");

	const char	k_chSeparatorVertical = '|';
	const char	k_chSeparatorHorizontal = '-';
	const char	k_chSeparatorJoin = '+';



	for (int iColumnNumber = 0; iColumnNumber < cBoard.GetBoardWidth(); ++iColumnNumber)
	{
		std::cout << " " << (iColumnNumber + 1);
	}
	std::cout << endl;


	for (int iRow = 0; iRow < cBoard.GetBoardHeight(); ++iRow)
	{

		SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);

		std::cout << k_chSeparatorJoin;

		// we print 2 chars per column
		for (int iSeparator = 0; iSeparator < cBoard.GetBoardWidth(); ++iSeparator)
		{
			std::cout << k_chSeparatorHorizontal << k_chSeparatorJoin;
		}
		std::cout << endl;


		std::cout << k_chSeparatorVertical;

		// again, 2 chars per column
		for (int iColumn = 0; iColumn < cBoard.GetBoardWidth(); ++iColumn)
		{
			// cache board element index
			char i8BoardElementIndex = cBoard.GetBoardElement(iColumn,iRow);

			// render board element in appropriate colour
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[i8BoardElementIndex]);
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[i8BoardElementIndex]);
			std::cout << sGameData.k_achBoardPieces[i8BoardElementIndex];

			// render frame in frame colour
			SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);
			std::cout << k_chSeparatorVertical;
		}
		std::cout << endl;
	}

	// final row at bottom of board
	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Frame]);
	std::cout << k_chSeparatorJoin;
	// we print 2 chars per column
	for (int iSeparator = 0; iSeparator < cBoard.GetBoardWidth(); ++iSeparator)
	{
		std::cout << k_chSeparatorHorizontal << k_chSeparatorJoin;
	}
	std::cout << endl;

	// render column numbers again
	SetConsoleTextAttribute(pConsole->g_hwndConsole, sGameData.k_ai8BoardElementColours[sGameData.k_iBoardSlot_Empty]);
	for (int iColumnNumber = 0; iColumnNumber < cBoard.GetBoardWidth(); ++iColumnNumber)
	{
		std::cout << " " << (iColumnNumber + 1);
	}
	std::cout << endl;


}

void CBoardActions::MakeAMove(int iWhatColumn)
{
	// find the 'lowest' available position in iArgColumn
	// N.B. 0,0 is top left of board, so 'lowest' row is highest index
	int iLowestEmptyRow = 0;
	for (int iRowToCheck = 0; iRowToCheck < cBoard.GetBoardHeight(); ++iRowToCheck)
	{
		if(sGameData.k_iBoardSlot_Empty == cBoard.GetBoardElement(iWhatColumn,iRowToCheck))
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
	//g_aaiBoard[iWhatColumn][iLowestEmptyRow] = pCurrentGame->GetActivePlayer();
	cBoard.SetBoardElement(iWhatColumn, iLowestEmptyRow, pCurrentGame->GetActivePlayer());
	// if this finds a winning row it sets g_iGameState and g_aaiWinningRow
	Update_CheckForWin();
	RenderBoard();
}

void CBoardActions::Update_CheckForWin()
{

	if (Update_CheckRows() || Update_CheckColumns() ||
		Update_CheckDiagonalLURD() || Update_CheckDiagonalRDLU() )
	{
		pCurrentGame->SetGameState(pCurrentGame->GetActivePlayer());
	}
}

bool CBoardActions::Update_CheckRows()
{
	int iMaxColumnToCheck = (cBoard.GetBoardWidth() - (k_iWinLineLength - 1));

	for (int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn)
	{
		for (int iRow = 0; iRow < cBoard.GetBoardHeight(); ++iRow)
		{
			// N.B. set the winning row as we go
			if (cBoard.GetBoardElement(iColumn,iRow) == pCurrentGame->GetActivePlayer())
			{
				g_aaiWinningRow[0][0] = iColumn;
				g_aaiWinningRow[0][1] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 becuase the 0th was the one we've already found at the start of the line
				int iLinePosIndex = (k_iWinLineLength - 1);

				for (; iLinePosIndex > 0; --iLinePosIndex)
				{
					int iCheckColumn = iColumn + iLinePosIndex;

					if (cBoard.GetBoardElement(iCheckColumn ,iRow) == pCurrentGame->GetActivePlayer())
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

bool CBoardActions::Update_CheckColumns()
{
	// check for vertical line:
	// * start at [0][0]
	// * end at [ k_iBoardWidth ][ iMaxRowToCheck ]
	// * always check for possible end of line before the middle pieces
	int iMaxRowToCheck	= ( cBoard.GetBoardHeight() - ( k_iWinLineLength - 1 ) );

	for( int iColumn = 0; iColumn < cBoard.GetBoardWidth(); ++iColumn )
	{
		for( int iRow = 0; iRow < iMaxRowToCheck; ++iRow )
		{
			// N.B. set the winning row as we go
			if( cBoard.GetBoardElement( iColumn, iRow ) == pCurrentGame->GetActivePlayer())
			{
				g_aaiWinningRow[ 0 ][ 0 ] = iColumn;
				g_aaiWinningRow[ 0 ][ 1 ] = iRow;

				// now come back along the line filling in g_aaiWinningRow
				// N.B. we're checking to > 0 becuase the 0th was the one we've already found at the start of the line
				int iLinePosIndex = ( k_iWinLineLength - 1 );
				
				for( ; iLinePosIndex > 0; --iLinePosIndex )
				{
					int iCheckRow = iRow + iLinePosIndex;

					if( cBoard.GetBoardElement( iColumn, iCheckRow ) == pCurrentGame->GetActivePlayer())
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

bool CBoardActions::Update_CheckDiagonalLURD( )
{
	
	int iMaxColumnToCheck	= ( cBoard.GetBoardWidth() - ( k_iWinLineLength - 1 ) );
	int iMaxRowToCheck		= ( cBoard.GetBoardHeight() - ( k_iWinLineLength - 1 ) );

	for( int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn )
	{
		for( int iRow = 0; iRow < iMaxRowToCheck; ++iRow )
		{
			// N.B. set the winning row as we go
			if( cBoard.GetBoardElement( iColumn, iRow ) == pCurrentGame->GetActivePlayer() )
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

					if( cBoard.GetBoardElement(iCheckColumn, iCheckRow) == pCurrentGame->GetActivePlayer())
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

bool CBoardActions::Update_CheckDiagonalRDLU()
{
	
	int iMaxColumnToCheck	= ( cBoard.GetBoardWidth() - ( k_iWinLineLength - 1 ) );
	int iMinRowToCheck		= ( k_iWinLineLength - 1 );

	for( int iColumn = 0; iColumn < iMaxColumnToCheck; ++iColumn )
	{
		for( int iRow = iMinRowToCheck; iRow < cBoard.GetBoardHeight(); ++iRow )
		{
			// N.B. set the winning row as we go
			if( cBoard.GetBoardElement( iColumn, iRow ) == pCurrentGame->GetActivePlayer())
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

					if( cBoard.GetBoardElement( iCheckColumn, iCheckRow ) == pCurrentGame->GetActivePlayer())
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

void CBoardActions::ResetBoard() {

	for (int iRowToReset = 0; iRowToReset < cBoard.GetBoardHeight(); ++iRowToReset)
	{
		for (int iColToReset = 0; iColToReset < cBoard.GetBoardWidth(); ++iColToReset) {
			cBoard.SetBoardElement(iColToReset,iRowToReset, sGameData.k_iBoardSlot_Empty);

		}
		
	}
	RenderBoard();


}






