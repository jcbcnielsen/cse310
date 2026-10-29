#ifndef UTILS_H
#define UTILS_H

#include <string_view>

// w_king and b_rook are much easier to read than \u2654 and \u265C
constexpr std::string_view w_king   = "\u2654";
constexpr std::string_view w_queen  = "\u2655";
constexpr std::string_view w_rook   = "\u2656";
constexpr std::string_view w_bishop = "\u2657";
constexpr std::string_view w_knight = "\u2658";
constexpr std::string_view w_pawn   = "\u2659";
constexpr std::string_view b_king   = "\u265A";
constexpr std::string_view b_queen  = "\u265B";
constexpr std::string_view b_rook   = "\u265C";
constexpr std::string_view b_bishop = "\u265D";
constexpr std::string_view b_knight = "\u265E";
constexpr std::string_view b_pawn   = "\u265F";

// Included for readability, and used extensively
// by pieces and the board whenever locations are relevant
struct Coords
{
    int rank;
    int file;
};

#endif
