#include "TexasHoldem.hpp"
#include "Card.hpp"

class Deck
{
private:
	Card deck[52];

public:
	
	//getter
	const Card* getDeck();


	//member functions

	//Fills deck[] with 52 cards from a standard deck in order and assigns the correct sprite to each card
	void initDeck();

	//Randomly swaps cards 260 times in the deck; equivalent to 5 shuffles (5 * 52)
	void shuffleDeck();


};
