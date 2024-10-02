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
    bool singlePlayerMode;
    CardCounter _cardCounter; // Card counter instance

public:
    Game(); // Constructor
    ~Game(); // Destructor

    void placeBets();
    void clearHands();
    void dealInitialCards();
    void play();
    void dealerTurn();
    void determineWinners();
    void update(const std::vector<Player>& players, const Player dealer);
};

#endif // BLACKJACK_GAME_H
