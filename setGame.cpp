#include <math.h>
#include "setGame.h"
#include <stdio.h>
//#include<cstdlib>
#include <stdlib.h>/* srand, rand */
#include <time.h>   


gameData::gameData() 
{
	// Create game data that should not be reset after initialized

	player1First = true;
	player2First = false;
	player1Score = 0;
	player2Score = 0;
	player1.piece = false;
	player1.score = 0;
	AI.piece = true;
	AI.score = 0;
	singlePlayerFlag = false;
	singlePlayerMode = false;
}

void gameData::gameInit() {
	//Create game data shat will be initialized after each game.

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			gameMapArr[i][j] = 0;
			gameMapVal[i][j] = 0;
		}
	}
	isWhite = false; // Black first
	gameFlag = true;
	rowPosOrder.clear();
	colPosOrder.clear();
	rowValVec.clear();
	colValVec.clear();
	player1.piece = !player1.piece;
	AI.piece = !AI.piece;
	srand(static_cast <unsigned> (time(0)));
	if (singlePlayerFlag) {
		singlePlayerMode = !singlePlayerMode;
		singlePlayerFlag = !singlePlayerFlag;
	}
}

//void gameData::gameStart(gameMode type) {
//
//	currentMode = doublePlayer;
//
//}

bool gameData::setPieces(int row, int column) {
	// Place a piece in an empty spot and switch player afterwords. 

	if (isWhite == false && gameMapArr[row][column] == 0 && gameFlag){
		gameMapArr[row][column] = -1;
		gameMapVal[row][column] = 0;
		isWhite = !isWhite;
		rowPosOrder.push_back(row);
		colPosOrder.push_back(column);
		return true;
	}
	else if (isWhite == true && gameMapArr[row][column] == 0 && gameFlag) {
		gameMapArr[row][column] = 1;
		gameMapVal[row][column] = 0;
		isWhite = !isWhite;
		rowPosOrder.push_back(row);
		colPosOrder.push_back(column);
		return true;
	}
	else {
		printf("Game Piece is placed, pleace reselect an location.\n");
		return false;
	}
}

bool gameData::checkWin(int row, int column) { // Need to check ???
	
	for (int i = 0; i < 5; i++) {
		// Check Row Connection
		if (((column - i) >= 0) && ((column - i + 4) <= 15)
			&& (gameMapArr[row][column - i] == gameMapArr[row][column - i + 1])
			&& (gameMapArr[row][column - i] == gameMapArr[row][column - i + 2])
			&& (gameMapArr[row][column - i] == gameMapArr[row][column - i + 3])
			&& (gameMapArr[row][column - i] == gameMapArr[row][column - i + 4])) 
			return true;
	}

	for (int i = 0; i < 5; i++) {
		// Check column connection
		if (row - i >= 0 && row - i + 4 <= 15
			&& gameMapArr[row - i][column] == gameMapArr[row - i + 1][column]
			&& gameMapArr[row - i][column] == gameMapArr[row - i + 2][column]
			&& gameMapArr[row - i][column] == gameMapArr[row - i + 3][column]
			&& gameMapArr[row - i][column] == gameMapArr[row - i + 4][column]) 
			return true;
	}

	for (int i = 0; i < 5; i++) {
		// Check top left to bottom right connection
		if (row - i >= 0 && row - i + 4 < 15
			&& column - i >= 0 && column - i + 4 < 15
			&& gameMapArr[row - i][column - i] == gameMapArr[row - i + 1][column - i + 1]
			&& gameMapArr[row - i][column - i] == gameMapArr[row - i + 2][column - i + 2]
			&& gameMapArr[row - i][column - i] == gameMapArr[row - i + 3][column - i + 3]
			&& gameMapArr[row - i][column - i] == gameMapArr[row - i + 4][column - i + 4]) 
			return true;
	}

	for (int i = 0; i < 5; i++) {
		// Check top right to bottom left connection
		if (row - i >= 0 && row - i + 4 < 15
			&& column + i < 15 && column + i - 4 >= 0
			&& gameMapArr[row - i][column + i] == gameMapArr[row - i + 1][column + i - 1]
			&& gameMapArr[row - i][column + i] == gameMapArr[row - i + 2][column + i - 2]
			&& gameMapArr[row - i][column + i] == gameMapArr[row - i + 3][column + i - 3]
			&& gameMapArr[row - i][column + i] == gameMapArr[row - i + 4][column + i - 4]) 
			return true;			
	}
		return false;
	
	
}

void gameData::isWin(int row, int column) {
	// Check if it's winning, print winning string, and switch color pieces for players

	if (checkWin(row, column) && gameMapArr[row][column] == -1) {
		gameFlag = !gameFlag;
		printf("Black win\n");
		printf("Press r to restart the game\n");
		if (player1First) {
			player1Score++;
		}
		else if (player2First) {
			player2Score++;
		}
		player1First != player1First;
		player2First != player2First;
	}
	else if (checkWin(row, column) && gameMapArr[row][column] == 1) {
		gameFlag = !gameFlag;
		printf("White win\n");
		printf("Press r to restart the game\n");
		if (player1First) {
			player1Score++;
		}
		else if (player2First) {
			player2Score++;
		}
		player1First != player1First;
		player2First != player2First;
	}
}

bool gameData::draw(int row, int column) {
	// keep a function in case if a draw happens. So that the program won't crash.

	if (!checkWin(row, column)) {
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				if (gameMapArr[i][j] == 0) 
					return false;
			}
		}
		return true;
	}
}


void gameData::isDraw(int row, int column) {
	// Actions if game is a draw.

	if (draw(row,column)) {
		gameFlag = !gameFlag;
		printf("Draw game\n");
		printf("Press r to restart the game\n");
		player1First != player1First;
		player2First != player2First;
	}
	
}

