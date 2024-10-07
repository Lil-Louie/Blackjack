#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Game.h"


// Constructor
Game::Game() : numPlayers(0), gameMode() {
    showMenu();
    std::cout << std::setw((40) / 2) << "Mode: "; // Centered title
    std::cin >> gameMode;
}


// Destructor
Game::~Game() {
    // Clean up allocated players
    for (auto player : _players) {
        delete player; // Free the memory allocated for each player
    }
}

void Game::showMenu() {
    const int width = 40;

    std::cout << std::string(width, '=') << "\n";
    std::cout << std::setw((width + 24) / 2) << "Welcome to Blackjack\n"; // Centered title
    std::cout << std::string(width, '=') << "\n";
    std::cout << std::setw((width + 15) / 2) << "Choose mode:\n"; // Centered prompt
    std::cout << std::setw((width + 28) / 2) << "1. Single Player vs CPU\n"; // Centered option 1
    std::cout << std::setw((width + 22) / 2) << "2. Multiplayer\n"; // Centered option 2
    std::cout << std::setw((width + 21) / 2) << "3. Test Simulator\n"; // Centered option 3
    std::cout << std::setw((width + 10) / 2) << "4. Exit\n"; // Centered option 4
    std::cout << std::string(width, '=') << "\n";

}



/*
void Game::startMultiplayerMode() {
    std::cout << "How many players do you want to play with? (1-6): ";
    std::cin >> numPlayers;

    // Validate input
    while (numPlayers < 1 || numPlayers > 6) {
        std::cout << "Please enter a valid number of players (1-6): ";
        std::cin >> numPlayers;
    }

    // Add players to the vector
    for (int i = 1; i <= numPlayers; ++i) {
        _players.push_back(new Player("Player " + std::to_string(i)));
    }
}
*/

void Game::startTestSimulator() {
    int games;
    int stratgey;
    int bankroll;
    int betSpread;

    std::cout << "Starting Test Simulator...\n";

}


void Game::start_shoe() {


    if (gameMode == '1') {
        numPlayers = 1; // Only one player
        _players.push_back(new BasicStrategyPlayer("Basic Strategy Player"));
    } else if (gameMode == '2') {
        std::cout << "How many players do you want to play with? (1-6): ";
        std::cin >> numPlayers;

        // Validate input
        while (numPlayers < 1 || numPlayers > 6) {
            std::cout << "Please enter a valid number of players (1-6): ";
            std::cin >> numPlayers;
        }

        // Add players to the vector
        for (int i = 1; i <= numPlayers; ++i) {
            _players.push_back(new Player("Player " + std::to_string(i)));
        }
    } else if (gameMode == '3') {
        startTestSimulator();
    } else {
        std::cout << "Invalid choice, exiting.\n";
        exit(1); // Exit if invalid choice
    }

    while (true) {
        if (_deck.getRemainingCards() <= 8) {
            std::cout << "Only " << _deck.getRemainingCards() << " cards left in the shoe. Ending game.\n";
            break; // End game if 8 or fewer cards are left
        }

        placeBets(); // Place bets before dealing cards
        dealInitialCards(); // Deal initial cards to players and dealer

        std::cout << "Current Card Count: " << _cardCounter.getCount() << std::endl;

        // Player turns
        for (auto &player: _players) {
            if (player->getScore() == 21) {
                std::cout << player->getName() << " has a Blackjack! They win with a payout of 1.5x their bet!\n";
                player->winBet();
                continue;
            }
            bool playerFinished = false;

            while (!playerFinished) {
                _dealer.showHand();
                player->showHand(); // Show player's hand

                char action;

                if (gameMode == 3) {
                    BasicStrategyPlayer *strategyPlayer = dynamic_cast<BasicStrategyPlayer *>(player);
                    if (strategyPlayer) {
                        // Get the dealer's face-up card value
                        int dealerCardValue = _dealer.getHand()[0].getValue();
                        action = strategyPlayer->decideAction(dealerCardValue);
                    } else {
                        std::cout << player->getName() << ", do you want to (H)it or (S)tand? ";
                        std::cin >> action;
                    }

                    // Process action
                    if (action == 'H' || action == 'h') {
                        player->addCard(_deck.drawCard());
                        if (player->getScore() > 21) {
                            std::cout << player->getName() << " busts with a score of " << player->getScore() << "!\n";
                            player->loseBet(); // Player loses their bet
                            playerFinished = true; // End turn if player busts
                        }
                    } else if (action == 'S' || action == 's') {
                        playerFinished = true; // End turn if player stands
                    } else {
                        std::cout << "Invalid input. Please enter H or S.\n";
                    }
                } else {
                    // Multiplayer logic here...
                }
            }
        }

        dealerTurn(); // Dealer's turn logic
        determineWinners(); // Determine winners

        clearHands();

        // Check if the shoe should be reset
        if (_deck.getRemainingCards() <= 8) {
            std::cout << "Only " << _deck.getRemainingCards() << " cards left in the shoe. Ending game.\n";
            break; // End game if 8 or fewer cards are left
        }
    }
}

