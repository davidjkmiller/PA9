// Asked AI to go through and organize/comment the code without modifying it
// Prompt: Can you go through my code and organize it to be more readable and add comments where necessary. Do not add explanations for things like "why a destructor is needed", do not modify the actual code.
// Console output was standardized and some clarifying comments were added

#include <iostream>
#include <cassert>
#include <string>
#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

using std::cout;
using std::endl;
using std::string;

// Helper function to easily create specific cards for testing
Card makeCard(string suit, string face, int rank) {
    Card c;
    c.setSuit(suit);
    c.setFace(face);
    c.setRank(rank);
    return c;
}

void testCardLogic() {
    cout << "Testing Card setters/getters... ";
    Card c = makeCard("Spades", "Ace", 1);
    assert(c.getSuit() == "Spades");
    assert(c.getFace() == "Ace");
    assert(c.getRank() == 1);
    cout << "PASS" << endl;
}

void testDeckLogic() {
    cout << "Testing Deck shuffling and drawing... ";
    Deck deck;
    deck.shuffleDeck();

    Card topCard = deck.drawCard();
    // Verify that a valid card was drawn
    assert(topCard.getFace() != "\0");
    assert(topCard.getSuit() != "\0");
    cout << "PASS" << endl;
}

void testScoringLogic() {
    HumanPlayer player;
    Card board[5];

    cout << "Testing Scoring - High Card... ";
    player.setHand(0, makeCard("Hearts", "Two", 2));
    player.setHand(1, makeCard("Spades", "Seven", 7));
    board[0] = makeCard("Clubs", "Nine", 9);
    board[1] = makeCard("Diamonds", "Jack", 11);
    board[2] = makeCard("Hearts", "King", 13);
    board[3] = makeCard("Spades", "Four", 4);
    board[4] = makeCard("Clubs", "Five", 5);
    assert(player.score(board) == 0); // 0 corresponds to high card
    cout << "PASS" << endl;

    cout << "Testing Scoring - One Pair... ";
    player.setHand(0, makeCard("Hearts", "Ace", 1));
    player.setHand(1, makeCard("Spades", "Ace", 1));
    board[0] = makeCard("Clubs", "Two", 2);
    board[1] = makeCard("Diamonds", "Three", 3);
    board[2] = makeCard("Hearts", "Four", 4);
    board[3] = makeCard("Spades", "Five", 5);
    board[4] = makeCard("Clubs", "Nine", 9);
    assert(player.score(board) == 1); // 1 corresponds to Pair
    cout << "PASS" << endl;

    cout << "Testing Scoring - Full House... ";
    player.setHand(0, makeCard("Hearts", "Ten", 10));
    player.setHand(1, makeCard("Spades", "Ten", 10));
    board[0] = makeCard("Clubs", "Ten", 10);
    board[1] = makeCard("Diamonds", "Four", 4);
    board[2] = makeCard("Hearts", "Four", 4);
    board[3] = makeCard("Spades", "Two", 2);
    board[4] = makeCard("Clubs", "Nine", 9);
    assert(player.score(board) == 6); // 6 corresponds to Full House
    cout << "PASS" << endl;

    cout << "Testing Scoring - Royal Flush... ";
    player.setHand(0, makeCard("Spades", "Ace", 1));
    player.setHand(1, makeCard("Spades", "King", 13));
    board[0] = makeCard("Spades", "Queen", 12);
    board[1] = makeCard("Spades", "Jack", 11);
    board[2] = makeCard("Spades", "Ten", 10);
    board[3] = makeCard("Hearts", "Two", 2);
    board[4] = makeCard("Clubs", "Three", 3);
    assert(player.score(board) == 9); // 9 corresponds to Royal Flush
    cout << "PASS" << endl;
}

void runAllTests() {
    cout << "--- RUNNING TEXAS HOLD'EM UNIT TESTS ---" << endl;

    testCardLogic();
    testDeckLogic();
    testScoringLogic();

    cout << "--- ALL TESTS COMPLETED SUCCESSFULLY ---" << endl;
}