void gameData::undo(void) {

	if (rowPosOrder.size() > 0 && colPosOrder.size() > 0) {
		gameMapArr[rowPosOrder.back()][colPosOrder.back()] = 0;
		rowPosOrder.pop_back();
		colPosOrder.pop_back();
		isWhite = !isWhite;
		// Exception Error ???
	}
	

}

// need to confirm
int gameData::getPiece(int row, int col, int dir, int dis) {

	switch (dir) {
	case 1:

		if (row - dis >= 0 && row - dis < 15 && col - dis >= 0 && col - dis < 15 )
		return gameMapArr[row - dis][col - dis];
		break;
	case 2:

		if (row - dis >= 0 && row - dis < 15)
		return gameMapArr[row - dis][col];
		break;
	case 3:
		if (row - dis >= 0 && row - dis < 15 && col + dis >= 0 && col + dis <= 14)
		return gameMapArr[row - dis][col + dis];
		break;
	case 4:
		if (col + dis >= 0 && col + dis <= 14)
		return gameMapArr[row][col + dis];
		break;
	case 5:
		if (row + dis >= 0 && row + dis <= 14 && col + dis >= 0 && col + dis <= 14)
		return gameMapArr[row + dis][col + dis];
		break;
	case 6:
		if (row + dis >= 0 && row + dis <= 14)
		return gameMapArr[row + dis][col];
		break;
	case 7:
		if (row + dis >= 0 && row + dis <=14 && col - dis >= 0 && col - dis <= 14)
		return gameMapArr[row + dis][col - dis];
		break;
	case 8:
		if (col - dis >= 0 && col - dis <= 14)
		return gameMapArr[row][col - dis];
		break;
	}

	return -2; // -2 out of bounds
}

void gameData::setVal(int row, int col) {

	int isMyPiece;
	isMyPiece = AI.piece ? 1 : -1;

	for (int i = -5; i < 6;  i++) {
		for (int j = -5; j < 6; j++) {
			if (gameMapArr[row + i][col + j] != 0) {
				continue; // ???
			}
			else {
				// 0AAAA0
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == getPiece(row + i, col + j, k, 3)
						&& getPiece(row + i, col + j, k, 3) == getPiece(row + i, col + j, k, 4)
						&& getPiece(row + i, col + j, k, 5) == 0
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 300000;
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500; 
						}
					}
				}

				// 0AAAAB
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == getPiece(row + i, col + j, k, 3)
						&& getPiece(row + i, col + j, k, 3) == getPiece(row + i, col + j, k, 4)
						&& getPiece(row + i, col + j, k, 5) == (0 - getPiece(row + i, col + j, k, 4))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
							gameMapVal[row + i][col + j] += 2500;
							if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
								gameMapVal[row + i][col + j] -= 500;
							}
						}
				}

				// A0AAA
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, -1) == getPiece(row + i, col + j, k, 1)
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == getPiece(row + i, col + j, k, 3)
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 3000;
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500;
						}
					}
				}

				// AA0AA
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, -1) == getPiece(row + i, col + j, k, 1)
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, -2) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 2600;
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500;
						}
					}
				}

				// 0AAA0
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == getPiece(row + i, col + j, k, 3)
						&& getPiece(row + i, col + j, k, 4) == 0
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 3000;
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500;
						}
					}
				}

				// 0A0AA0
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, -1) == getPiece(row + i, col + j, k, 1)
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 3) == 0
						&& getPiece(row + i, col + j, k, -2) == 0
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 3000;
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500;
						}
					}
				}

				// 0AAAB
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == getPiece(row + i, col + j, k, 3)
						&& getPiece(row + i, col + j, k, 4) == (0 - getPiece(row + i, col + j, k, 3))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						if (getPiece(row + i, col + j, k, 1) != isMyPiece) {
							gameMapVal[row + i][col + j] -= 500; 
						}
					}
				}

				// 0A0AAB
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, -1) == getPiece(row + i, col + j, k, 1)
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 2) == (0 - getPiece(row + i, col + j, k, 3))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 500;
					}
				}

				// BA0AA0
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, -1) == getPiece(row + i, col + j, k, 1)
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, -2) == (0 - getPiece(row + i, col + j, k, 2))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 500;
					}
				}

				// 0AA0
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 3) == 0
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 600;
					}
				}
				// 0AAB
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) == getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 3) == (0 - getPiece(row + i, col + j, k, 2))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 150;
					}
				}
				for (int k = 1; k < 9; k++) {
					if (row + i >= 0 && row + i < 15
						&& col + j >= 0 && col + j < 15
						&& getPiece(row + i, col + j, k, 1) != getPiece(row + i, col + j, k, 2)
						&& getPiece(row + i, col + j, k, 1) != (0 - getPiece(row + i, col + j, k, 3))
						&& getPiece(row + i, col + j, k, 1) != 0
						&& getPiece(row + i, col + j, k, 1) != -2) {
						gameMapVal[row + i][col + j] += 1;
					}
				}
			}
		}
	}
}

void gameData::AISetPiece(void) {
	int maxVal = 0;
	if (AI.piece != isWhite) {
		return;
	}
	else {
		rowValVec.clear();
		colValVec.clear();
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				if (maxVal == gameMapVal[i][j]) {
					rowValVec.push_back(i);
					colValVec.push_back(j);
				}
				else if (gameMapVal[i][j] > maxVal) {
					rowValVec.clear();
					rowValVec.push_back(i);
					colValVec.clear();
					colValVec.push_back(j);
					maxVal = gameMapVal[i][j];
				}
			}
		}
		setPieces(rowValVec.front(), colValVec.front());
		isWin(rowValVec.front(), colValVec.front());
	}
}