#include <iostream>
#include "Card.h"
#ifndef BLACKJACK_CARDCOUNTING_H
#define BLACKJACK_CARDCOUNTING_H


class CardCounter {
private:
    int _count; // Current count

public:
    CardCounter() : _count(0) {}
    int getCount() const { return _count; }
    void reset() { _count = 0; }

    void updateCount(Card card) {
        int rank = card.getRank();
        if (rank >= 2 && rank <= 6) {
            _count += 1; // Low cards
        } else if (rank >= 10) {
            _count -= 1; // High cards (10, J, Q, K, A)
        }
    }

};


#endif //BLACKJACK_CARDCOUNTING_H
