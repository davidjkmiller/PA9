#pragma once


//Header
#include "Card.hpp"

class Player //ABSTRACT
{
protected:
	Card hand[2];
	float balance; //CHANGE TO TOKENS
	int isDealer; //if 0, not dealer, if 1, is dealer
	int hasFolded; //if 0, has not folded, if 1, has folded
	//thinking about adding a player ID attribute so that singleplayer messages are specific to the player



public:

	//constructor
	Player();

	//default destructor
	~Player();

	//setters
	void setHand(int cardNum, Card newCard);
	void setDealer(int button);
	void setBalance(float changeInBalance);
	void setFoldStatus(int fold);

	//getters
	Card* getHand();
	int checkIfDealer();
	float getBalance();
	int getFoldStatus();


	//member functions
	void viewHand();


	virtual float play(float &currentBet, Card* Board) = 0;
};

class HumanPlayer : public Player
{

public: 
	//constructor
	using Player::Player;
	
	//member functions
	float play(float& currentBet, Card* Board) override;
};

class CPU : public Player
{
public:
	//constructor
	using Player::Player;

	//member functions

	//currently just picks a random number between 1 and 3 to decide whether to call, raise, or fold
	float play(float& currentBet, Card* Board) override;
};

void pressAnyKey();