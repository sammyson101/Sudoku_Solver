#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;
/*
 	Spot represents a single place in the sudoku puzzle.
	Given represents whether the number was passed into the program as a part of the original puzzle
*/

class Spot{
	public:
	int value;
	int quadrant;
	bool given;
};


class Puzzle{
	public:
	Puzzle();
	vector<vector<Spot> > getPuzzle(char*);
	void printBoard();
	
	bool checkRow(int, int);
	bool checkColumn(int, int);
	bool checkQuad(int, int);	

	void solve(int, int);
	
	void goForward(int&, int&);
	void goBack(int&, int&);

	vector<vector<Spot > > board;
	bool solved;
};

Puzzle::Puzzle(){}


/*Create board based on the file passed on program call*/
vector<vector<Spot> > Puzzle::getPuzzle(char* fileName){
	ifstream f(fileName);
	if(f.is_open() == false){
		cout << "Invalid file name" << endl;
		exit(1);
	}
	vector<vector<Spot> > board(9, vector<Spot> (9));
	Spot temp;
	temp.given = false;
		
	int i = 0;
	int j = 0;
	int k = 0;

	
	//pushback temp spots into board
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){	
			//checking for quadrants
			//top 3
			if(i <= 2){
				if(j <= 2){
					temp.quadrant = 0;
				}
				else if(j >= 6){
					temp.quadrant = 2;
				}
				else{
					temp.quadrant = 1;
				}	
			}
			//bottom 3
			else if(i >= 6){
				if(j <= 2){
					temp.quadrant = 6;
				}
				else if(j >= 6){
					temp.quadrant = 8;
				}
				else{
					temp.quadrant = 7;
				}
			}
			//middle 3
			else{
				if(j <= 2){
					temp.quadrant = 3;
				}
				else if(j >= 6){
					temp.quadrant = 5;
				}
				else{
					temp.quadrant = 4;
				}
			}							
		board[i][j] = temp;	
		}
	}
		
	char c;
	string values;
	i = 0;
	//read puzzle values into a long string
	while(f.get(c)){
		if(c != ' ' && c != '\n'){
			values += c;
		}
	}
	

	//parse string into board
	//magic 9's (0 indexing) for 9x9 board
	i = 0;
	j = 0;
	k = 0;
	
	while(k < values.length() - 1){
		while (i < 9){
			while(j < 9){
				board[i][j].value = values[k] - '0'; // - '0' for ascci conversion
				if(values[k] != '0'){
					board[i][j].given = true;
				}
				//cout << "values[k] = " << values[k] << " board[i][j].value = " << board[i][j].value <<  " given = " << board[i][j].given << " quadrant = " << board[i][j].quadrant << endl;
				j++;
				k++;
			}
			j = 0;
			i++;
		}
	}	
	return board;
}

/*Prints full board */
void Puzzle::printBoard(){
	int i = 0;
	int j = 0;
	cout << "-------------------------" << endl;
	for(i = 0; i < 9; i++){
		cout << "|";
		for(j = 0; j < 9; j++){
			if(board[i][j].value == 0){
				cout << " -";
			}
			else{
				cout << ' ' << board[i][j].value;
			}
			if(j == 2|| j == 5 || j == 8){
				cout << " |";
			}

		}
		cout << endl;
		if( i == 2 || i == 5 || i == 8){
			cout << "-------------------------" << endl;
		}
	}
}

/*Checks if the spot's value exists elsewhere in the row*/
bool Puzzle::checkRow(int row, int column){
	int checkVal = board[row][column].value;
	int matches = 0;
	for(int j = 0; j < 9; j++){
		if(board[row][j].value == checkVal){
			matches++;
		}
	}
	if(matches > 1){
		return false;
	}
	else{
		return true;
	}
}

/*Checks if the spot's value exists elsewhere in the column*/
bool Puzzle::checkColumn(int row, int column){
	int checkVal = board[row][column].value;
	int matches = 0;
	for(int i = 0; i < 9; i++){
		if(board[i][column].value == checkVal){
			matches++;
		}
	}
	if(matches > 1){
		return false;
	}
	else{
		return true;
	}
}

/*Checks if spot's value exists elsewhere in the quadrant*/
bool Puzzle::checkQuad(int row, int column){
	int matches = 0;
	int checkVal = board[row][column].value;
	int quad = board[row][column].quadrant;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(board[i][j].quadrant == quad){
				if(board[i][j].value == checkVal){
					matches++;
				}
			}
		}
	}
	if(matches > 1){
		return false;
	}
	else{
		return true;
	}
}

/*Move the spot we are looking at one spot forward*/
void Puzzle::goForward(int &row, int &column){
	if(column == 8){
		column = 0;
		row++;
	}
	else{
		column++;
	}
}

/*Move the spot we are looking at back to first non given spot*/
void Puzzle::goBack(int &row, int &column){
	//go back at least once
	if(column == 0){
		row--;
		column = 8;
	}
	else{
		column--;
	}
	//test to find value that isn't given
	while(board[row][column].given == true){
		if(column == 0){
			row--;
			column = 8;
		}
		else{
			column--;
		}
	}
}

/*Solves Puzzle*/
void Puzzle::solve(int row, int column){
	
	//special case for last spot BASE CASE
	if(row == 8 && column == 8 && solved == false){
		//if we are at the last spot and the last spot was given: we are done
		if(board[8][8].given == true){
			solved = true;
			printBoard();
			return;
		}	
		
		//else last spot was not given inc as normal. If tests pass we done
		if(solved == false){
			board[row][column].value++;
		}	
		//looping through checks to set a value
		while(checkRow(row,column) == false || checkColumn(row,column) == false || checkQuad(row,column) == false && solved == false){

			board[row][column].value++;
		}
		//if a value == 10 then it didn't have a vaild move
		//reset to 0
		if(board[row][column].value == 10 && solved == false){
			board[row][column].value = 0;
			goBack(row, column);
			solve(row, column);
		}
		
		//if its a vaild value the puzzle has been solved
		else{
			solved = true;
			printBoard();
			return;
		}
	}	

	//check for given value. We aren't allowed to change those. Go to next call
	if(board[row][column].given == true && solved == false){
		goForward(row, column);
		//cout << "going forward to " << row << ' ' << column << endl;
		solve(row, column);	
	}
	
	//inc at least once
	if(solved == false){
		board[row][column].value++;
	}	
	//looping through checks to set a value
	while(checkRow(row,column) == false || checkColumn(row,column) == false || checkQuad(row,column) == false && solved == false){

		board[row][column].value++;
	}
	//if a value == 10 then it didn't have a vaild move
	//reset to 0
	if(board[row][column].value == 10 && solved == false){
		board[row][column].value = 0;
		goBack(row, column);
		solve(row, column);
	}

	else if (solved == false){
	goForward(row, column);
	solve(row, column);
	}
}


int main(int argc, char **argv){
	Puzzle puzzle;
	puzzle.board = puzzle.getPuzzle(argv[1]);
	puzzle.solved = false;
	puzzle.printBoard();
	puzzle.solve(0,0);
	return 0;	
}