void Game::placeBets() {
    for (auto player : _players) {
        showTable();
        int betAmount;
        std::cout << player->getName();
        std::cout << ", enter your bet amount: ";
        std::cin >> betAmount;
        player->placeBet(betAmount);
    }
}



void Game::dealInitialCards() {
    int set = 0;
    while (set != 2) {
        for (auto player : _players) {
            player->addCard(_deck.drawCard());
            _cardCounter.Hi_LO_updateCount(player->getHand().back()); // Update count for player's card
        }
        _dealer.addCard(_deck.drawCard());
        _cardCounter.Hi_LO_updateCount(_dealer.getHand().back()); // Update count for dealer's card
        set++;
    }
}



void Game::dealerTurn() {
    std::cout << "Dealer's turn...\n";
    while (_dealer.getScore() < 17) { // Dealer hits until reaching 17 or more
        _dealer.addCard(_deck.drawCard());
    }
    _dealer.showHand();
}

void Game::determineWinners() {
    int dealerScore = _dealer.getScore();
    std::cout << "Dealer's score: " << dealerScore << "\n";

    for (auto player : _players) {
        int playerScore = player->getScore();
        std::cout << player->getName() << "'s score: " << playerScore << "\n";

        if (playerScore > 21) {
            std::cout << player->getName() << " loses (bust).\n";
            player->loseBet();
        } else if (dealerScore > 21 || playerScore > dealerScore) {
            std::cout << player->getName() << " wins!\n";
            player->winBet(); // Player wins their bet
        } else if (playerScore < dealerScore) {
            std::cout << player->getName() << " loses.\n";
            player->loseBet(); // Player loses their bet
        } else {
            std::cout << player->getName() << " pushes (tie).\n";
            player->pushBet(); // Push means return the bet
        }
    }
}




void Game::clearHands() {
    // Clear hands and scores for dealer
    _dealer.clearHand();

    // Clear hands and scores for all players
    for (auto player : _players) {
        player->clearHand();
    }
}
/*
void Game::update(const std::vector<Player>& players, const Player dealer) {
    // Print Dealer's Cards
    std::cout << std::setw(15) << "Dealer's Cards: ";
    dealer.getHand(); // Note: you might want to implement a way to print the dealer's hand

    // Print Players' Status
    std::cout << std::setw(15) << "Player"
              << std::setw(15) << "Bankroll"
              << std::setw(15) << "Current Bet"
              << std::setw(30) << "Cards" << std::endl;

    for (const auto& player : players) {
        std::cout << std::setw(15) << player.getName()
                  << std::setw(15) << "$" << player.getBankroll()
                  << std::setw(15) << "$" << player.getCurrentBet()
                  << std::setw(30);
        for (const auto& card : player.getHand()) {
            std::cout << card.toString() << " "; // Adjusted to use toString() for cards
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}*/



void Game::showTable(){

    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "-Running Count: " + std::to_string(_cardCounter.getCount()) + "                                       ";
    std:: cout << _dealer.getName();
    std::cout << "                                                           -\n";
    std::cout << "-                                              ________________________                                                  -\n";
    std::cout << "-                                                         ";
    std:: cout << _dealer.getScore();
    std::cout << "                                                              -\n";
    for (int i = 0; i < 18; ++i) {
        std::cout << "-                                                                                                                        -\n";
    }
    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    // Print Bet amounts
    for (int i = 0; i < 6; ++i) {
        std::cout << "|Bet:$" << std::setw(5) << std::to_string(_players[i]->getCurrentBet()) + "    |          ";
    }
    std::cout << "|\n";
    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    // Print Player Names
    for (int i = 0; i < 6; ++i) {
        std::cout << "|" << std::setw(10) << _players[i]->getName() << "|          ";
    }
    std::cout << "\n";

    // Print "Bankroll" labels
    for (int i = 0; i < 6; ++i) {
        std::cout << "|" << std::setw(10) << "Bankroll  |          ";
    }
    std::cout << "\n";

    // Print Player Bankrolls
    for (int i = 0; i < 6; ++i) {
        std::cout << "|$" << std::setw(6) << _players[i]->getBankroll() << "   |          ";
    }
    std::cout << "\n";

    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";

}