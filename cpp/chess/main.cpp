#include "Utils.h"
#include "Pieces.h"
#include "Board.h"
#include <iostream>
#include <string>

int main()
{
    // Setup cross-game variables
    std::string replay {"y"};

    // Outer game loop, only repeats on replays
    while (replay.front() == 'y' || replay.front() == 'Y') {
        // Setup the game
        Board board {};

        // Inner game loop, repeats each turn/round
        for (int turn {1}; true; ++turn) {
            Piece* p {};
            std::string confirm {};
            std::string err_msg {""};

            // Show the player the current board,
            // and choose which piece to move,
            // repeating until successful and confirmed
            while (true) {
                board.print_board();
                std::cout << err_msg;

                if (turn % 2 == 1) {
                    std::cout << "White";
                }
                else
                    std::cout << "Black";
                std::cout << ", choose a piece to move:\n";

                p = board.get_piece();

                if (p) {
                    // Successfully picked a piece of the same color,
                    // show possible moves and ask player to confirm,
                    // repeating the loop if not
                    if ((turn % 2 == 1) == p->is_white()) {
                        err_msg = "";
                        board.print_board(p);

                        std::cout << "Do you want to move this piece?\nType Y/yes to move it or anything else to cancel: ";
                        std::cin >> confirm;

                        if (confirm.front() == 'y' || confirm.front() == 'Y')
                            break;
                    }
                    // Picked a piece of the wrong color
                    else
                        err_msg = "That piece was not your color.\n";
                }
                // Picked an empty space (nullptr)
                else
                    err_msg = "That was a blank space.\n";
            }

            Coords move_to {};
            Piece* space {};

            // Show the player the possible moves,
            // and choose where to move,
            // repeating until successful
            while (true) {
                board.print_board(p);
                std::cout << err_msg << "Choose where to move to:\n";

                if (p->symbol() == w_king || p->symbol() == b_king)
                    std::cout << "Note: move your King onto its own space to acknowledge a checkmate.\n";

                move_to = board.get_coords();
                space = board.get_piece(move_to);

                if (space) {
                    if (p->is_valid_move(space)) {
                        p->move(space->coords().rank, space->coords().file, board.get_board());
                        break;
                    }
                }
                else if (p->is_valid_move(move_to)) {
                    p->move(move_to.rank, move_to.file, board.get_board());
                    break;
                }
                err_msg = "That is not a valid space to move to.\n";
            }

            // Determine if either player has been put in checkmate,
            // and break the main gameplay loop if so
            if (board.black().checkmate()) {
                board.print_board();
                std::cout << "Checkmate! Black loses!\n";
                break;
            }
            else if (board.white().checkmate()) {
                board.print_board();
                std::cout << "Checkmate! White loses!\n";
                break;
            }
        }

        // Ask if the user wishes to play again,
        // and repeat the outer loop if so
        std::cout << "Do you want to play again?\nType Y/yes to replay or anything else to quit: ";
        std::cin >> replay;
    }
    return 0;
}
