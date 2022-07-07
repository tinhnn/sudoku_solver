// sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

#define SUDOKU_9X9   81
#define SUDOKU_16X16 256

//vector<vector<int>> board;
int board_sz;
vector<vector<int>> board = {
    {0, 0, 0,/**/ 8, 0, 0,/**/ 0, 0, 0},
    {0, 0, 5,/**/ 0, 0, 9,/**/ 7, 0, 0},
    {0, 0, 0,/**/ 1, 0, 4,/**/ 0, 6, 9},
    //----------------------------------
    {0, 0, 3,/**/ 0, 0, 5,/**/ 0, 0, 7},
    {8, 0, 0,/**/ 9, 0, 1,/**/ 0, 4, 0},
    {0, 9, 7,/**/ 0, 0, 0,/**/ 0, 0, 8},
    //----------------------------------
    {1, 0, 0,/**/ 0, 0, 0,/**/ 0, 0, 0},
    {7, 0, 0,/**/ 0, 6, 0,/**/ 0, 0, 0},
    {0, 0, 0,/**/ 0, 0, 0,/**/ 0, 2, 0}
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

void getBoard(vector<vector<int>>& board) {
    cout << "Nhap sudoku board: ";
    bool isOK = false;
    string tmpdata;
    while (isOK != true) {
        cin >> tmpdata;
        int len = tmpdata.length();
        if (len == SUDOKU_9X9 || len == SUDOKU_16X16) {
            if (len == SUDOKU_9X9) {
                // Sudoku 9x9
                int idx = 0;
                for (idx = 0; idx < SUDOKU_9X9; idx++) {
                    int num = tmpdata.at(idx) - '0';
                    if (num < 0 || num > 9) {
                        cout << "Data board khong hop le!!!\nNhap sudoku board: ";
                        break;
                    }
                    else {
                        board[idx / 9][idx % 9] = num;
                    }
                }
                if (idx == 81) isOK = true;
            }
            else {
                // Sudoku 16x16
                // TODO
            }
        }
        else {
            cout << "Kich thuoc board khong hop le!!!\nNhap sudoku board: ";
        }
    }
}

int main()
{
    // Input data to board
    getBoard(board);

    // solver
    solveSudoku();

    // print output board
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << "    ";
            if (j % 3 == 2) {
                cout << "|";
            }
        }
        cout << endl;
        if (i % 3 == 2) {
            cout << "---------------------------------------------" << endl;
        }
    }

    int flag = getchar();
    flag = getchar();
    return flag;
}