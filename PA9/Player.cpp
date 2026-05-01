#include "Player.hpp"

//PLAYER///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Constructor========================================================================
Player::Player()
{
	Card nullCard;

	playerID = "\0";

	hand[0] = hand[1] = nullCard;

	balance = 1000.00;

	isDealer = 0;

	hasFolded = 0;

	handScore = 0;
}

//Destructor========================================================================
Player::~Player()
{
}

//Setters===========================================================================
void Player::setHand(int cardNum, Card newCard)
{
	hand[cardNum] = newCard;
}

void Player::setPlayerID(string ID)
{
	playerID = ID;
}

void Player::setDealer(int button)
{
	isDealer = button;
}

void Player::setBalance(float changeInBalance)
{
	balance += changeInBalance;
}

void Player::setFoldStatus(int fold)
{
	hasFolded = fold;
}

void Player::setHandScore(int newScore)
{
	handScore = newScore;
}

//Getters===========================================================================
Card* Player::getHand()
{
	return hand;
}

string Player::getPlayerID() const
{
	return playerID;
}

int Player::checkIfDealer() const
{
	return isDealer;
}

float Player::getBalance() const
{
	return balance;
}

int Player::getFoldStatus() const
{
	return hasFolded;
}

int Player::getHandScore() const
{
	return handScore;
}

//Member functions
void Player::viewHand()
{
	cout << getHand()[0] << getHand()[1] << endl;
}

int Player::score(Card* board)
{
	Card combo[5], cards[7];

	//have cards[] hold both the hole cards and the community cards
	cards[0] = hand[0];
	cards[1] = hand[1];
	cards[2] = board[0];
	cards[3] = board[1];
	cards[4] = board[2];
	cards[5] = board[3];
	cards[6] = board[4];

	//AI GENERATED CODE FROM HERE--------------------------------------------

	// Generate all 5-card combinations from 7 cards (C(7,5) = 21 combinations)
	// We'll use indices to represent which 5 cards to take from the 7
	int bestScore = 0;

	// Iterate through all possible 5-card combinations
	// Using 4 nested loops for the first 4 indices, the 5th is implicit
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			for (int k = j + 1; k < 5; ++k)
			{
				for (int l = k + 1; l < 6; ++l)
				{
					for (int m = l + 1; m < 7; ++m)
					{
						// Fill combo array with the current combination
						combo[0] = cards[i];
						combo[1] = cards[j];
						combo[2] = cards[k];										//I used AI for this portion of code because I was unaware of the mathematical
						combo[3] = cards[l];										//algorithm which would create each 5 card combo of the 7 cards available.
						combo[4] = cards[m];										// My prompt: In my score function in Player.cpp, I need to sort every
																					//combination of 5 cards from the 7 cards within the cards array into the 
						// Check each hand type and update best score				//into the combo array. Can you code this for me?

						int currentScore = 0;

						if (isRoyalFlush(combo))
						{
							currentScore = 9;
						}
						else if (isStraightFlush(combo))
						{
							currentScore = 8;
						}
						else if (isFourOfAKind(combo))
						{
							currentScore = 7;
						}
						else if (isFullHouse(combo))
						{
							currentScore = 6;
						}
						else if (isFlush(combo))
						{
							currentScore = 5;
						}
						else if (isStraight(combo))
						{
							currentScore = 4;
						}
						else if (isThreeOfAKind(combo))
						{
							currentScore = 3;
						}
						else if (isTwoPair(combo))
						{
							currentScore = 2;
						}
						else if (isPair(combo))
						{
							currentScore = 1;
						}
						// else currentScore remains 0 (high card)

						// Keep track of the best score found
						if (currentScore > bestScore)
						{
							bestScore = currentScore;
						}
					}
				}
			}
		}
	}

	return bestScore;
	//TO HERE-------------------------------------------------------------------------------------
}	

int Player::isRoyalFlush(Card* combo)
{
	//if flush and an ace-high straight
	if (isStraightFlush(combo) == 8 && getHandScore() == 47)
	{
		//no handScore needs to be set since a royal flush is a set deck
		return 9;
	}

	return 0;
}

int Player::isStraightFlush(Card* combo)
{
	if (isFlush(combo) == 5 && isStraight(combo) == 4)
	{
		return 8;
	}

	return 0;
}

int Player::isFourOfAKind(Card* combo)
{
	int count = 0;

	for (int i = 0; i < 2; ++i)
	{
		count = 0;
		setHandScore(0);

		for (int j = i; j < 5; ++j)
		{
			if (combo[i].getFace() == combo[j].getFace()) ++count;
		}


		if (count == 4)
		{
			//handScore only needs to be the rank of the card that formed the four of a kind
			setHandScore(combo[i].getRank());
			return 7;
		}
	}

	return 0;
}

int Player::isFullHouse(Card* combo)
{
	//finds the first two different cards in the hand and if there is a card not matching either, there is no full house
	//the function also counts the number of both cards, and if they are 3 and 2, there is a full house

	string face1 = "\0", face2 = "\0";
	int count1 = 1, count2 = 0, rank1 = 0, rank2 = 0;

	face1 = combo[0].getFace();
	rank1 = combo[0].getRank();

	for (int i = 1; i < 5; ++i)
	{
		//sets face2
		if (face2 == "\0" && combo[i].getFace() != face1)
		{
			face2 = combo[i].getFace();
		}

		//keeps track of face1 occurrences
		if (combo[i].getFace() == face1)
		{
			++count1;
		}
		//keeps track of face2 occurrences
		else if (combo[i].getFace() == face2)
		{
			++count2;
		}

		//checks for a third unique card
		if (face1 != "\0" && face2 != "\0")
		{
			if (combo[i].getFace() != face1 && combo[i].getFace() != face2) return 0;
		}
	}

	//only one count variable needs to be checked
	if (count1 < 2 || count1 > 3)
	{
		setHandScore(0);
		return 0;
	}
	else
	{
		//handScore must only be the rank of whatever card made the three of a kind
		if (count1 == 3)
		{
			setHandScore(rank1);
		}
		else setHandScore(rank2);

		return 6;
	}
}

