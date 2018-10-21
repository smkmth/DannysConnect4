#pragma once				   

class CBoard {
private:
	int k_iBoardWidth;
	int k_iBoardHeight;

	int* m_iaBoardArray;

	int k_iWinLineLength = 4;

public:
	CBoard();

	~CBoard();

	int* GetBoard();
	int GetBoardElement(int x, int y);
	void SetBoardElement(int x, int y, int value);

	int GetBoardWidth();
	int GetBoardHeight();




};
