#ifndef SETGAME_H
#define SETGAME_H
#include <vector>     // Vector

enum gameMode { singlePlayer, doublePlayer };

class gameData {
public:

	int gameMapArr[15][15]; // 0: Blank.   -1: Black piece.   1: WhitePiece
	std::vector<int> rowPosOrder;
	std::vector<int> colPosOrder;
	std::vector<int> rowValVec;
	std::vector<int> colValVec;
	int gameMapVal[15][15];
	bool isWhite; // False: Black. True:White.
	gameMode currentMode;
	int player1Score, player2Score;	 // To count the score of the player even if the black/white changes.
	bool player1First, player2First; 
	bool gameFlag; // To toggle and disable the game after the winning.
	bool singlePlayerFlag, singlePlayerMode;

	struct player {
	public:
		bool piece;
		int score;
	}player1, player2, AI;
	
	gameData();
	void gameInit();
	//void gameStart(gameMode type);
	bool checkWin(int row, int column);
	bool draw(int row, int column);
	void isWin(int row, int column);
	//void scoreCnt();
	bool setPieces(int row, int column);
	void isDraw(int row, int column);
	void undo(void);
	int getPiece(int row, int col, int dir, int dis);// dir: starting from left top is 1, increment ccw
	void setVal(int row, int col);
	void AISetPiece(void);

};


#endif
