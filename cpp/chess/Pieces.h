#ifndef PIECES_H
#define PIECES_H

#include "Utils.h"
#include <string_view>
#include <vector>

class Piece
{
public:
    Piece(bool is_white, std::string_view symbol, Coords coords);

    virtual bool is_valid_move(const Coords& coords) const = 0;

    virtual bool is_valid_move(const Piece* space) const = 0;

    // virtual because Pawn and King have special extensions to the function
    virtual void move(const int r, const int f, std::vector<std::vector<Piece*>>* board);

    bool is_white() const;

    std::string_view symbol() const;

    Coords coords() const;

    bool has_moved() const;

    virtual ~Piece() = default;

protected:
    // Protected because the default constructor is only included for
    // compatibility with virtual inheritance
    Piece();

private:
    const bool m_is_white;
    const std::string_view m_symbol;
    Coords m_coords;
    bool m_has_moved {false};
};

class Pawn : public Piece
{
public:
    Pawn(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;

    // Overrides Piece::move() because Pawn needs to determine if promote() needs to be called
    void move(const int r, const int f, std::vector<std::vector<Piece*>>* board) override;

private:
    // Private because it's only meant to be used by Pawn::move()
    void promote(std::vector<std::vector<Piece*>>* board);
};

// virtaully inherits from Piece to avoid giving Queen two copies of Piece
class Rook : virtual public Piece
{
public:
    Rook(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;

protected:
    // Protected because the default constructor is only included for
    // compatibility with virtual inheritance
    Rook();
};

class Knight : public Piece
{
public:
    Knight(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;
};

// virtually inherits from Piece to avoid giving Queen two copies of Piece
class Bishop : virtual public Piece
{
public:
    Bishop(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;

protected:
    // Protected because the default constructor is only included for
    // compatibility with virtual inheritance
    Bishop();
};

// Uses multiple inheritance with Rook and Bishop to use in is_valid_move()
class Queen : public Rook, public Bishop
{
public:
    Queen(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;
};

class King : public Piece
{
public:
    King(bool is_white, Coords coords);

    bool is_valid_move(const Coords& space_coords) const override;

    bool is_valid_move(const Piece* space_piece) const override;

    // Overrides Piece::move() because King needs to be able to alter m_checkmate
    void move(const int r, const int f, std::vector<std::vector<Piece*>>* board) override;

    bool checkmate() const;

private:
    bool m_checkmate {false};
};

#endif
