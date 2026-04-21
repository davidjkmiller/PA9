#pragma once

//Headers
#include "Card.hpp"
#include "Player.hpp"



//Class=================================================================================

class Deck
{
private:
	Card deckInOrder[52];
	stack<Card> deck;

public:
	
	//constructor
	Deck();

	//getter
	const Card* getDeckInOrder();



	//member functions

	//Fills deck[] with 52 cards from a standard deck in order and assigns the correct sprite to each card
	void initDeck();

	//Randomly swaps cards 260 times in the deck; equivalent to 5 shuffles (5 * 52) and then pushes the shuffled cards into the deck stack
	void shuffleDeck();

	//Deals two cards to each player, accepts Player* so that the function works for both Humans and CPUs
	void deal(Player* p1, Player* p2, Player* p3, Player* p4);

	Card drawCard();


};
