#include "TexasHoldem.hpp"

class Card
{
private: 
	string suit;
	string face;
	string sprite; //will change once sprite class exists

public:

	//constructor
	Card();

	//setters
	void setSuit(string newSuit);
	void setFace(string newFace);
	void setSprite(string newSprite);

	//getters
	string getSuit() const;
	string getFace() const;
	string getSprite() const;

	//member functions
	Card& operator=(const Card& rhs);

};