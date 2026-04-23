#include "TexasHoldem.hpp"

void TexasHoldem::runApp()
{
	int choice = 0, playAgain = 0;
	float prizePool = 0, currentBet = 1;
	Deck deck;
	Card Board[5];
	
	//actual players
	HumanPlayer p1, p2, p3, p4;
	CPU c2, c3, c4;

	//pointers to players that get assigned based on who is dealer
	Player* Dealer, *player1, *player2, *player3;

	do
	{
		do
		{
			displayMenu();
			cin >> choice;
		} while (choice < 1 || choice > 5);

		switch (choice)
		{
		case PLAY: //=====================================================================================
			do {
				do
				{
					system("cls");
					cout << "How would you like to play?" << endl << endl;
					cout << "1. Singleplayer" << endl;
					cout << "2. Multiplayer" << endl;
					cout << "3. Back" << endl;

					cin >> choice;

				} while (choice < 1 || choice > 3);

				switch (choice)
				{
				case SINGLEPLAYER://---------------------------------------------------------------------------

					//One human player, three CPU players

					//STAGE 0: NON CYCLICAL SETUP


					//random dealer selection only applicable for the very first round
					system("cls");
					chooseDealer(p1, c2, c3, c4);
					pressAnyKey();

					do
					{
						//STAGE ONE: SET UP--------------------------------------------------------------------

						//buy in ($5)
						prizePool += 20;
						p1.setBalance(-5);
						c2.setBalance(-5);
						c3.setBalance(-5);
						c4.setBalance(-5);

						/*!!!!!
						* DEV NOTE : 1) Need to make it so that play immediately ends if everyone but one persond folds
						*			 2) Need to make it so that players are eliminated from play if they fold
						  !!!!!*/

						//deck is shuffled
						deck.shuffleDeck();

						cout << "The deck is shuffled. \n\n";

						//dealer and player pointers are set
						if (p1.checkIfDealer() == 1)
						{
							Dealer = &p1;
							player1 = &c2;
							player2 = &c3;
							player3 = &c4;
						}
						else if (c2.checkIfDealer() == 1)
						{
							Dealer = &c2;
							player1 = &c3;
							player2 = &c4;
							player3 = &p1;
						}
						else if (c3.checkIfDealer() == 1)
						{
							Dealer = &c3;
							player1 = &c4;
							player2 = &p1;
							player3 = &c2;
						}
						else if (c4.checkIfDealer() == 1)
						{
							Dealer = &c4;
							player1 = &p1;
							player2 = &c2;
							player3 = &c3;
						}

						//STAGE 2: BLINDS------------------------------------------------------------------------------------------------------
						cout << "Player 1 must post the small blind and Player 2 must post the big blind." << endl << endl;
						cout << "Player 1 bets $" << currentBet / 2 << ". Player 2 bets $" << currentBet << "." << endl;

						player1->setBalance(currentBet/2);
						player2->setBalance(currentBet);

						pressAnyKey();
						system("cls");
						//STAGE 3: DEAL------------------------------------------------------------------------------------------------------

						//cards get dealt
						deck.deal(player1, player2, player3, Dealer);

						cout << "Cards have been dealt. Players may now take their first action." << endl;
						pressAnyKey();
						system("cls");

						//STAGE 4: PREFLOP------------------------------------------------------------------------------------------------------

						//DEV NOTE: player number and play itself goes clockwise, does play start with player1 since they are
						//left of the dealer or does it start with player 3 since they are "under the gun" ?

						player1->play(currentBet, Board);
						player2->play(currentBet, Board);
						player3->play(currentBet, Board);
						Dealer->play(currentBet, Board);

						//STAGE 5: THE FLOP-----------------------------------------------------------------------------------------------------
						system("cls");
						
						//draw the flop
						for (int i = 0; i < 3; ++i)
						{
							Board[i] = deck.drawCard();
						}

						
						cout << "The flop has been drawn." << endl << endl
							<< "The Flop: " << endl
							<< "            " << Board[0]
							<< "            " << Board[1]
							<< "            " << Board[2];

						pressAnyKey();

						player1->play(currentBet, Board);
						player2->play(currentBet, Board);
						player3->play(currentBet, Board);
						Dealer->play(currentBet, Board);

						//STAGE 6: THE TURN-----------------------------------------------------------------------------------------------------
						system("cls");

						//draw the turn
						

						//print the current board

						//play

						//STAGE 7: THE RIVER----------------------------------------------------------------------------------------------------
						system("cls");

						//draw the river

						//print the current board

						//play

						//STAGE 8: THE SHOWDOWN------------------------------------------------------------------------------------------------
						system("cls");

						//score whoever hasn't folded
						//compare hands and determine winneer
						//award cash/tokens


						//ask to play again
						//dealer switces to the left
						//set all folds to zero
						//reset bet


						/*!!!!!
						* DEV NOTE : This stage should also be the default whenever there is only one player who has yet to fold
						  !!!!!*/


					} while (playAgain == 1);


					break;

				case MULTIPLAYER://-----------------------------------------------------------------------------
					break;

				case BACK:

					break;
				}
			} while (choice != 3);


			break;
		case RULES: //==================================================================================

			//NOT DONE

			break;
		case TIP_DEVS: //=======================================================================================

			cout << "Venmo @davidjkmiller" << endl;
			pressAnyKey();
			system("cls");

			break;
		case CREDITS: //====================================================================================

			//NOT DONE

			break;
		case EXIT: //======================================================================================

			system("cls");
			cout << "Goodbye!" << endl << endl;

			break;

		}

	} while (choice != 5);


	return;

}

void TexasHoldem::displayMenu()
{
	std::system("cls");

	cout << "TEXAS HOLD'EM" << endl << endl;

	cout << "1. PLAY" << endl;
	cout << "2. RULES" << endl;
	cout << "3. TIP DEVS" << endl;
	cout << "4. CREDITS" << endl;
	cout << "5. EXIT" << endl;
}

void TexasHoldem::chooseDealer(Player& p1, Player& p2, Player& p3, Player& p4)
{
	srand((unsigned)time(NULL)); /* seed random-number generator */

	int dealer = 0;

	dealer = rand() % 4;
	dealer++;

	if (dealer == 1)
	{
		p1.setDealer(1);
		cout << "Player 1 is the dealer!" << endl << endl;
	}
	else if (dealer == 2)
	{
		p2.setDealer(1);
		cout << "Player 2 is the dealer!" << endl << endl;
	}
	else if (dealer == 3)
	{
		p3.setDealer(1);
		cout << "Player 3 is the dealer!" << endl << endl;
	}
	else if (dealer == 4)
	{
		p4.setDealer(1);
		cout << "Player 4 is the dealer!" << endl << endl;
	}

	return;
}


