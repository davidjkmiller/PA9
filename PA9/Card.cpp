#include "Card.hpp"

Card::Card()
{
	suit = "\0";
	face = "\0";
	sprite = "\0";
	rank = 0;
}

//SETTERS======================================================================================
void Card::setSuit(string newSuit)
{
	suit = newSuit;
}

void Card::setFace(string newFace)
{
	face = newFace;
}

void Card::setSprite(string newSprite)
{
	sprite = newSprite;
}

void Card::setRank(int newRank)
{
	rank = newRank;
}


//GETTERS======================================================================================
string Card::getSuit() const
{
	return suit;
}

string Card::getFace() const
{
	return face;
}

string Card::getSprite() const
{
	return sprite;
}

int Card::getRank() const
{
	return rank;
}

//MEMBER FUNCTIONS=============================================================================
Card& Card::operator=(const Card& rhs)
{
	suit = rhs.suit;
	face = rhs.face;
	sprite = rhs.sprite;

	return (*this);
}

//=============================================================================================

ostream& operator<<(ostream& lhs, const Card rhs)
{
	lhs << rhs.getFace() << " of " << rhs.getSuit() << endl;

	return lhs;
}
