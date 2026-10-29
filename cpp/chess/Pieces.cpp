#include "Utils.h"
#include "Pieces.h"
#include <cmath>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Standard constructor used by the child Piece classes
Piece::Piece(bool is_white, std::string_view symbol, Coords coords)
    : m_is_white {is_white}
    , m_symbol {symbol}
    , m_coords {coords}
{
}

// Default constructor specified for virtual inheritance purposes
Piece::Piece()
    : m_is_white {}
    , m_symbol {}
    , m_coords {}
{
}

// Relocate the piece to a different part of the board
// In Piece instead of Board for inheritance purposes (required by Pawn and King)
void Piece::move(const int r, const int f, std::vector<std::vector<Piece*>>* board)
{
    if (!m_has_moved)
        m_has_moved = true;
    if ((*board)[r][f])
        delete (*board)[r][f];
    (*board)[r][f] = this;
    (*board)[m_coords.rank][m_coords.file] = nullptr;
    m_coords = {r, f};
}

// Get the bool specifying if the piece is white or not
bool Piece::is_white() const { return m_is_white; }

// Get the string_view containing the symbol to draw for the piece
std::string_view Piece::symbol() const { return m_symbol; }

// Get the coordinates of the piece on the board
Coords Piece::coords() const { return m_coords; }

// Return the bool specifying if the piece has been moved before
bool Piece::has_moved() const { return m_has_moved; }

// Constructor for pawns
Pawn::Pawn(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_pawn : b_pawn, coords)
{
}

// Determine if a pawn is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool Pawn::is_valid_move(const Coords& space_coords) const
{
    return (
        (
        coords().rank - space_coords.rank == (is_white() ? 1 : -1) && std::abs(coords().file - space_coords.file) < 2
        ) || (!has_moved() ? (
        coords().rank - space_coords.rank == (is_white() ? 2 : -2) &&
        std::abs(coords().file - space_coords.file) == 0
        ) : false)
        );
}

// Determine if a pawn is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool Pawn::is_valid_move(const Piece* space) const
{
    return (is_white() != space->is_white()) && is_valid_move(space->coords());
}

// Special addition to normal piece movement:
// pawns promote when they reach the other side of the board
void Pawn::move(const int r, const int f, std::vector<std::vector<Piece*>>* board)
{
    Piece::move(r, f, board);
    if (coords().rank == 0 || coords().rank == 7)
        promote(board);
}

// Pawn promotion creates a new piece of the same color,
// puts it in the board vector in place of the pawn,
// and deletes the pawn at the end
void Pawn::promote(std::vector<std::vector<Piece*>>* board)
{
    std::string option {};
    Piece* promotion {};

    while (true) {
        std::cout << "Pawn promotion! Type one of the following options in:\nQ/queen, K/knight, R/rook, or B/bishop: ";
        std::cin >> option;

        if (option.front() == 'Q' || option.front() == 'q') {
            promotion = new Queen(is_white(), coords());
            break;
        }
        else if (option.front() == 'K' || option.front() == 'k') {
            promotion = new Knight(is_white(), coords());
            break;
        }
        else if (option.front() == 'R' || option.front() == 'r') {
            promotion = new Rook(is_white(), coords());
            break;
        }
        else if (option.front() == 'B' || option.front() == 'b') {
            promotion = new Bishop(is_white(), coords());
            break;
        }
        // If the player did not type a valid option,
        // inform them and loop back around to getting user input
        else
            std::cout << "That is not a valid promotion option.\n";
    }

    // Put the newly created piece on the board in place of the pawn
    // and ensure the pawn deletes itself before its pointer becomes unreachable
    (*board)[coords().rank][coords().file] = promotion;
    delete this;
}

// Constructor for rooks
Rook::Rook(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_rook : b_rook, coords)
{
}

// Default constructor for rooks included for virual inheritance purposes
Rook::Rook() {}

// Determine if a rook is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool Rook::is_valid_move(const Coords& space_coords) const
{
    return (std::abs(coords().rank - space_coords.rank) == 0 != std::abs(coords().file - space_coords.file == 0));
}

// Determine if a rook is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool Rook::is_valid_move(const Piece* space) const
{
    return (is_white() != space->is_white()) && is_valid_move(space->coords());
}

// Constructor for knights
Knight::Knight(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_knight : b_knight, coords)
{
}

// Determine if a knight is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool Knight::is_valid_move(const Coords& space_coords) const
{
    return (std::abs(coords().rank - space_coords.rank) == 2 && std::abs(coords().file - space_coords.file) == 1) ||
        (std::abs(coords().rank - space_coords.rank) == 1 && std::abs(coords().file - space_coords.file) == 2);
}

// Determine if a rook is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool Knight::is_valid_move(const Piece* space) const
{
    return (is_white() != space->is_white()) && is_valid_move(space->coords());
}

// Constructor for bishops
Bishop::Bishop(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_bishop : b_bishop, coords)
{
}

// Default constructor for bishops included for virtual inheritance purposes
Bishop::Bishop() {}

// Determine if a bishop is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool Bishop::is_valid_move(const Coords& space_coords) const
{

    return (
        std::abs(coords().rank - space_coords.rank) == std::abs(coords().file - space_coords.file) &&
        std::abs(coords().rank - space_coords.rank) != 0
        );
}

// Determine if a bishop is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool Bishop::is_valid_move(const Piece* space) const
{
    return (is_white() != space->is_white()) && is_valid_move(space->coords());
}

// Constructor for queens
Queen::Queen(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_queen : b_queen, coords)
{
}

// Determine if a queen is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool Queen::is_valid_move(const Coords& space_coords) const
{
    return Rook::is_valid_move(space_coords) || Bishop::is_valid_move(space_coords);
}

// Determine if a queen is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool Queen::is_valid_move(const Piece* space) const
{
    return (is_white() != space->is_white()) && is_valid_move(space->coords());
}

// Constructor for kings
King::King(bool is_white, Coords coords)
    : Piece(is_white, is_white ? w_king : b_king, coords)
{
}

// Determine if a king is able to move to a space given by its coordinates
// Overridden from Piece's virtual function
bool King::is_valid_move(const Coords& space_coords) const
{
    return (std::abs(coords().rank - space_coords.rank) < 2 && std::abs(coords().file - space_coords.file) < 2);
}

// Determine if a king is able to move to a space given by the piece occupying that space
// Overridden from Piece's virtual function
bool King::is_valid_move(const Piece* space) const
{
    return space == this || ((is_white() != space->is_white()) && is_valid_move(space->coords()));
}

// Get the bool indicating if a king is in checkmate
bool King::checkmate() const { return m_checkmate; }

// Special addition to movement:
// have a King move onto itself for checkmate
void King::move(const int r, const int f, std::vector<std::vector<Piece*>>* board)
{
    if (coords().rank == r && coords().file == f) {
        m_checkmate = true;
        return;
    }
    Piece::move(r, f, board);
}
