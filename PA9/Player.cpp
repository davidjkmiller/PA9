#include "Player.hpp"

//PLAYER///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Constructor========================================================================
Player::Player()
{
	Card nullCard;

	hand[0] = hand[1] = nullCard;

	balance = 1000.00;

	isDealer = 0;

	hasFolded = 0;
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

//Getters===========================================================================
Card* Player::getHand()
{
	return hand;
}

int Player::checkIfDealer()
{
	return isDealer;
}

float Player::getBalance()
{
	return balance;
}

int Player::getFoldStatus()
{
	return hasFolded;
}

//Member functions
void Player::viewHand()
{
	cout << getHand()[0] << getHand()[1] << endl;
}

//HUMAN PLAYER/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Member Functions
float HumanPlayer::play(float& currentBet, Card* Board)
{
	int choice = 0;

	//If player is unable to bet they must fold
	if (currentBet > getBalance())
	{
		system("cls");
		cout << "I'm afraid you are forced to fold my friend." << endl;
		pressAnyKey();

		return 0;
	}

	do
	{
		system("cls");
		cout << "The Board: " << endl;
		if (Board[0].getFace() != "\0")  cout << "            " << Board[0] << endl;
		if (Board[1].getFace() != "\0")  cout << "            " << Board[1] << endl;
		if (Board[2].getFace() != "\0")  cout << "            " << Board[2] << endl;
		if (Board[3].getFace() != "\0")  cout << "            " << Board[3] << endl;
		if (Board[4].getFace() != "\0")  cout << "            " << Board[4] << endl;

		cout << endl << "What is your move?" << endl << endl;
		cout << "1. Call" << endl;
		cout << "2. Raise" << endl;
		cout << "3. Fold" << endl;

		cout << endl << "Hand:" << endl;
		viewHand();

		cout << endl << "Balance: " << getBalance() << endl;

		cin >> choice;

	} while (choice < 1 || choice > 3);

	if (choice == 1) //CALL
	{
		setBalance(-currentBet);
		cout << "You called." << endl << "New Balance: " << getBalance() << endl;
		pressAnyKey();
		return currentBet;
	}
	else if (choice == 2) //RAISE
	{
		currentBet *= 2; //double the bet
		setBalance(-currentBet);
		cout << "You raised." << endl << "New Balance: " << getBalance() << endl;
		pressAnyKey();
		return currentBet;
	}
	else if (choice == 3) //FOLD
	{
		setFoldStatus(1);
		cout << "You folded." << endl;
		pressAnyKey();

		return 0;
	}

}

//CPU//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Member functions===============================================================================
float CPU::play(float &currentBet, Card* Board)
{
	int choice = 0;

	//If player is unable to bet they must fold
	if (currentBet > getBalance())
	{
		system("cls");
		cout << "They were forced to fold!" << endl;
		pressAnyKey();

		return 0;
	}

	srand((unsigned)time(NULL)); /* seed random-number generator */

	choice = rand() % 3 + 1;

	if (choice == 1) //CALL
	{
		setBalance(-currentBet);
		cout << "They called!" << endl; //*See playerID comment on Player.hpp
		pressAnyKey();

		return currentBet;
	}
	else if (choice == 2) //RAISE
	{
		currentBet *= 2; //double the bet
		setBalance(-currentBet);
		cout << "They raised! The new bet is $" << currentBet << "." << endl; //*
		pressAnyKey();

		return currentBet;
	}
	else if (choice == 3) //FOLD
	{
		setFoldStatus(1);
		cout << "They folded!" << endl;
		pressAnyKey();

		return 0;
	}

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