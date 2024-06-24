#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;

void boards(vector<vector<int>>& board, int rows, int cols);
void dispboard(const vector<vector<int>>& board, const vector<vector<bool>>& revealed);
void dcolor(const vector<vector<int>>& board, const vector<vector<bool>>& revealed, int x1, int y1, int x2, int y2);
bool raskrtye(const vector<vector<bool>>& revealed);
void statistik(time_t startTime, time_t endTime, int moves);

int main() {
    int choice;
    cout << "Select board size:\n1. 4x4\n2. 5x6\n";
    cin >> choice;

    int rows = (choice == 1) ? 4 : 5;
    int cols = (choice == 1) ? 4 : 6;

    vector<vector<int>> board(rows, vector<int>(cols));
    vector<vector<bool>> revealed(rows, vector<bool>(cols, false));
    boards(board, rows, cols);

    int moves = 0;
    time_t startTime = time(0);
    time_t endTime;

    while (true) {
        dispboard(board, revealed);
        int x1, y1, x2, y2;
        cout << "Enter coordinates of first card (row and column): ";
        cin >> x1 >> y1;
        cout << "Enter coordinates of second card (row and column): ";
        cin >> x2 >> y2;

        if (x1 == x2 && y1 == y2) {
            cout << "You cannot select the same card twice." << endl;
            continue;
        }

        if (revealed[x1][y1] || revealed[x2][y2]) {
            cout << "One or both of these cards are already revealed." << endl;
            continue;
        }

        moves++;
        dcolor(board, revealed, x1, y1, x2, y2);
        this_thread::sleep_for(chrono::seconds(2));

        if (board[x1][y1] == board[x2][y2]) {
            cout << "Match found!" << endl;
            revealed[x1][y1] = true;
            revealed[x2][y2] = true;
        }
        else {
            cout << "No match. Cards will be hidden again." << endl;
        }

        if (raskrtye(revealed)) {
            endTime = time(0);
            cout << "Congratulations! You've matched all the cards!" << endl;
            break;
        }
    }

    statistik(startTime, endTime, moves);
    return 0;
}

void boards(vector<vector<int>>& board, int rows, int cols) {
    vector<int> cards;
    for (int i = 1; i <= rows * cols / 2; ++i) {
        cards.push_back(i);
        cards.push_back(i);
    }
    srand(time(0));
    random_shuffle(cards.begin(), cards.end());

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = cards[i * cols + j];
        }
    }
}

void dispboard(const vector<vector<int>>& board, const vector<vector<bool>>& revealed) {
    cout << "  ";
    for (int j = 0; j < board[0].size(); ++j) {
        cout << setw(2) << j << " ";
    }
    cout << endl;

    for (int i = 0; i < board.size(); ++i) {
        cout << setw(2) << i << " ";
        for (int j = 0; j < board[0].size(); ++j) {
            if (revealed[i][j]) {
                cout << setw(2) << board[i][j] << " ";
            }
            else {
                cout << " * ";
            }
        }
        cout << endl;
    }
}

void dcolor(const vector<vector<int>>& board, const vector<vector<bool>>& revealed, int x1, int y1, int x2, int y2) {
    cout << "  ";
    for (int j = 0; j < board[0].size(); ++j) {
        cout << setw(2) << j << " ";
    }
    cout << endl;

    for (int i = 0; i < board.size(); ++i) {
        cout << setw(2) << i << " ";
        for (int j = 0; j < board[0].size(); ++j) {
            if (revealed[i][j] || (i == x1 && j == y1) || (i == x2 && j == y2)) {
                if ((i == x1 && j == y1) || (i == x2 && j == y2)) {
                    if (board[x1][y1] == board[x2][y2]) {
                        cout << "\033[32m" << setw(2) << board[i][j] << "\033[0m ";
                    }
                    else {
                        cout << "\033[31m" << setw(2) << board[i][j] << "\033[0m ";
                    }
                }
                else {
                    cout << setw(2) << board[i][j] << " ";
                }
            }
            else {
                cout << " * ";
            }
        }
        cout << endl;
    }
}

bool raskrtye(const vector<vector<bool>>& revealed) {
    for (const auto& row : revealed) {
        if (find(row.begin(), row.end(), false) != row.end()) {
            return false;
        }
    }
    return true;
}

void statistik(time_t startTime, time_t endTime, int moves) {
    ofstream statsFile("statistics.txt", ios::app);
    if (statsFile.is_open()) {
        statsFile << "Game played on: " << time(&startTime)
            << "Time taken: " << difftime(endTime, startTime) << " seconds\n"
            << "Number of moves: " << moves << "\n\n";
        statsFile.close();
    }
    else {
        cerr << "Unable to open statistics file." << endl;
    }
}
