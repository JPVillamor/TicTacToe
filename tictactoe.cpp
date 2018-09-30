//This is the board for the game. 
//It's not an actual 3x3 board, rather proud of it.
#include <iostream>
#include <stdio.h>
#include <vector>

//PvE tictactoe!
//player will be 'X'

using namespace std;

struct Block { //each individual spot on the game board
	int n; //an easy indicator for location on the board
	char token; //this is what is shown on the screen, either the n, X or O
};

class Game {
private:
	struct Block Board[8][5];
	int state = 0; //states of game; 0 is neutral, 1 is player victory, 2 is cpu victory
	int nCurrent; //the active n value
public:
	void initBoard(); //set up the game
	void display(); //actually show the board
	void update(); //keeps score basically
	char finder(int n); //gives token of position n
	void placer(int n, char sub); //replaces token of position n with sub
	int openSpot(int n); //checks if the spot is not claimed by a token
	int validMove(int x); //makes sure the user input is in range.
	void getInput(); //takes user integer input
	void playerMove(); //incorporates input, validMove, finder, and placer
	void cpuMove(); //assess the current board, plan future moves and executes them
	int checkState(); //checks for win/endgame
	void execute(); //combines all the methods
};

void Game::initBoard() { //initalizes the empty rows
	int pos[24] {0,1,2,3,4,5,6,7,8,0,3,6,1,4,7,2,5,8,0,4,8,2,4,6};
	for (int k = 0; k < 24; k++) {
		Board[k/3][k%3].n = pos[k];
		Board[k/3][k%3].token = pos[k] + 48;
	}
	for (int i = 0; i < 8; i++) {
		Board[i][3].n = 0;
		Board[i][3].token = '?';
		Board[i][4].n = 0;
		Board[i][4].token = '?';
	}
}

void Game::display() { //show the board
	cout << "- - -" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << Board[i][j].token << " ";
		}
		cout << endl;
	}
}

void Game::update() { //counts Xs and Os each row
	for (int i = 0; i < 8; i++) { //refreshes the scoreboard so duplicates don't happen
		Board[i][3].n = 0; 
		Board[i][4].n = 0; 	
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			if (Board[i][j].token == 'X')
				Board[i][3].n++; //score in the X column
			else if (Board[i][j].token == 'O')
				Board[i][4].n++; //score in the O column
		}
	}
}

char Game::finder(int n) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			if (Board[i][j].n == n) //matches argument int with indicator int
				return Board[i][j].token; //then gives the corresponding token
		}
	}
}

void Game::placer(int n, char sub) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			if (Board[i][j].n == n)
				Board[i][j].token = sub; //places the argument char into the board
		}
	}
}

int Game::openSpot(int n) {
	if (finder(n) != 'X' && finder(n) != 'O') 
		return 1;
	else 
		return 0;
}

int Game::validMove(int x) {
	if (x >= 0 && x <= 8 && openSpot(x))
		return 1;
	else
		return 0;
}

void Game::getInput() {
	do {
		cout << "Enter position: ";
		cin >> nCurrent;
	} while (!(validMove(nCurrent)));
}

void Game::playerMove() {
	getInput();
	placer(nCurrent, 'X');
}

void Game::cpuMove() {
	int done = 0;
	for (int i = 0; i < 8; i++) { //offense mechanism
		if (Board[i][4].n == 2 && Board[i][3].n == 0) {
			for (int j = 0; j < 3; j++) {
				if (Board[i][j].token != 'O') {
					nCurrent = Board[i][j].n;
					placer(nCurrent, 'O');
					done = 1;
				}
			}
		}
		if (done == 1)
			break;
	}
	if (done == 0) {
		for (int i = 0; i < 8; i++) { //defense mechanism
			if (Board[i][3].n == 2 && Board[i][4].n == 0) {
				for (int j = 0; j < 3; j++) {
					if (Board[i][j].token != 'X') {
						placer(Board[i][j].n, 'O');
						done = 1;
					}
				}
			}
			if (done == 1)
				break;
		}
	}
	if (done == 0) { //advanced defenses
		if (finder(4) == 'O') {
			if (Board[6][3].n == 2 || Board[7][3].n == 2) {
				for (int p = 1; p < 9; p+=2) {
					if (openSpot(p)) {
						placer(p, 'O');
						done = 1;
						break;
					}
				}
			}
		}
	}
	if (done == 0 && finder(4) == 'O') {
		int even, odd;
		for (int p = 0; p < 9; p++) {
			if (finder(p) == 'X' && p%2 == 0)
				even = p;
			else if (finder(p) == 'X' && p%2 == 1)
				odd = p;
		}
		switch (even) {
			case 0:
				if (odd == 5)
					placer(2, 'O');
				else if (odd == 7)
					placer(6, 'O');
				done = 1;
				break;
			case 2:
				if (odd == 3)
					placer(0, 'O');
				else if (odd == 7)
					placer(8, 'O');
				done = 1;
				break;
			case 6:
				if (odd == 1)
					placer(0, 'O');
				else if (odd == 5)
					placer(8, 'O');
				done = 1;
				break;
			case 8:
				if (odd == 1)
					placer(2, 'O');
				else if (odd == 3)
					placer(6, 'O');
				done = 1;
				break;
		}
	}
	if (done == 0) { //default moves
		if (openSpot(4)) {
			placer(4, 'O');
			done = 1;
		}
		else if (openSpot(0) && (finder(5) != 'X' && finder(7) != 'X')) {
			placer(0, 'O');
			done = 1;
		}
		else if (openSpot(8)) {
			placer(8, 'O');
			done = 1;
		}
		else if (openSpot(2)) {
			placer(2, 'O');
			done = 1;
		}
		else if (openSpot(6)) {
			placer(6, 'O');
			done = 1;
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (Board[i][4].n == 0) {
					for (int j = 0; j < 3; j++) {
						if (Board[i][j].token != 'O' && Board[i][j].token != 'X') {
							placer(Board[i][j].n, 'O');
							break;
						}
					}
					break;
				}
			}
		}
	}
}

int Game::checkState() {
	for (int i = 0; i < 8; i++) {
		if (Board[i][3].n == 3)
			return 1;
		else if (Board[i][4].n == 3)
			return 2;
	}
	return 0;
}

void Game::execute() {
	initBoard();
	for (int t = 0; t < 9; t++) {
		update();
		state = checkState(); //checkState() depends on update()!
		display();
		if (state != 0)
			break;
		if (t%2 == 0)
			playerMove();
		else
			cpuMove();
	}
	//update();
	display();
	switch (state) {
		case 0:
			cout << "tie" << endl;
			break;
		case 1:
			cout << "Player wins." << endl;
			break;
		case 2:
			cout << "cpu wins." << endl;
			break;
	}
}

int main() {
	Game TicTacToe;
	TicTacToe.execute();
	return 0;
}
