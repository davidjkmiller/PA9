#include "Deck.hpp"

//CONSTRUCTOR=================================================================================

Deck::Deck()
{
	this->initDeck();
}

//GETTERS======================================================================================
const Card* Deck::getDeckInOrder()
{
	return deckInOrder;
}

//MEMBER FUNCTIONS=============================================================================
void Deck::initDeck()
{
	int suitIndex = 0;
	int faceIndex = 0;
	int spriteIndex = 0; //sprite index also functions as the deck index since they increment at the same time

	//Initialize suit, face, and sprite array
	string suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };

	const string face[13] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight",
		"Nine", "Ten", "Jack", "Queen", "King" };

	//sprite arr[52]: has all the sprites in the order corresponding to the arrays above
	/*For now lets call it ->*/ string sprite[52] = { "\0" };

	for (suitIndex = 0; suitIndex < 4; suitIndex++)
	{
		for (faceIndex = 0; faceIndex < 13; faceIndex++)
		{
			deckInOrder[spriteIndex].setSuit(suit[suitIndex]);
			deckInOrder[spriteIndex].setFace(face[faceIndex]);
			deckInOrder[spriteIndex].setSprite(sprite[spriteIndex]);

			++spriteIndex;
		}
	}


}

void Deck::shuffleDeck()
{
	srand((unsigned)time(NULL)); /* seed random-number generator */

	Card tempCard;
	int i = 0;
	int j = 0;

	//goes through the deck 260 times and swaps cards randomly (equivalent of 5 shuffles)
	for (int count = 0; count < 5; ++count)
	{
		for (int i = 0; i < 52; ++i)
		{

			j = rand() % 52;
			tempCard = deckInOrder[i];
			deckInOrder[i] = deckInOrder[j];
			deckInOrder[j] = tempCard;


		}
	}

	//push cards starting from the bottom of deckInOrder into the deck stack to maintain the order
	for (i = 51; i >= 0; --i)
	{
		deck.push(deckInOrder[i]);
	}
}

void Deck::deal(Player* p1, Player* p2, Player* p3, Player* p4)
{
	int count = 0;

	//draw two cards from the deck for each player
	while (count < 2)
	{
		p1->setHand(count, drawCard());						
		p2->setHand(count, drawCard());
		p3->setHand(count, drawCard());
		p4->setHand(count, drawCard());

		++count;
	}
}

Card Deck::drawCard()
{
	Card card = deck.top(); //get top card
	deck.pop();				//discard top card

	return card;
}



