#include "TexasHoldem.hpp"

void TexasHoldem::runApp()
{
	int choice = 0, playAgain = 0, round = 1, foldCount = 0;
	char YorN = '\0';
	int* scoreArr[4];
	float prizePool = 0, currentBet = 5;
	Deck deck;
	Card Board[5];
	
	//actual players
	HumanPlayer p1, p2, p3, p4;

	//Computer Players
	CPU c2, c3, c4;

	//set playerID's for computers
	c2.setPlayerID("CPU 1");
	c3.setPlayerID("CPU 2");
	c4.setPlayerID("CPU 3");

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

					do //play again loop
					{
						round = 1; //set round to 1

						do //round loop
						{
							if (round == 1)//STAGE ONE: SET UP--------------------------------------------------------------------
							{
								//buy in ($5)
								prizePool += 20;
								p1.setBalance(-5);
								c2.setBalance(-5);
								c3.setBalance(-5);
								c4.setBalance(-5);

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

								//increment round
								++round;
							}
							else if(round == 2)//STAGE 2: BLINDS------------------------------------------------------------------------------------------------------
							{
								cout << player1->getPlayerID() << " must post the small blind and " << player2->getPlayerID() << " must post the big blind." << endl << endl;
								cout << player1->getPlayerID() << " bets $" << currentBet / 2 << ". Player 2 bets $" << currentBet << "." << endl;

								prizePool += currentBet * 1.5;
								player1->setBalance(currentBet / 2);
								player2->setBalance(currentBet);

								pressAnyKey();
								system("cls");

								//increment round
								++round;
							}
							else if (round == 3)//STAGE 3: DEAL------------------------------------------------------------------------------------------------------
							{
								//cards get dealt
								deck.deal(player1, player2, player3, Dealer);

								cout << "Cards have been dealt. Players may now take their first action." << endl;
								pressAnyKey();
								system("cls");

								//increment round
								++round;
							}
							else if (round == 4)//STAGE 4: PREFLOP------------------------------------------------------------------------------------------------------
							{
								//DEV NOTE: player number and play itself goes clockwise, does play start with player1 since they are
								//left of the dealer or does it start with player 3 since they are "under the gun" ?

								//play
								player1->play(prizePool, currentBet, Board);
								player2->play(prizePool, currentBet, Board);
								player3->play(prizePool, currentBet, Board);
								Dealer->play(prizePool, currentBet, Board);

								//increment round
								++round;
							}
							else if (round == 5 && foldCount != 3)//STAGE 5: THE FLOP-----------------------------------------------------------------------------------------------------
							{
								system("cls");

								//draw the flop
								for (int i = 0; i < 3; ++i)
								{
									Board[i] = deck.drawCard();
								}


								cout << "The flop has been drawn." << endl << endl
									<< "The board: " << endl
									<< "            " << Board[0] << endl
									<< "            " << Board[1] << endl
									<< "            " << Board[2] << endl;

								pressAnyKey();

								//only allow players who haven't folded to play
								if (!player1->getFoldStatus()) player1->play(prizePool, currentBet, Board);
								if (!player2->getFoldStatus()) player2->play(prizePool, currentBet, Board);
								if (!player3->getFoldStatus()) player3->play(prizePool, currentBet, Board);
								if (!Dealer->getFoldStatus()) Dealer->play(prizePool, currentBet, Board);
								
								//increment round
								++round;
							}
							else if (round == 6 && foldCount != 3)//STAGE 6: THE TURN-----------------------------------------------------------------------------------------------------
							{
								system("cls");

								//draw the turn
								Board[3] = deck.drawCard();

								//print the current board
								cout << "The turn has been drawn." << endl << endl
									<< "The board: " << endl
									<< "            " << Board[0] << endl
									<< "            " << Board[1] << endl
									<< "            " << Board[2] << endl
									<< "            " << Board[3] << endl;

								//only allow players who haven't folded to play
								if (!player1->getFoldStatus()) player1->play(prizePool, currentBet, Board);
								if (!player2->getFoldStatus()) player2->play(prizePool, currentBet, Board);
								if (!player3->getFoldStatus()) player3->play(prizePool, currentBet, Board);
								if (!Dealer->getFoldStatus()) Dealer->play(prizePool, currentBet, Board);

								//increment round
								++round;
							}
							else if (round == 7 && foldCount != 3)//STAGE 7: THE RIVER----------------------------------------------------------------------------------------------------
							{
								system("cls");

								//draw the river
								Board[4] = deck.drawCard();

								//print the current board
								cout << "The river has been drawn." << endl << endl
									<< "The board: " << endl
									<< "            " << Board[0] << endl
									<< "            " << Board[1] << endl
									<< "            " << Board[2] << endl
									<< "            " << Board[3] << endl
									<< "            " << Board[4] << endl;

								//only allow players who haven't folded to play
								if (!player1->getFoldStatus()) player1->play(prizePool, currentBet, Board);
								if (!player2->getFoldStatus()) player2->play(prizePool, currentBet, Board);
								if (!player3->getFoldStatus()) player3->play(prizePool, currentBet, Board);
								if (!Dealer->getFoldStatus()) Dealer->play(prizePool, currentBet, Board);

								//increment round
								++round;
							}
							else if (round == 8 && foldCount != 3)//STAGE 8: THE SHOWDOWN------------------------------------------------------------------------------------------------
							{
								system("cls");

								// Create array of active players for winner determination
								Player* activePlayers[4] = { player1, player2, player3, Dealer };

								// Determine winner and award prize pool
								determineWinner(activePlayers, Board, 4, prizePool);

								prizePool = 0;

								pressAnyKey();

								++round; //increment round
							}
						}while (round < 9 && foldCount != 3); //loop while the showdown has not been reached and while at least two players remain in play

						system("cls");

						cout << "Would you like to play again? y/n \n";
						
						do
						{
							cin >> YorN;

						} while (YorN != 'y' && YorN != 'n');

						if (YorN == 'y')
						{
							playAgain = 1;
						}
						else playAgain = 0;


						//dealer switches to the left
						if (p1.checkIfDealer() == 1)
						{
							p1.setDealer(0);
							Dealer = &c2;
							player1 = &c3;
							player2 = &c4;
							player3 = &p1;
							c2.setDealer(1);
						}
						else if (c2.checkIfDealer() == 1)
						{
							c2.setDealer(0);
							Dealer = &c3;
							player1 = &c4;
							player2 = &p1;
							player3 = &c2;
							c3.setDealer(1);
						}
						else if (c3.checkIfDealer() == 1)
						{
							c3.setDealer(0);
							Dealer = &c4;
							player1 = &p1;
							player2 = &c2;
							player3 = &c3;
							c4.setDealer(1);
						}
						else if (c4.checkIfDealer() == 1)
						{
							c4.setDealer(0);
							Dealer = &p1;
							player1 = &c2;
							player2 = &c3;
							player3 = &c4;
							p1.setDealer(1);
						}
								
						//set all folds to zero
						Dealer->setFoldStatus(0);
						player1->setFoldStatus(0);
						player2->setFoldStatus(0);
						player3->setFoldStatus(0);

						//reset bet and all other used variables including handScore
						currentBet = 1;

					} while (playAgain == 1);


					break;

				case MULTIPLAYER://-----------------------------------------------------------------------------
				{
					//enter name
					//??

					int multiChoice = 0;
					system("cls");

					std::cout << "1. Host Game" << std::endl;
					std::cout << "2. Join Game" << std::endl;
					std::cin >> multiChoice; //get user's choice if they are hosting or joining

					if (multiChoice == 1)
					{
						if (mpNetwork.startHost()) //start hosting on local device
						{
							std::cout << "Hosting on port 7777. Waiting for players..." << std::endl;

							int numPlayers = 0;
							do //asks how many players are playing in the lobby
							{
								std::cout << "How many players? (2-4): " << std::endl;
								std::cin >> numPlayers;
							} while (numPlayers < 2 || numPlayers > 4);

							while (mpNetwork.getmpPlayerCount() < numPlayers) //waits for all players to connect
							{
								mpNetwork.update(); //keep checking for players
								cout << "Waiting for players..." << mpNetwork.getmpPlayerCount() << "/" << numPlayers << "\r";
							}
							std::cout << "Players connected! Starting Game..." << std::endl;

							NetworkPlayer np1(mpNetwork);
							NetworkPlayer np2(mpNetwork);
							NetworkPlayer np3(mpNetwork);

							chooseDealer(p1, np1, np2, np3);
							pressAnyKey();
						}
						else
						{
							std::cout << "Failed to host game." << std::endl;
						}
					}
					else if (multiChoice == 2) //if user is joining a game
					{
						std::string ipString;
						std::cout << "Enter host's IP address: ";
						std::cin >> ipString;

						auto hostIp = sf::IpAddress::fromString(ipString);
						if (mpNetwork.joinGame(hostIp.value())) //join game to the user with the ip address they entered
						{
							std::cout << "Connected to host." << std::endl;
						}
						else
						{
							std::cout << "Failed to connect to host." << std::endl;
						}
					}
					pressAnyKey();
					break;
				}

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

//AI CREATED FUNCTION USING THE PROMPT:
/*"In TexasHoldem,cpp, I am trying to develop an
algorithm which compares the scores of each player and returns a 
message saying who won. What is an efficient eay of accomplishing this?"*/
//However I've edited the code to serve my purposes
void TexasHoldem::determineWinner(Player* players[], Card* Board, int numPlayers, float prizePool)
{
	Player* winner;

	// Struct to store player data and score
	struct PlayerScore
	{
		Player* player;
		int score;
	};

	// Array to hold all non-folded players and their scores
	PlayerScore scores[4];
	int nonFoldedCount = 0;

	// Collect scores from all non-folded players
	for (int i = 0; i < numPlayers; ++i)
	{
		if (!players[i]->getFoldStatus())
		{
			scores[nonFoldedCount].player = players[i];
			scores[nonFoldedCount].score = players[i]->score(Board);
			++nonFoldedCount;
		}
	}

	// If only one player hasn't folded, they win by default
	if (nonFoldedCount == 1)
	{
		cout << "\nAll other players folded!" << endl;
		cout << scores[0].player->getPlayerID() << " wins the pot of $" << prizePool << "!" << endl;
		scores[0].player->setBalance(prizePool);
		return;
	}

	// Find the maximum score
	int maxScore = scores[0].score;
	for (int i = 1; i < nonFoldedCount; ++i)
	{
		if (scores[i].score > maxScore)
		{
			maxScore = scores[i].score;
		}
	}

	// Count how many players have the max score (for tie handling) and set winner
	int winnerCount = 0;

	//records who has the greatest handScore in case there is a tie (to ensure the absolute winner)
	int maxTieScore = 0;

	for (int i = 0; i < nonFoldedCount; ++i)
	{
		if (scores[i].score == maxScore)
		{
			++winnerCount;
			if (scores[i].player->getHandScore() > maxTieScore)
			{
				maxTieScore = scores[i].player->getHandScore();
				winner = scores[i].player;
			}
		}
	}

	// Output results and award prize
	cout << "\n===== SHOWDOWN =====" << endl;
	for (int i = 0; i < nonFoldedCount; ++i)
	{
		cout << scores[i].player->getPlayerID() << " had a ";
		if (scores[i].score == 9) cout << "Royal Flush!" << endl;
		else if (scores[i].score == 8) cout << "Straight Flush!" << endl;
		else if (scores[i].score == 7) cout << "Four of a Kind!" << endl;
		else if (scores[i].score == 6) cout << "Full House!" << endl;
		else if (scores[i].score == 5) cout << "Flush!" << endl;
		else if (scores[i].score == 4) cout << "Stright!" << endl;
		else if (scores[i].score == 3) cout << "Three of a Kind!" << endl;
		else if (scores[i].score == 2) cout << "Two Pair!" << endl;
		else if (scores[i].score == 1) cout << "Pair!" << endl;
		else if (scores[i].score == 0) cout << "... nothing... yikes friend." << endl;

		//should print sprites instead
		scores[i].player->viewHand();
		cout << endl;

	}
	cout << "===================" << endl;

	
	cout << "\n\n" << winner->getPlayerID() << " wins!" << endl;
	cout << "Prize pool: $" << prizePool << endl;
	winner->setBalance(prizePool);
	return;
		
}


