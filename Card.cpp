#include "Card.h"


int Card::getValue() const {
    if (_rank >= 2 && _rank <= 10) {
        return _rank; // Number cards
    } else if (_rank >= 11 && _rank <= 13) {
        return 10; // Face cards
    } else {
        return 11; // Ace, but will handle later for flexibility
    }
}


std::string Card::toString() const {
    std::string rankStr = (_rank == 1) ? "Ace" :
                          (_rank == 11) ? "Jack" :
                          (_rank == 12) ? "Queen" :
                          (_rank == 13) ? "King" :
                          std::to_string(_rank);
    return rankStr + " of " + _suit.getName();
}
