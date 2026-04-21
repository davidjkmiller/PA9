#pragma once

//Libraries
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<string>
#include <random>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;
using std::stack;

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

	//overloaded assignment operator
	Card& operator=(const Card& rhs);

	

};

//overloaded stream insertion operator
ostream& operator<< (ostream& lhs, const Card rhs);