// Yixing Zheng
// zheng129@mail.chapman.edu
// Mar 27, 2019
// Project1

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <thread>
#include <pthread.h>
#include <chrono>

//defining rows/columns/grids and pointing to them
int* solidRows = new int[9];
int* solidColumns = new int[9];
int* solidGrids = new int[9];

//creating valid board
int** board = new int*[9];

using namespace std;

struct thread_data {
	int** board_data;
};

//checks each row of the puzzle
void *checkRows(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;

	for (int row = 0; row < 9; ++row)
	{
		int temp[9];
		for (int column = 0; column < 9; ++column)
		{
			temp[column] = board[row][column];
		}

		sort(temp, temp + 9);
		int solution[9];

		for (int i = 0; i < 9; ++i)
		{
			solution[i] = i + 1;
		}

		int sum = 0;

		for (int i = 0; i < 9; ++i)
		{
			if (solution[i] == temp[i])
			{
				sum++;
			}
		}

		if (sum == 9)
		{
			solidRows[row] = 1;
		}
		else
		{
			solidRows[row] = 0;
		}
	}

	int rowCheck = 0;

	for (int i = 0; i < 9; ++i)
	{
		if(solidRows[i] == 1)
		{
			rowCheck++;

			if (rowCheck == 9)
			{
				cout << "Every row in the puzzle is correct." << endl;
			}
		}
		else
		{
			cout << "There is an error in row " << i + 1 << endl;
		}
	}
	pthread_exit(NULL);
}

//same thing but checks the columns of the puzzle
void *checkColumns(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;

	this_thread::sleep_for(std::chrono::milliseconds(100));

	for(int column = 0; column < 9; ++column)
	{
		int temp [9];
		for (int row = 0; row < 9; ++row)
		{
			temp[row] = board[row][column];
		}

		sort(temp, temp + 9);
		int solution[9];
		for (int i = 0; i < 9; ++i)
		{
			solution[i] = i + 1;
		}

		int sum = 0;

		for (int i = 0; i < 9; ++i)
		{
			if (solution[i] == temp[i])
			{
				sum++;
			}
		}

		if (sum == 9)
		{
			solidColumns[column] = 1;
		}
		else
		{
			solidColumns[column] = 0;
		}
	}

	int columnCheck = 0;

	for (int i = 0; i < 9; ++i)
	{
		if(solidColumns[i] == 1)
		{
			columnCheck++;

			if (columnCheck == 9)
			{
				cout << "Every column in the puzzle is correct." << endl;
			}
		}
		else
		{
			cout << "There is an error in column " << i + 1 << endl;
		}
	}
	pthread_exit(NULL);
}

void *checkGrids(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;

	int currentGrid = 0;

	this_thread::sleep_for(std::chrono::milliseconds(200));

	for (int row = 0; row < 9; row += 3)
	{
		for (int column = 0; column < 9; column += 3)
		{
			int temp[9];
			temp[0] = board[row][column];
			temp[1] = board[row][column + 1];
			temp[2] = board[row][column + 2];
			temp[3] = board[row + 1][column];
			temp[4] = board[row + 1][column + 1];
			temp[5] = board[row + 1][column + 2];
			temp[6] = board[row + 2][column];
			temp[7] = board[row + 2][column + 1];
			temp[8] = board[row + 2][column + 2];

			sort(temp, temp + 9);

			int solution[9];
			for (int i = 0; i < 9; ++i)
			{
				solution[i] = i + 1;
			}

			int sum = 0;

			for (int i = 0; i < 9; ++i)
			{
				if (solution[i] == temp[i])
				{
					sum++;
				}
			}

			if (sum == 9)
			{
				solidGrids[currentGrid] = 1;
			}
			else
			{
				solidGrids[currentGrid] = 0;
			}

			++currentGrid;
		}
	}

	int checkGrid = 0;
	for (int i = 0; i < 9; ++i)
	{
		if (solidGrids[i] == 1)
		{
			checkGrid++;
			if (checkGrid == 9)
			{
				cout << "Every grid in the puzzle is correct." << endl;
			}
		}
		else
		{
			cout << "There is an error at grid " << i + 1 << endl;
		}
	}
	pthread_exit(NULL);
}

