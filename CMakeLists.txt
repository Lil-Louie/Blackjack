cmake_minimum_required(VERSION 3.26)
project(Blackjack)

set(CMAKE_CXX_STANDARD 17)

add_executable(Blackjack main.cpp Card.cpp CardCounting.cpp Deck.cpp Suit.h Player.cpp Game.cpp)
