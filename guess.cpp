#include <iostream> // for input og output
#include <cstdlib> // for random og srand
#include <ctime> // for tid

int main() { // hovedfunksjon

    std::srand(std::time(0)); // random tall hver gang siden vi bruker tid
    int secretNumber = std::rand() % 100 + 1; // 1 to 100
    int guess; // tallet som brukeren gjetter
    int attempts = 0; // antall forsøk

    std::cout << "Welcome to the Number Guessing Game!\n"; // into til spillet + ny linje

    do { // loop til brukeren gjetter riktig
        std::cout << "Enter your guess (1-100): ";
        std::cin >> guess;
        attempts++;

        if (guess > secretNumber) {
            std::cout << "Too high! Try again.\n";
        } else if (guess < secretNumber) {
            std::cout << "Too low! Try again.\n";
        } else {
            std::cout << "🎉 Correct! You guessed it in " << attempts << " tries.\n";
        }
    } while (guess != secretNumber);

    return 0;
}
