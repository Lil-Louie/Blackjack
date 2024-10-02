#include <iostream>
#ifndef BLACKJACK_SUIT_H
#define BLACKJACK_SUIT_H

class Suit {
private:
    std::string _name;
public:
    Suit(const std::string& name) : _name(name) {}

    std::string getName() const {
        return _name;
    }
};



#endif //BLACKJACK_SUIT_H
