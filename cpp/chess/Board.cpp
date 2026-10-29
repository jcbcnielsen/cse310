#include "Utils.h"
#include "Pieces.h"
#include "Board.h"
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

// Get a pointer to the board
std::vector<std::vector<Piece*>>* Board::get_board() { return &m_board; }

// Display the current board to the players
// If a pointer to a piece is passed, highlight that piece and its movement options
void Board::print_board(Piece* p)
{
    // Clear the screen
    std::cout << "\033[2J\033[1;1H";

    // Print the board header
    std::cout << "    File                  File\nRank  1  2  3  4  5  6  7  8\n";

    for (int rank {0}; rank < 8; ++rank) {
        // Print the rank label
        std::cout << "   " << rank + 1 << ' ';
        for (int file {0}; file < 8; ++file) {
            Piece* space {m_board[rank][file]};

            // If there is a piece selected to view the moves of
            if (p) {
                if (p == space) {
                    print_space('{', p->symbol());
                }
                else if (space) {
                    if (p->is_valid_move(space)) {
                        print_space('(', space->symbol());
                    }
                    else
                        print_space('[', space->symbol());
                }
                else if (p->is_valid_move({rank, file})) {
                    print_space('(', " ");
                }
                else
                    print_space('[', " ");
            }
            // If the board is just being printed at the beginning of a player's turn
            else if (space)
                print_space('[', space->symbol());
            else
                print_space('[', " ");
        }
        std::cout << "\n";
    }
    // Put a newline space between the board and the next prompt
    std::cout << std::endl;
}

// Print a space based on the border and the piece on it
void Board::print_space(char border, std::string_view symbol)
{
    std::cout << border << symbol;

    switch (border)
    {
    case '[':
        std::cout << ']';
        break;
    case '(':
        std::cout << ')';
        break;
    case '{':
        std::cout << '}';
        break;
    }
}

// Get a single coordinate for a piece
// Broken out into its own function for DRY purposes
int Board::get_coordinate(std::string_view rf) const
{
    int c {};
    while (!(0 < c) || !(c <= 8))
    {
        std::cout << "Enter the " << rf << ": ";
        std::cin >> c;

        // Make sure the program doesn't break if the user enters a non-int
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Validate the input before returning it
        if (!(0 < c) || !(c <= 8)) {
            std::cout << "That was not a valid " << rf << ". Please enter a valid " << rf << ".\n";
        }
    }
    return c;
}

// Get coordinates by user input
Coords Board::get_coords() const
{
    int r {get_coordinate("rank")};
    int f {get_coordinate("file")};

    // Ranks and files are displayed to the players on a 1-8 scale instead of a 0-7 scale,
    // so decrement each coordinate before returning them
    return {--r, --f};
}

// Get a piece on the board by user inputted coordinates
Piece* Board::get_piece() const
{
    Coords rf {get_coords()};
    return m_board[rf.rank][rf.file];
}

// Get a piece on the board by passed coordinates
Piece* Board::get_piece(Coords coords) const
{
    return m_board[coords.rank][coords.file];
}

// Get a const reference to the black king (not a pointer to the black king)
const King& Board::black() const
{
    return *m_black;
}

// Get a const reference to the white king (not a pointer to the white king)
const King& Board::white() const
{
    return *m_white;
}

// Make sure to deallocate the memory for the pieces before destroying the board
Board::~Board()
{
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            delete m_board[r][f];
        }
    }
}
