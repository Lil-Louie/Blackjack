#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H

#include <iostream>
#include <vector>
#include "Deck.h"
#include "Player.h"
#include "CardCounting.h" // Include other relevant headers

class Game {
private:
    Deck _deck;
    Player _dealer{"Dealer"};
    std::vector<Player*> _players; // Store pointers to Player
    int numPlayers;
    char gameMode;
    CardCounter _cardCounter; // Card counter instance

public:
    Game(); // Constructor
    ~Game(); // Destructor

    void startTestSimulator();
    void startMultiplayerMode();
    void showMenu();
    void showTable();
    void placeBets();
    void clearHands();
    void dealInitialCards();
    void start_shoe();
    void dealerTurn();
    void determineWinners();
    void update(const std::vector<Player>& players, const Player dealer);
    char mode() { return gameMode; }
};

#endif // BLACKJACK_GAME_H
