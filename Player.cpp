#include "Player.h"


int Player::getScore() const {
    int total = 0;
    int aces = 0;

    for (const auto& card : _hand) {
        total += card.getValue();
        if (card.getValue() == 11) {
            aces++;
        }
    }

    // Adjust for Aces if total > 21
    while (total > 21 && aces > 0) {
        total -= 10; // Count Ace as 1 instead of 11
        aces--;
    }

    return total;
}

void Player::showHand() const {
    std::cout << _name << "'s hand: ";
    for (const auto& card : _hand) {
        std::cout << card.toString() << ", ";
    }
    std::cout << "Score: " << getScore() << std::endl;
}

// Betting methods
void Player::placeBet(int amount) {
    if (amount <= _bankroll && amount > 0) {
        _currentBet = amount;
        _bankroll -= amount;
    } else {
        std::cout << "Invalid bet amount.\n";
    }
}

void Player::winBet(bool blackjack) {
    if (blackjack) {
        _bankroll += _currentBet * 1.5; // Blackjack payout
    } else {
        _bankroll += _currentBet * 2; // Regular win payout
    }
    _currentBet = 0; // Reset current bet
}

void Player::pushBet() {
    _bankroll += _currentBet; // Push means return the bet
    _currentBet = 0; // Reset current bet
}


