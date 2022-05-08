#include <stdio.h>
#include <stdbool.h>

void printBoard(char board[][26], int n);
void makeMoves(char board[][26], int n, char colour);
bool moveIsValid(char board[][26], int n, char colour, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool positionInBounds(int n, int row, int col);
char findOpposite(char turn);
void flipInDirection(char board[][26], int n, char colour, int row, int col);
void flipTiles(char board[][26], int n, char colour, int row, int col, int flipDeltaRow, int flipDeltaCol);
bool moveAvailable(char board[][26], int n, char colour);
void determineWinner(char board[][26], int n);
int countTiles(char board[][26], int n, char colour);
void copyTheBoard(char board[][26], char copyBoard[][26], int n);
int findOpponentMoves(char copyBoard[][26], int n, char colour);

int main(void){
	
	char board[26][26]; 
	int n;
	char computer, human, turn, letterRow, letterCol;
	int row, col;
	bool gameOver = false;
	
	printf("Enter the board dimension: ");
	scanf("%d", &n);
	
	printf("Computer plays (B/W): ");
	scanf(" %c", &computer);
	
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			if (i == n/2-1 && j == n/2-1){
				board[i][j]= 'W';
			}
			else if (i == n/2-1 && j == n/2){
				board[i][j]= 'B';
			}
			else if (i == n/2 && j == n/2-1){
				board[i][j]= 'B';
			}
			else if (i == n/2 && j == n/2){
				board[i][j]= 'W';
			}
			else{
				board[i][j]= 'U'; 
			}
		}
	}
	
	printBoard(board, n);
	
	if (computer == 'W'){
		human = 'B';
		turn = human;
	}
	else{
		human = 'W';
		turn = computer;
	}
	while(gameOver!=true){
		if (turn == computer){
			//make a move
			makeMoves(board, n, computer);
			printf("\n");
			printBoard(board, n);
		}
		else{
			printf("Enter move for colour %c (RowCol): ", human);
			scanf(" %c%c", &letterRow, &letterCol);
			row = letterRow - 97;
			col = letterCol - 97;
			if (moveIsValid(board, n, human, row, col)){
				board[row][col] = human;
				flipInDirection(board, n, human, row, col);
				printBoard(board, n);
			}
			else{
				printf("Invalid move.\n");
				printf("%c player wins.\n", computer);
				gameOver = true;
				return 0;
			}
		}
		if (gameOver!=true){
			if(moveAvailable(board, n, findOpposite(turn))){
				turn = findOpposite(turn);
			}
			else if(moveAvailable(board, n, turn)){
				turn = turn;
				printf("%c player has no valid move.\n", findOpposite(turn));
			}
			else{
				gameOver = true;
			}
		}
	}
	determineWinner(board, n);
	return 0;
}


bool moveAvailable(char board[][26], int n, char colour){
	bool available = false;
	bool stop = false;
	for (int i=0; (i<n) && !stop; i++){
		for(int j=0; (j<n) && !stop; j++){
			if (moveIsValid(board, n, colour, i, j)){
				available = true;
				stop = true;
			}
		}
	}
	return available;
}

void determineWinner(char board[][26], int n){
	int whiteScore = 0;
	int blackScore = 0;
	
	for (int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(board[i][j] == 'W'){
				whiteScore++;
			}
			else if(board[i][j] == 'B'){
				blackScore++;
			}
		}
	}
	if(blackScore>whiteScore){
		printf("B player wins.");
	}
	else if(whiteScore>blackScore){
		printf("W player wins.");
	}
	else if(blackScore==whiteScore){
		printf("\nDraw!");
	}
}

void printBoard(char board[][26], int n){
	char letter = 'a';
	printf ("  ");
	
	for (int i=0; i<n; i++){
		printf("%c", letter);
		letter = letter + 1;
	}
	
	printf("\n");
	letter = 'a';
	
	for (int i=0; i<n; i++){
		printf("%c ", letter);
		letter = letter + 1;
		//printf(");
		for (int j=0; j<n; j++){
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

char findOpposite(char turn){
	/*if (turn == computer){
		turn = human;
	}
	else{
		turn = computer;
	}*/
	if (turn == 'W'){
		turn = 'B';
	}
	else{
		turn = 'W';
	}
	return turn;
}

int countTiles(char board[][26], int n, char colour){
	int tiles = 0;
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(board[i][j] == colour){
				tiles++;
			}
		}
	}
	return tiles;
}

void copyTheBoard(char board[][26], char copyBoard[][26], int n){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			copyBoard[i][j] = board[i][j];
		}
	}
}

int findOpponentMoves(char copyBoard[][26], int n, char colour){
	int moves = 0;
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(moveIsValid(copyBoard, n, colour, i, j)){
				moves++;
			}
		}
	}
	return moves;
}

