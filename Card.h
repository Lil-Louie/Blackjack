#include <iostream>
#include "Suit.h"

#ifndef BLACKJACK_CARD_H
#define BLACKJACK_CARD_H


class Card {
private:
    int _rank;
    Suit _suit;

public:
    Card(int rank, const Suit& suit) : _suit(suit) {
        if (rank < 1 || rank > 13) {
            throw std::invalid_argument("Rank must be between 1 and 13");
        }
        _rank = rank;
    }

    int getValue() const;
    Suit getSuit() const { return _suit; }
    int getRank() { return _rank; }
    std::string toString() const;

};



#endif //BLACKJACK_CARD_H
