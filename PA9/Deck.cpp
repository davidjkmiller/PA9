#include "Deck.hpp"

//GETTERS======================================================================================
const Card* Deck::getDeck()
{
	return deck;
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
			deck[spriteIndex].setSuit(suit[suitIndex]);
			deck[spriteIndex].setFace(face[faceIndex]);
			deck[spriteIndex].setSprite(sprite[spriteIndex]);

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
			tempCard = deck[i];
			deck[i] = deck[j];
			deck[j] = tempCard;


		}
	}
}



