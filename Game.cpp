#include "Game.h"

// Constructor
Game::Game() : numPlayers(0), singlePlayerMode(false) {
    char modeChoice;
    std::cout << "Choose mode:\n1. Single Player vs CPU\n2. Multiplayer\n3.Test Simulator";
    std::cin >> modeChoice;

    if (modeChoice == '1') {
        singlePlayerMode = true;
        numPlayers = 1; // Only one player
        // Use the Basic Strategy player
        _players.push_back(new BasicStrategyPlayer("Basic Strategy Player"));
    } else if (modeChoice == '2') {
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
    } else {
        std::cout << "Invalid choice, exiting.\n";
        exit(1); // Exit if invalid choice
    }
}

// Destructor
Game::~Game() {
    // Clean up allocated players
    for (auto player : _players) {
        delete player; // Free the memory allocated for each player
    }
}

void Game::placeBets() {
    for (auto player : _players) {
        std::cout << player->getName() << ", your current bankroll is $" << player->getBankroll() << ".\n";
        int betAmount;
        std::cout << "Enter your bet amount: ";
        std::cin >> betAmount;
        player->placeBet(betAmount);
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

void Game::dealInitialCards() {
    int set = 0;
    while (set != 2) {
        for (auto player : _players) {
            player->addCard(_deck.drawCard());
            _cardCounter.updateCount(player->getHand().back()); // Update count for player's card
        }
        _dealer.addCard(_deck.drawCard());
        _cardCounter.updateCount(_dealer.getHand().back()); // Update count for dealer's card
        set++;
    }
}

void Game::play() {
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

                if (singlePlayerMode) {
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

void Game::dealerTurn() {
    std::cout << "Dealer's turn...\n";
    while (_dealer.getScore() < 17) { // Dealer hits until reaching 17 or more
        _dealer.addCard(_deck.drawCard());
    }
    _dealer.showHand(); // Show dealer's hand
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
