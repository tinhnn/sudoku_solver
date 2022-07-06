// sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

void solveSudoku(vector<vector<char>>& board);

uint16_t groups[27];

static constexpr uint16_t allBits() {
    return (1 << 9) - 1;
}

static constexpr uint16_t charToBit(char c) {
    return 1 << (c - '1');
}

inline uint16_t& row(int i) {
    return groups[i];
}

inline uint16_t& col(int j) {
    return groups[9 + j];
}

inline uint16_t& square(int i, int j) {
    return groups[18 + 3 * (i / 3) + j / 3];
}

bool solveSudoku(vector<vector<int>>& board, int const i, int const j) {
    if (i > 8 || j > 8) return true;

    int& c = board[i][j];
    uint16_t rowGroup = row(i);
    uint16_t colGroup = col(j);
    uint16_t squareGroup = square(i, j);
    uint16_t possibleValues = '.' == c ? (allBits() & ~(rowGroup | colGroup | squareGroup)) : charToBit(c);
    char     savedChar = c;

    for (char nextC = '1'; nextC <= '9'; ++nextC) {
        if (charToBit(nextC) & possibleValues) {
            c = nextC;
            row(i) |= charToBit(nextC);
            col(j) |= charToBit(nextC);
            square(i, j) |= charToBit(nextC);

            int nextI = j == 8 ? i + 1 : i;
            int nextJ = (1 + j) % 9;

            if (solveSudoku(board, nextI, nextJ)) {
                return true;
            }
            else {
                row(i) = rowGroup;
                col(j) = colGroup;
                square(i, j) = squareGroup;
                c = savedChar;
            }
        }
    }
    return false;
}

void solveSudoku(vector<vector<int>>& board) {
    for (int i = 0; i < 9; ++i) {
        vector<int> const& r = board[i];
        for (int j = 0; j < 9; ++j) {
            char const c = r[j];
            if (c > 0 && c < 10) {
                row(i) |= charToBit(c);
                col(j) |= charToBit(c);
                square(i, j) |= charToBit(c);
            }
        }
    }
    solveSudoku(board, 0, 0);
}

int main()
{
    vector<vector<int>> board;

    // Input data to board
    for (int i = 0; i < 9; i++) {
        vector<int> row;
        for (int j = 0; j < 9; j++) {
            int num;
            cout << "Nhap matrix[" << i << "][" << j << "]: ";
            cin >> num;
            row.push_back(num);
        }
        board.push_back(row);
    }

    // solver
    solveSudoku(board);

    // print output board
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << "    ";
        }
        cout << endl;
    }

    getchar();
}