void makeMoves(char board[][26], int n, char colour){
	int tilesBeforeFlip = countTiles(board, n, colour);
	int tilesAfterFlip = 0;
	int score = 0, maxScore = 0;
	int highRow = 0, highCol = 0;
	int corners = 0;
	int maxTilesFlipped = 0;
	int currentTilesflipped = 0;
	int opponentMovesEliminated = 0;
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(moveIsValid(board, n, colour, i, j)){
				//printf("\nMove is valid!!!!!!!!!  %c%c", i+97, j+97);
				char copyBoard[26][26];
				copyTheBoard(board, copyBoard, n);
				
				copyBoard[i][j] = colour;
				flipInDirection(copyBoard, n, colour, i, j);
				
				tilesAfterFlip = countTiles(copyBoard, n, colour);
				currentTilesflipped = tilesAfterFlip - tilesBeforeFlip - 1;
				opponentMovesEliminated = findOpponentMoves(board, n, findOpposite(colour)) - findOpponentMoves(copyBoard, n, findOpposite(colour));
				if(i==0){
					if(j==0 || j==n-1){
						corners = 5;
					}
					else{
						corners = 2;
					}
				}
				else if(i==n-1){
					if(j==0 || j==n-1){
						corners = 5;
					}
					else{
						corners = 2;
					}
				}
				
				if(opponentMovesEliminated > 0){
					score = 2*currentTilesflipped + 6*corners + 4*opponentMovesEliminated;
				}
				else{
					score = 2*currentTilesflipped + 6*corners + opponentMovesEliminated;
				}
				if(score > maxScore){
					maxScore = score;
					highRow = i;
					highCol = j;
				}
			}
			else{
				continue;
			}
		}
	}
	//printf("\nHigh rowCol: %c%c and maxscore: %d and score: %d", highRow+97, highCol+97, maxScore, score);
	board[highRow][highCol] = colour;
	flipInDirection(board, n, colour, highRow, highCol);
	printf("Computer places %c at %c%c.", colour, highRow+97, highCol+97);

}


bool moveIsValid(char board[][26], int n, char colour, int row, int col){
	bool isValid = false; 
	if (board[row][col] != 'U'){ 
		isValid = false;
	}
	else{
		if(checkLegalInDirection(board, n, row, col, colour, -1, -1)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, -1, 0)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, -1, 1)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, 0, -1)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, 0, 1)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, 1, -1)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, 1, 0)){
			isValid = true;
		}
		else if(checkLegalInDirection(board, n, row, col, colour, 1, 1)){
			isValid = true;
		}
		else{
			isValid = false;
		}
	}	
	return isValid;
}


bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
	bool legalMove = false;
	
	if (board[row+deltaRow][col+deltaCol] == colour){
		legalMove = false;
	}
	else if(positionInBounds(n, row+deltaRow, col+deltaCol) == false){
		legalMove = false;
	}
	else if(board[row+deltaRow][col+deltaCol] == 'U'){
		legalMove = false;
	}
	else{
		for(int i=1; i<n; i++){
			if (board[row+i*deltaRow][col+i*deltaCol] == colour){
				legalMove = true;
				break;
			}
			else if(board[row+i*deltaRow][col+i*deltaCol] == 'U'){
				legalMove = false;
				break;
			}
			else if(!positionInBounds(n, row+i*deltaRow ,col+i*deltaCol)){
				legalMove = false;
				break;
			}
			else{
				continue;
			}
		} 
	}
	return legalMove;
}


bool positionInBounds(int n, int row, int col){
	if (row <0 || col <0){
		return false;
	}
	else if(row> n-1 || col>n-1){
		return false;
	}
	else{
		return true;
	}
}


// flips tiles in legal direction
void flipInDirection(char board[][26], int n, char colour, int row, int col){
	int flipDeltaRow;
	int flipDeltaCol;
	
	if(checkLegalInDirection(board, n, row, col, colour, -1, -1)){
		flipDeltaRow = -1;
		flipDeltaCol = -1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, -1, 0)){
		flipDeltaRow = -1;
		flipDeltaCol = 0;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, -1, 1)){
		flipDeltaRow = -1;
		flipDeltaCol = 1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, 0, -1)){
		flipDeltaRow = 0;
		flipDeltaCol = -1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, 0, 1)){
		flipDeltaRow = 0;
		flipDeltaCol = 1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, 1, -1)){
		flipDeltaRow = 1;
		flipDeltaCol = -1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, 1, 0)){
		flipDeltaRow = 1;
		flipDeltaCol = 0;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
	if(checkLegalInDirection(board, n, row, col, colour, 1, 1)){
		flipDeltaRow = 1;
		flipDeltaCol = 1;
		flipTiles(board, n, colour, row, col, flipDeltaRow, flipDeltaCol);
	}
}

void flipTiles(char board[][26], int n, char colour, int row, int col, int flipDeltaRow, int flipDeltaCol){
	for (int i=1; i<n; i++){
		if(board[row+i*flipDeltaRow][col+i*flipDeltaCol] == colour){
			break;
		}
		else{
			board[row+i*flipDeltaRow][col+i*flipDeltaCol] = colour;
		}
	}
}