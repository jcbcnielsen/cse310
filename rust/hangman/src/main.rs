use std::io::*;
use std::fs::File;
use std::collections::{HashMap, HashSet};
use rand;
use clearscreen;

// Use the word list to initialize a random game of hangman
fn generate_random_game(word_list:&Vec<String>) -> (HashMap<char, Vec<usize>>, String) {

    let word = word_list[rand::random_range::<usize, _>(..word_list.len())].clone();

    let mut letters:HashMap<char, Vec<usize>> = HashMap::new();

    let mut display:String = String::new();

    for (i, letter) in word.to_uppercase().chars().enumerate() {
        if letter != ' ' {
            display.push('_');
            if letters.contains_key(&letter) {
                letters.get_mut(&letter).unwrap().push(i);
            } else {
                letters.insert(letter, vec![i]);
            }
        } else {
            display.push(' ');
        }
    }

    return (letters, display);
}

// Get user input, making sure it is only one letter
fn get_guess() -> char {
    let mut raw_guess: String = String::new();
    let valid_guess: char;

    loop {
        raw_guess.clear();

        print!("Please guess a letter: ");
        stdout().flush().unwrap();

        stdin().read_line(&mut raw_guess).expect("Failed to read input");

        if raw_guess.trim().len() == 1 {
            valid_guess = raw_guess.to_uppercase().chars().nth(0).unwrap();
            break;
        }

        println!("You may only guess a single letter at a time.");
    }

    return valid_guess;
}

// Update the terminal with the current game state
fn show_display(display: &String, misses: &usize, guesses: &HashSet<char>) {
    let mut guess_str = String::new();

    for letter in guesses.iter() {
        guess_str.push(letter.clone());
    }

    let head: &str = if misses >= &(1 as usize) {"O"} else {""};
    let shoulders: &str = if misses >= &(2 as usize) {"/|\\"} else {""};
    let torso: &str = if misses >= &(3 as usize) {"/ | \\"} else {""};
    let waist: &str = if misses >= &(4 as usize) {"/ \\"} else {""};
    let feet: &str = if misses >= &(5 as usize) {"/   \\"} else {""};

    // Avoid having everything printed extend the terminal,
    // as that can both become extremely long as the player
    // plays for longer times, and is strange behavior for
    // a game regardless
    clearscreen::clear().unwrap();

    println!(" _____\n |   |\n |   {head}\n |  {shoulders}\n | {torso}\n |  {waist}\n | {feet}\n |\n========= Guessed Letters: {guess_str}");

    if misses < &(5 as usize) {
        println!("Word to guess: {display}");
    }
}

// Update the display string on a correct guess
fn update_display(letter: (&char, &Vec<usize>), display: &mut String) {
    let (letter, indices) = letter;

    for i in indices {
        display.replace_range(i..&(i + 1 as usize), &letter.to_string());
    }
}

fn main() {
    // Need to import the words that can be chosen from,
    // but shouldn't have to repeat this on replays,
    // so leave it out of the outer loop
    let mut lines: Vec<String> = Vec::new();
    for line in BufReader::new(File::open("src/hangman_words.txt").unwrap()).lines() {
        lines.push(line.unwrap());
    }

    loop { // Outer game loop, only repeats on replays

        // Need to initialize the main game variables,
        // but shouldn't have to repeat this during the same game,
        // so leave it out of the main game loop
        let (letters, mut display) = generate_random_game(&lines);
        let mut guesses:HashSet<char> = HashSet::new();
        let mut misses: usize = 0;

        loop { // Main game loop, repeats as long as game has not ended

            // Need to show the player what is happening each pass
            // through the main game loop before input can be gathered
            show_display(&display, &misses, &guesses);
            let guess: char = get_guess();

            // Test the player's guess
            if !guesses.contains(&guess) && letters.contains_key(&guess) {
                update_display(letters.get_key_value(&guess).unwrap(), &mut display);
                guesses.insert(guess);
                println!("Good guess!");
            } else if guesses.contains(&guess) {
                println!("{guess} was already guessed. Guess a different letter.");
            } else if !letters.contains_key(&guess) {
                println!("Too bad. {guess} wasn't in the word.");
                guesses.insert(guess);
                misses += 1;
            }

            // Check if the game is over as either a win or a loss
            if !display.contains('_') {
                println!("Congratulations! You guessed it! Word: {display}");
                break;
            } else if misses >= 5 {
                // Show display is repeated here because that is
                // the only way to see the feet displayed after
                // the fifth wrong guess
                show_display(&display, &misses, &guesses);
                println!("Game Over!");
                break;
            } else {
                // Need to wait for otherwise unused input here or else
                // show_display() will instantly clear the display before
                // the player can be told if their guess was right or wrong
                print!("Press enter to continue...");
                stdout().flush().unwrap();
                stdin().read_line(&mut String::new()).expect("Failed to read input");
            }
        }

        // Check if the player wants to play a new game, breaking the outer loop if not
        let mut replay: String = String::new();

        print!("Do you want to continue? (Enter \"y\" to replay or anything else to quit) ");
        stdout().flush().unwrap();

        stdin().read_line(&mut replay).expect("Failed to read input");

        if replay.trim().to_uppercase() != "Y" {
            break
        }
    }
}