int main()
{
	//File I/O
	char file;
	ifstream fileIn;
	string filename;

	cout << "Enter the file name of the sudoku puzzle: ";
	cin >> filename;

	fileIn.open(filename);

	//show the sudoku format in the terminal
	for(int i = 0; i < 9; ++i)
	{
		board[i] = new int[9];
	}

	//help the program read the input by separating numbers from commas
	char** inputboard = new char*[18];
	for(int i = 0; i < 9; ++i)
	{
		inputboard[i] = new char[18];
	}

	//read the sudoku numbers from the file and place them in the empty puzzle
	for(int row = 0; row < 9; ++row)
	{
		for(int column = 0; column < 18; ++ column)
		{
			fileIn >> file;
			inputboard[row][column] = file;
		}
	}

	//ignoring commas by accessing every other input array
	for (int row = 0; row < 9; ++row)
	{
		board[row][0] = inputboard[row][0] - '0';
		board[row][1] = inputboard[row][2] - '0';
		board[row][2] = inputboard[row][4] - '0';
		board[row][3] = inputboard[row][6] - '0';
		board[row][4] = inputboard[row][8] - '0';
		board[row][5] = inputboard[row][10] - '0';
		board[row][6] = inputboard[row][12] - '0';
		board[row][7] = inputboard[row][14] - '0';
		board[row][8] = inputboard[row][16] - '0';
	}

	//finally we can print the puzzle
	cout << '\n' << "Sudoku Puzzle: " << '\n';
	for(int row = 0; row < 9; ++row)
	{
		for(int column = 0; column < 9; ++column)
		{
			if (column != 2 && column != 5)
			{
				cout << board[row][column] << " ";
			}
			else
			{
				cout << board[row][column] << "  ";
			}
		}
		if (row != 2 && row != 5)
		{
			cout << '\n';
		}
		else
		{
			cout << '\n' << '\n';
		}
	}

	cout << '\n';

	//initialize solve state boolean
	bool solved = false;
	int cycle = 1;
	pthread_t row, column, grid;
	thread_data td[3];
	int iret1, iret2, iret3;

	//if errors are found, identify them based on row and column and replace them
	//and prevent the puzzle from being 'solved'
	//count cycles to see how many moves it takes to solve
	while (!solved)
	{
		cout << "Cycle: " << cycle << endl;
		cycle++;

		td[0].board_data = board;
		iret1 = pthread_create(&row, nullptr, checkRows, (void*)&td[0]);
		if(iret1)
		{
			fprintf(stderr,"Error - pthread_create() return code: %d\n", iret1);
			exit(EXIT_FAILURE);
		}

		td[1].board_data = board;
		iret2 = pthread_create(&column, nullptr, checkColumns, (void*)&td[1]);
		if(iret2)
		{
			fprintf(stderr,"Error - pthread_create() return code: %d\n", iret2);
			exit(EXIT_FAILURE);
		}

		td[2].board_data = board;
		iret3 = pthread_create(&grid, nullptr, checkGrids, (void*)&td[2]);
		if(iret3)
		{
			fprintf(stderr,"Error - pthread_create() return code: %d\n", iret3);
			exit(EXIT_FAILURE);
		}

		pthread_join(row, NULL);
		pthread_join(column, NULL);
		pthread_join(grid, NULL);

		cout << '\n';

		int wrongRow = 0, wrongColumn = 0;
		for (int i = 0; i < 9; ++i)
		{
			if (solidRows[i] == 0)
			{
				wrongRow++;
			}
			if (solidColumns[i] == 0)
			{
				wrongColumn++;
			}
		}

		if (wrongRow == 0 && wrongColumn == 0)
		{
			solved = true;
		}
		else
		{
			int solution[] = {1,2,3,4,5,6,7,8,9};
			int currentRow, currentColumn;
			int errorNumber;

			for (int i = 0; i < 9; ++i)
			{
				if(solidRows[i] == 0)
				{
					currentRow = i;
				}
				if(solidColumns[i] == 0)
				{
					currentColumn = i;
				}
			}

			int tempPuzzle[9];

			for (int column = 0; column < 9; ++column)
			{
				tempPuzzle[column] = board[currentRow][column];
			}

			sort(tempPuzzle, tempPuzzle + 9);

			for (int i = 0; i < 9; ++i)
			{
				if (solution[i] != tempPuzzle[i])
				{
					errorNumber = i + 1;
				}
			}

			cout << "Replace " << board[currentRow][currentColumn] << " with " << errorNumber << " at row " << currentRow + 1 << " column " << currentColumn + 1 << "\n" << endl;

			board[currentRow][currentColumn] = errorNumber;

			cout << "The current working puzzle is: " << endl;

			for (int row = 0; row < 9; ++row)
			{
				for (int column = 0; column < 9; ++column)
				{
					if (column != 2 && column != 5)
					{
						cout << board[row][column] << " ";
					}
					else
					{
						cout << board[row][column] << "  ";
					}
				}
				if (row != 2 && row != 5)
				{
					cout << '\n';
				}
				else
				{
					cout << '\n' << '\n';
				}
			}

			cout << "" << endl;
		}
	}
	//should this go in solved?
	cout << "The solved puzzle: " << endl;
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
		{
			if (column != 2 && column != 5)
			{
				cout << board[row][column] << " ";
			}
			else
			{
				cout << board[row][column] << "  ";
			}
		}
		if (row != 2 && row != 5)
		{
			cout << '\n';
		}
		else
		{
			cout << '\n' << '\n';
		}
	}

	fileIn.close();

	return 0;
}
