use std::io::*;
use clearscreen;

// Get and validate a positive number from user input
fn get_number(message:&str, min:usize, max:usize) -> usize {
    loop {
        let mut input: String = String::new();

        print!("{}", message);
        stdout().flush().unwrap();

        stdin().read_line(&mut input).expect("Failed to read input");

        // None of the user inputs accept 0 values, so use that as a default for errors
        let input_result = input.trim().parse::<usize>();
        let number = match input_result {
            Ok(num) => num,
            Err(_) => 0
        };

        // Positive inputs still need to be within the required range, so check that before returning
        if number < min || number > max {
            println!("Enter a valid number!");
        } else {
            return number;
        }
    }
}

// Assign each player a character to display their spaces on the board
fn assign_chars(game_count:&usize) -> (char, String, char, String) {
    if game_count % 2 == 1 {
        return ('X', String::from("Player 1 (X)"), 'O', String::from("Player 2 (O)"));
    }
    return ('O', String::from("Player 1 (O)"), 'X', String::from("Player 2 (X)"));
}

// Get a player's action
fn get_action(player_str:&str) -> (usize, usize) {
    let x = get_number(format!("{player_str}, enter an X coordinate from 1 to 3: ").as_str(), 1, 3);
    let y = get_number(format!("{player_str}, enter a Y coordinate from 1 to 3: ").as_str(), 1, 3);
    return (x - 1, y - 1);
}

// Check if the action is valid, then update the game board and return the validity of the action
fn update_board(player_char:&char, x:&usize, y:&usize, game_board:&mut Vec<Vec<char>>) -> bool {
    if game_board[*x][*y] != ' ' {
        return false;
    }
    game_board[*x][*y] = *player_char;
    return true;
}

// Have the player take their turn
fn take_turn(player_str:&str, player_char:&char, game_board:&mut Vec<Vec<char>>) {
    let mut update_success = false;
    while update_success == false {
        let (x, y) = get_action(&player_str);
        update_success = update_board(&player_char, &x, &y, game_board);
        if update_success == false {
            println!("Please enter coordinates that have not already been taken.");
        }
    }
}

// Update the terminal display to show the current state of the game
fn print_board(game_board:&Vec<Vec<char>>) {
    clearscreen::clear().expect("Failed to clear the terminal screen");

    println!("  X  1  2  3 X\nY              Y\n   |--|--|--|\n3  | {}| {}| {}|  3", game_board[0][2], game_board[1][2], game_board[2][2]);
    println!("   |--|--|--|\n2  | {}| {}| {}|  2", game_board[0][1], game_board[1][1], game_board[2][1]);
    println!("   |--|--|--|\n1  | {}| {}| {}|  1", game_board[0][0], game_board[1][0], game_board[2][0]);
    println!("   |--|--|--|\nY              Y\n  X  1  2  3 X");
}

// Check to see if the game has reached an end state, and return a character representing that state
fn check_end(game_board:&Vec<Vec<char>>) -> char {
    let win_routes: Vec<(&char, &char, &char)> = vec![
        (&game_board[0][0], &game_board[0][1], &game_board[0][2]), // Bottom Row
        (&game_board[1][0], &game_board[1][1], &game_board[1][2]), // Middle Row
        (&game_board[2][0], &game_board[2][1], &game_board[2][2]), // Top Row
        (&game_board[0][0], &game_board[1][0], &game_board[2][0]), // Left Column
        (&game_board[0][1], &game_board[1][1], &game_board[2][1]), // Middle Column
        (&game_board[0][2], &game_board[1][2], &game_board[2][2]), // Right Column
        (&game_board[0][0], &game_board[1][1], &game_board[2][2]), // BL to TR Diagonal
        (&game_board[0][2], &game_board[1][1], &game_board[2][0])  // TL to BR Diagonal
    ];
    let mut draw_routes = 0;

    // If the route is a win, return the winning player's character
    // If the route is full without being a winner, increase the draw route counter
    for route in win_routes {
        if *route.0 != ' ' && *route.0 == *route.1 && *route.0 == *route.2 {
            return *route.0;
        } else if *route.0 != ' ' && *route.1 != ' ' && *route.2 != ' ' {
            draw_routes += 1;
        }
    }

    // If the draw route counter is equal to the number of possible winning routes (8), end the game as a draw.
    // If there are no completed routes and the game is not a draw, default to returning a space
    if draw_routes == 8 {
        return 'D';
    }
    return ' ';
}

fn main() {
    // Initialize outside-game counters
    let mut player_one_score: usize = 0;
    let mut player_two_score: usize = 0;
    let mut draw_count: usize = 0;
    let mut game_count: usize = 1;

    // Outer game loop, only repeats on replays
    loop {

        let (player_one_char, player_one_str, player_two_char, player_two_str) = assign_chars(&game_count);
        let mut game_board: Vec<Vec<char>> = vec![vec![' ', ' ', ' '], vec![' ', ' ', ' '], vec![' ', ' ', ' ']];
        let mut round_count: usize = 1;

        // Main game loop, repeats until game ends
        loop {

            // Print the current state of the game to the terminal
            print_board(&game_board);

            // Have the next player take their turn
            if (game_count + round_count) % 2 == 0 {
                take_turn(&player_one_str, &player_one_char, &mut game_board);
            } else {
                take_turn(&player_two_str, &player_two_char, &mut game_board);
            }

            // Check if a player has won, and end the game if they have
            let winning_char = check_end(&game_board);
            if winning_char == player_one_char {
                print_board(&game_board);
                println!("Game Over! Player 1 Wins!\n");
                player_one_score += 1;
                break;
            } else if winning_char == player_two_char {
                print_board(&game_board);
                println!("Game Over! Player 2 Wins!\n");
                player_two_score += 1;
                break;
            } else if winning_char == 'D' {
                print_board(&game_board);
                println!("Game Over! It's a Draw!\n");
                draw_count += 1;
                break;
            }

            // If the game has not ended, increment the round count
            round_count += 1;
        }

        // Display the results of the game
        println!("Player 1 Score: {player_one_score}\nPlayer 2 Score: {player_two_score}\nTied Games: {draw_count}\n");

        // Check if the player wants to play a new game, breaking the outer loop if not
        let mut replay: String = String::new();

        print!("Do you want to play again? (Enter \"y\" to replay or anything else to quit) ");
        stdout().flush().unwrap();

        stdin().read_line(&mut replay).expect("Failed to read input");

        if replay.trim().to_lowercase() != "y" {
            break;
        }

        // If the game is being replayed, increment the game counter
        game_count += 1;
    }
}
