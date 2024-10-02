#include <iostream>
#include "Card.h"

#ifndef BLACKJACK_PLAYER_H
#define BLACKJACK_PLAYER_H



class Player {
private:
    std::string _name;
    std::vector<Card> _hand;
    int _bankroll; // Player's total money
    int _currentBet; // Current bet for the round

public:
    Player(const std::string& name, int initialBankroll = 1000)
            : _name(name), _bankroll(initialBankroll), _currentBet(0) {}

    virtual ~Player() {} // Virtual destructor

    std::string getName() const { return _name; }
    void addCard(const Card& card) { _hand.push_back(card); }
    int getBankroll() const { return _bankroll; }
    int getCurrentBet() const { return _currentBet; }
    int getScore() const;
    void showHand() const;
    const std::vector<Card>& getHand() const { return _hand; }
    void clearHand() { _hand.clear(); }


    // Betting methods
    void placeBet(int amount);
    void winBet(bool blackjack = false);
    void pushBet();
    void loseBet() { _currentBet = 0; }
};


class BasicStrategyPlayer : public Player {
public:
    BasicStrategyPlayer(const std::string& name) : Player(name) {}

    char decideAction(int dealerCardValue) {
        int playerScore = getScore();

        if (playerScore <= 11) {
            return 'H'; // Always hit
        } else if (playerScore >= 17) {
            return 'S'; // Always stand
        } else if (playerScore >= 12 && playerScore <= 16) {
            if (dealerCardValue >= 7) {
                return 'H'; // Hit against dealer's strong card
            } else {
                return 'S'; // Stand against dealer's weak card
            }
        }

        return 'H'; // Default to hit (safeguard)
    }
};


#endif //BLACKJACK_PLAYER_H
