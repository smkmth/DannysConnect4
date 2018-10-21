#pragma once
#include "pch.h"
#include <iostream>
#include "CBoard.h"




CBoard::CBoard()
{
	k_iBoardWidth = 5;
	k_iBoardHeight = 7;
	//int** m_iaBoardArray = new int*[k_iBoardWidth];
	//for (int i = 0; i < k_iBoardWidth; ++i) {
	//	m_iaBoardArray[i] = new int[k_iBoardHeight];;
	//}
	m_iaBoardArray = new int[k_iBoardWidth * k_iBoardHeight];


}

CBoard::~CBoard() {
	//for (int i = 0; i < k_iBoardWidth; ++i) {
	//	delete [] m_iaBoardArray[i];
	//}
	delete [] m_iaBoardArray;

}

int* CBoard::GetBoard() {

	return m_iaBoardArray;


}

int CBoard::GetBoardElement(int x, int y) {

	return m_iaBoardArray[y*k_iBoardWidth + x];

	//return m_iaBoardArray[x][y];
}

void CBoard::SetBoardElement(int x, int y, int value) 
{

	m_iaBoardArray[y*k_iBoardWidth + x] = value;

}

int CBoard::GetBoardWidth()					{ return k_iBoardWidth; }
int CBoard::GetBoardHeight()				{ return k_iBoardHeight; }
