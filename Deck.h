#include <iostream>
#include "Suit.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include <memory>

#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H

class Deck {
private:
    std::vector<Card> _cards;
    size_t _currentCardIndex; // Track the current card index for drawing

public:
    Deck() : _currentCardIndex(0) {
        std::vector<Suit> suits = { Suit("Hearts"), Suit("Diamonds"), Suit("Clubs"), Suit("Spades") };
        for (const auto& suit : suits) {
            for (int rank = 1; rank <= 13; ++rank) {
                _cards.emplace_back(rank, suit);
            }
        }
        shuffle();
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(_cards.begin(), _cards.end(), g);
        _currentCardIndex = 0; // Reset the index after shuffling
    }

    Card drawCard() {
        if (_currentCardIndex >= _cards.size()) {
            throw std::out_of_range("No more cards in the deck.");
        }
        return _cards[_currentCardIndex++]; // Return the next card and increment the index
    }

    bool isEmpty() const {
        return _currentCardIndex >= _cards.size();
    }

    int getRemainingCards() const {
        return _cards.size() - _currentCardIndex; // Return remaining cards
    }
};

#endif //BLACKJACK_DECK_H
