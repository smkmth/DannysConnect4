#pragma once

struct GameData {

	

	const enum BoardSlots {
		k_iBoardSlot_Empty,
		k_iBoardSlot_Player1,
		k_iBoardSlot_Player2,
		k_iBoardSlot_Frame
	};


	const enum GameState {
		k_iState_Playing,
		k_iState_Won_Player1,
		k_iState_Won_Player2,
		k_iState_Draw,
	};



	const char k_ai8BoardElementColours[4] =
	{
		7,	// white on black
		12,	// red on black
		14,	// yellow on black
		9,	// blue on black
	};

	const char k_achBoardPieces[4] =
	{
		' ', // no piece - g_achBoardPieces[ k_iBoardSlot_Empty ]
		'@', // player 1 - g_achBoardPieces[ k_iBoardSlot_Player1 ]
		'O', // player 2 - g_achBoardPieces[ k_iBoardSlot_Player2 ]
		'!', // not used, but provided since k_iBoardSlot_Frame exists
	};

	
};