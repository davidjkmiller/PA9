#pragma once


//Header
#include "Card.hpp"

class Player //ABSTRACT
{
protected:
	Card hand[2];
	string playerID; //name specifier of player so that game messages are unique to the player
	float balance; //CHANGE TO TOKENS
	int isDealer; //if 0, not dealer, if 1, is dealer
	int hasFolded; //if 0, has not folded, if 1, has folded
	int handScore; //variable that sums the ranks of the cards of your greatest score; only used if players tie to determine the winner

public:

	//constructor
	Player();

	//default destructor
	virtual ~Player();

	//setters
	void setHand(int cardNum, Card newCard);
	void setPlayerID(string ID);
	void setDealer(int button);
	void setBalance(float changeInBalance);
	void setFoldStatus(int fold);
	void setHandScore(int newScore);

	//getters
	Card* getHand();
	string getPlayerID() const;
	int checkIfDealer() const;
	float getBalance() const;
	int getFoldStatus() const;
	int getHandScore() const;


	//member functions
	void viewHand();

	// Added in uiChoice to work with mouse events
	virtual float play(float& prizePool, float& currentBet, Card* Board, int uiChoice = 0) = 0;

	//scores the players hand and returns an integer based on how good the hand is, and prints what hand the player had
	int score(Card* board);

	//Hand indentification functions-------

	int isRoyalFlush(Card* combo);

	int isStraightFlush(Card* combo);

	int isFourOfAKind(Card* combo);

	int isFullHouse(Card* combo);

	int isFlush(Card* combo);

	/*					*** AI USE (sort of) *** 
		used the prompt: "For my function isStraight() in player.hpp,
		what is an efficient way to code it so that it recognizes the
		poker hand known as a straight?"

		When I used the prompt it told me a whole bunch of things, 
		but the only thing I implemented was its idea to use another
		array for ranks and to manually assign each face a rank.
	*/
	int isStraight(Card* combo);

	int isThreeOfAKind(Card* combo);

	int isTwoPair(Card* combo);

	int isPair(Card* combo);
};

class HumanPlayer : public Player
{

public: 
	//constructor
	using Player::Player;
	
	//member functions
	// Added in uiChoice
	float play(float& prizePool, float& currentBet, Card* Board, int uiChoice = 0) override;
};

class CPU : public Player
{
public:
	//constructor
	using Player::Player;

	//member functions

	//currently just picks a random number between 1 and 3 to decide whether to call, raise, or fold
	// Added in uiChoice
	float play(float& prizePool, float& currentBet, Card* Board, int uiChoice = 0) override;
};

void pressAnyKey();