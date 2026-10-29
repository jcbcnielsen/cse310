#ifndef BOARD_H
#define BOARD_H

#include "Utils.h"
#include "Pieces.h"
#include <string_view>
#include <vector>

class Board
{
public:
    std::vector<std::vector<Piece*>>* get_board();

    void print_board(Piece* p = nullptr);

    Piece* get_piece() const;

    Piece* get_piece(Coords coords) const;

    Coords get_coords() const;

    const King& black() const;

    const King& white() const;

    ~Board();

private:
    // private because it's only meant to be used by print_board()
    void print_space(char border, std::string_view symbol);

    // private because it's only meant to be used by get_coords()
    int get_coordinate(std::string_view rf) const;

    // The kings have their own special pointers kept track of outside of m_board
    // because they need to be accessible from an unknown point on the board
    // at the end of each round
    King* m_black { new King(false, {0, 4}) };
    King* m_white { new King(true, {7, 4}) };

    // The default value for m_board is kinda ugly,
    // but initializing the state of the board at the beginning of the game
    // was going to be ugly no matter where I put it,
    // so doing it as a default value in the header seemed to be
    // the least-ugly way to do it
    std::vector<std::vector<Piece*>> m_board {
        {
            // The back row of the black player
            new Rook(false, {0, 0}),
            new Knight(false, {0, 1}),
            new Bishop(false, {0, 2}),
            new Queen(false, {0, 3}),
            m_black,
            new Bishop(false, {0, 5}),
            new Knight(false, {0, 6}),
            new Rook(false, {0, 7})
        },
        {
            // The black pawns
            new Pawn(false, {1, 0}),
            new Pawn(false, {1, 1}),
            new Pawn(false, {1, 2}),
            new Pawn(false, {1, 3}),
            new Pawn(false, {1, 4}),
            new Pawn(false, {1, 5}),
            new Pawn(false, {1, 6}),
            new Pawn(false, {1, 7})
        },
        // The empty rows between the player's pieces
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        {
            // The white pawns
            new Pawn(true, {6, 0}),
            new Pawn(true, {6, 1}),
            new Pawn(true, {6, 2}),
            new Pawn(true, {6, 3}),
            new Pawn(true, {6, 4}),
            new Pawn(true, {6, 5}),
            new Pawn(true, {6, 6}),
            new Pawn(true, {6, 7})
        },
        {
            // The back row of the white player
            new Rook(true, {7, 0}),
            new Knight(true, {7, 1}),
            new Bishop(true, {7, 2}),
            new Queen(true, {7, 3}),
            m_white,
            new Bishop(true, {7, 5}),
            new Knight(true, {7, 6}),
            new Rook(true, {7, 7})
        }
    };
};

#endif