int Player::isFlush(Card* combo)
{
	string suitTarget;
	int highCard = 0;

	suitTarget = combo[0].getSuit();

	for (int i = 1; i < 5; ++i)
	{
		if (combo[i].getRank() > highCard) highCard = combo[i].getRank();

		if (combo[i].getSuit() != suitTarget) return 0;
	}

	setHandScore(highCard);
	return 5;
}

int Player::isStraight(Card* combo)
{
	int rank[5] = { 0, 0, 0, 0, 0 };
	int hold = 0;

	//assign a rank to each card
	for (int i = 0; i < 5; ++i)
	{
		rank[i] = combo[i].getRank();
	}

	//bubble sort rank[]
	for (int j = 0; j < 5; ++j)
	{
		for (int k = j + 1; k < 5; ++k)
		{
			if (rank[j] > rank[k])
			{
				hold = rank[k];
				rank[k] = rank[j];
				rank[j] = hold;
			}
		}
	}

	//check if it's an ace-high straight (10-J-Q-K-A)
	if (rank[0] == 1 && rank[1] == 10 && rank[2] == 11 && rank[3] == 12 && rank[4] == 13)
	{
		setHandScore(47);
		return 4;
	}

	//check for a normal straight
	for (int j = 0; j < 4; ++j)
	{
		if (rank[j] != rank[j + 1] - 1) return 0;
	}

	setHandScore(combo[0].getRank() + combo[1].getRank() + combo[2].getRank() + combo[3].getRank() + combo[4].getRank());
	return 4;
	
}

int Player::isThreeOfAKind(Card* combo)
{
	int count = 0;

	for (int i = 0; i < 3; ++i)
	{
		count = 0;

		for (int j = i; j < 5; ++j)
		{
			if (combo[i].getFace() == combo[j].getFace()) ++count;
		}


		if (count == 3)
		{
			setHandScore(combo[i].getRank());
			return 3;
		}
	}

	return 0;
}

int Player::isTwoPair(Card* combo)
{
	string face1 = "\0", face2 = "\0", face3 = "\0";
	int count1 = 1, count2 = 0, count3 = 0, highCard = 0;

	face1 = combo[0].getFace();
	highCard = combo[0].getRank();

	for (int i = 1; i < 5; ++i)
	{
		//sets face2
		if (face2 == "\0" && combo[i].getFace() != face1)
		{
			face2 = combo[i].getFace();
			if (combo[i].getRank() > highCard) highCard = combo[i].getRank();
		}
		//sets face3
		else if (face3 == "\0" && (combo[i].getFace() != face1 && combo[i].getFace() != face2))
		{
			face3 = combo[i].getFace();
			if (combo[i].getRank() > highCard) highCard = combo[i].getRank();
		}

		//keeps track of face1 occurrences
		if (combo[i].getFace() == face1)
		{
			++count1;
		}
		//keeps track of face2 occurrences
		else if (combo[i].getFace() == face2)
		{
			++count2;
		}
		//keeps track of face3 occurrences
		else if (combo[i].getFace() == face3)
		{
			++count3;
		}
	}


	if ((count1 == 2 && count2 == 2) || (count1 == 2 && count3 == 2) || (count2 == 2 && count3 == 2))
	{
		setHandScore(highCard);
		return 2;
	}
	else return 0;
}

int Player::isPair(Card* combo)
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = i + 1; j < 5; ++j)
		{
			if (combo[i].getFace() == combo[j].getFace())
			{
				setHandScore(combo[i].getRank());
				return 1;
			}
		}
	}

	return 0;
}



//HUMAN PLAYER/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Member Functions
float HumanPlayer::play(float& prizePool, float& currentBet, Card* Board, int uiChoice)
{
	int choice = 0;

	//If player is unable to bet they must fold
	if (currentBet > getBalance())
	{
		setFoldStatus(1);

		return 0;
	}

	// Refactor to use uiChoice variable which gets updated through mouse clicks
	if (uiChoice == 1) //CALL
	{
		setBalance(-currentBet);
		prizePool += currentBet;
		return currentBet;
	}
	else if (uiChoice == 2) //RAISE
	{
		currentBet *= 2; //double the bet
		setBalance(-currentBet);
		prizePool += currentBet;
		return currentBet;
	}
	else if (uiChoice == 3) //FOLD
	{
		setFoldStatus(1);
		return 0;
	}
	return 0;

}

//CPU//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Member functions===============================================================================
float CPU::play(float& prizePool, float& currentBet, Card* Board, int uiChoice) {
	int choice = 0;

	//If player is unable to bet they must fold
	if (currentBet > getBalance())
	{
		setFoldStatus(1);
		return 0;
	}

	srand((unsigned)time(NULL)); /* seed random-number generator */

	choice = rand() % 3 + 1;

	if (choice == 1) //CALL
	{
		setBalance(-currentBet);
		prizePool += currentBet;

		return currentBet;
	}
	else if (choice == 2) //RAISE
	{
		currentBet *= 2; //double the bet
		setBalance(-currentBet);
		prizePool += currentBet;

		return currentBet;
	}
	else if (choice == 3) //FOLD
	{
		setFoldStatus(1);

		return 0;
	}

	return 0;
}

void pressAnyKey()
{
	char key = '\0';
	cout << "Press any key: " << endl;

	while (key == NULL)
	{
		cin >> key;
	}
}