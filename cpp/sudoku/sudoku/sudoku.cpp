// sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;
vector<vector<int>> board = {
    {0, 0, 0,/**/ 7, 0, 0,/**/ 0, 0, 0},
    {0, 2, 0,/**/ 0, 0, 0,/**/ 0, 0, 9},
    {0, 0, 8,/**/ 0, 0, 0,/**/ 2, 0, 4},
    //----------------------------------
    {0, 0, 6,/**/ 0, 4, 1,/**/ 0, 5, 0},
    {2, 0, 0,/**/ 0, 6, 8,/**/ 4, 0, 0},
    {0, 0, 0,/**/ 0, 0, 0,/**/ 0, 0, 0},
    //----------------------------------
    {0, 6, 0,/**/ 0, 0, 0,/**/ 0, 7, 0},
    {0, 0, 5,/**/ 0, 1, 6,/**/ 0, 0, 0},
    {0, 0, 0,/**/ 0, 3, 0,/**/ 9, 0, 8}
};

bool isPresentInCol(int col, int num) { //check whether num is present in col or not
    for (int row = 0; row < 9; row++)
        if (board[row][col] == num)
            return true;
    return false;
}
bool isPresentInRow(int row, int num) { //check whether num is present in row or not
    for (int col = 0; col < 9; col++)
        if (board[row][col] == num)
            return true;
    return false;
}
bool isPresentInBox(int boxStartRow, int boxStartCol, int num) {
    //check whether num is present in 3x3 box or not
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (board[row + boxStartRow][col + boxStartCol] == num)
                return true;
    return false;
}

bool findEmptyPlace(int& row, int& col) { //get empty location and update row and column
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (board[row][col] == 0) //marked with 0 is empty
                return true;
    return false;
}
bool isValidPlace(int row, int col, int num) {
    //when item not found in col, row and current 3x3 box
    return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
        col - col % 3, num);
}
bool solveSudoku() {
    int row, col;
    if (!findEmptyPlace(row, col))
        return true; //when all places are filled
    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            board[row][col] = num;
            if (solveSudoku()) //recursively go for other rooms in the grid
                return true;
            board[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}


int main()
{
    //vector<vector<int>> board;

    //// Input data to board
    //for (int i = 0; i < 9; i++) {
    //    vector<int> row;
    //    for (int j = 0; j < 9; j++) {
    //        int num;
    //        cout << "Nhap matrix[" << i << "][" << j << "]: ";
    //        cin >> num;
    //        row.push_back(num);
    //    }
    //    board.push_back(row);
    //}

    // solver
    solveSudoku();

    // print output board
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << "    ";
        }
        cout << endl;
    }

    getchar();
}