#include "TexasHoldem.hpp"

TexasHoldem::TexasHoldem() : window(sf::VideoMode({ 1920, 1080 }), "Texas Hold'em"), backgroundSprite(titleTexture),
	winnerTextDisplay(mainFont, "", 35), actionLogText(mainFont, "", 24),
	potText(mainFont, "", 20), betText(mainFont, "", 20), balanceText(mainFont, "", 20) {

	//Computer Players
	CPU c2, c3, c4;

	currentState = TITLE_SCREEN;
	currentPhase = DEALING;
	currentStage = WAITING_TO_START;

	// Invisible box to fake a fade transition
	fadeShape.setSize({ static_cast<float>(1280), 720.f });
	fadeShape.setFillColor(sf::Color(0, 0, 0, 0));
	fadeAlpha = 0.f;
	fadingToBlack = true;

	prizePool = 0;
	currentBet = 5;
	roundNumber = 1;

	p1.setPlayerID("PLAYER 1");
	c2.setPlayerID("CPU 1");
	c3.setPlayerID("CPU 2");
	c4.setPlayerID("CPU 3");

	//pointers to players that get assigned based on who is dealer
	Player* Dealer, *player1, *player2, *player3;

	for (int i = 0; i < 4; i++) {
		turnOrder[i] = nullptr;
	}

	loadAssets();
}

TexasHoldem::~TexasHoldem() {
	delete playButton;
	delete rulesButton;
	delete tipButton;
	delete creditsButton;
	delete exitButton;
	delete dealButton;
	delete callButton;
	delete raiseButton;
	delete foldButton;
}

void TexasHoldem::loadAssets()
{
	// Had AI add in the debug/event statements because I was lazy and wanted to see if it would do it with minimal input
	// Prompt: "Add in debug statements to check if textures and assets are loading in correctly"

	if (!titleTexture.loadFromFile("Assets/menus/splashScreen.png")) std::cout << "Failed to load title screen!\n";
	if (!menuTexture.loadFromFile("Assets/menus/tableBG.png")) std::cout << "Failed to load menu screen!\n";
	if (!gameBgTexture.loadFromFile("Assets/menus/tableBG.png")) std::cout << "Failed to load game screen!\n";

	if (!playButtonTexture.loadFromFile("Assets/menus/play_menu.png")) std::cout << "Missing play_menu.png\n";
	if (!rulebuttonTexture.loadFromFile("Assets/menus/rule_menu.png")) std::cout << "Missing rule_menu.png\n";
	if (!tipDevsButtonTexture.loadFromFile("Assets/menus/tipdevs_menu.png")) std::cout << "Missing tipdevs_menu.png\n";
	if (!creditsButtonTexture.loadFromFile("Assets/menus/credits_menu.png")) std::cout << "Missing credits_menu.png\n";
	if (!exitButtonTexture.loadFromFile("Assets/menus/exit_menu.png")) std::cout << "Missing exit_menu.png\n";

	backgroundSprite.setTexture(titleTexture, true);

	if (!mainFont.openFromFile("Assets/CowboyMovie.ttf")) {
		std::cout << "WARNING: Font failed to load! Buttons will not display text.\n";
	}

	// Update font in case it wasn't loaded already
	winnerTextDisplay.setFont(mainFont);
	actionLogText.setFont(mainFont);
	potText.setFont(mainFont);
	betText.setFont(mainFont);
	balanceText.setFont(mainFont);

	// Setup Text Colors and Positions
	winnerTextDisplay.setFillColor(sf::Color::Yellow);
	winnerTextDisplay.setPosition({ 400.f, 350.f });

	actionLogText.setFillColor(sf::Color::White);
	actionLogText.setPosition({ 400.f, 20.f });

	potText.setFillColor(sf::Color::White);
	potText.setPosition({ 20.f, 20.f });
	betText.setFillColor(sf::Color::White);
	betText.setPosition({ 20.f, 50.f });
	balanceText.setFillColor(sf::Color::White);
	balanceText.setPosition({ 20.f, 80.f });

	// Set up main menu image buttons
	playButton = new ImageButton(playButtonTexture, { 640.f, 80.f });
	rulesButton = new ImageButton(rulebuttonTexture, { 640.f, 200.f });
	tipButton = new ImageButton(tipDevsButtonTexture, { 640.f, 350.f });
	creditsButton = new ImageButton(creditsButtonTexture, { 640.f, 500.f });
	exitButton = new ImageButton(exitButtonTexture, { 640.f, 650.f });

	// Set up gameplay buttons
	dealButton = new Button(mainFont, "DEAL NEXT", { 150.f, 50.f }, 75, sf::Color::Transparent, sf::Color::Black);
	dealButton->setPosition({ 1050.f, 600.f });

	callButton = new Button(mainFont, "CALL", { 100.f, 50.f }, 75, sf::Color(50, 150, 50), sf::Color::White);
	callButton->setPosition({ 850.f, 500.f });

	raiseButton = new Button(mainFont, "RAISE", { 100.f, 50.f }, 75, sf::Color(150, 100, 50), sf::Color::White);
	raiseButton->setPosition({ 850.f, 550.f });

	foldButton = new Button(mainFont, "FOLD", { 100.f, 40.f }, 75, sf::Color(150, 50, 50), sf::Color::White);
	foldButton->setPosition({ 850.f, 600.f });
}

std::string TexasHoldem::getFilenameForCard(Card card)
{
	std::string face = card.getFace();
	std::string suit = card.getSuit();

	if (face == "Ace") face = "ace";
	else if (face == "Deuce") face = "2";
	else if (face == "Three") face = "3";
	else if (face == "Four") face = "4";
	else if (face == "Five") face = "5";
	else if (face == "Six") face = "6";
	else if (face == "Seven") face = "7";
	else if (face == "Eight") face = "8";
	else if (face == "Nine") face = "9";
	else if (face == "Ten") face = "10";
	else if (face == "Jack") face = "jack";
	else if (face == "Queen") face = "queen";
	else if (face == "King") face = "king";

	std::transform(suit.begin(), suit.end(), suit.begin(), [](unsigned char c) { return std::tolower(c); });
	return "Assets/cards/" + face + " of " + suit + ".png";
}

sf::Texture& TexasHoldem::getCardTexture(Card card)
{
	std::string filename = getFilenameForCard(card);
	if (cardTextures.find(filename) == cardTextures.end())
	{
		sf::Texture tempTexture;
		if (!tempTexture.loadFromFile(filename)) std::cout << "ERROR: Missing asset: " << filename << std::endl;
		cardTextures[filename] = std::move(tempTexture);
	}
	return cardTextures[filename];
}

void TexasHoldem::updateUI()
{
	potText.setString("POT: $" + std::to_string((int)prizePool));
	betText.setString("CURRENT BET: $" + std::to_string((int)currentBet));
	balanceText.setString("YOUR BALANCE: $" + std::to_string((int)players[0]->getBalance()));
}

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
					playGame(&p1, &c2, &c3, &c4); //runs playGame function for game logic
					break;
				case MULTIPLAYER://-----------------------------------------------------------------------------
				{
					//enter name
					//?
					int multiChoice = 0;
					system("cls");

					std::cout << "1. Host Game" << std::endl;
					std::cout << "2. Join Game" << std::endl;
					std::cin >> multiChoice; //get user's choice if they are hosting or joining

					if (multiChoice == 1) //host side
					{
						if (mpNetwork.startHost()) //start hosting on local device
						{
							std::cout << "Hosting on port 7777. Waiting for players..." << std::endl;

	actionLogText.setString("ROUND " + std::to_string(roundNumber) + "! " + player1->getPlayerID() + " POSTS SMALL BLIND, " + player2->getPlayerID() + " POSTS BIG BLIND.");

	// Render player's cards
	for (int i = 0; i < 2; i++) {
		sf::Sprite sprite(getCardTexture(p1.getHand()[i]));
		sprite.setScale({ 2.f, 2.f });
		sprite.setPosition({ 500.f + (i * 150.f), 500.f });
		playerHandSprites.push_back(sprite);
	}

							//Establish the other players
							NetworkPlayer np1(mpNetwork);
							NetworkPlayer np2(mpNetwork);
							NetworkPlayer np3(mpNetwork);

							chooseDealer(p1, np1, np2, np3); //choose dealer
							pressAnyKey();
							playGame(&p1, &np1, &np2, &np3); //play the game
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
						std::cin >> ipString; //gets host ipaddress

						auto hostIp = sf::IpAddress::fromString(ipString); //gets hostIp from the ipString var
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

	if (activeCount <= 1) {
		currentPhase = SHOWDOWN;
		winnerTextDisplay.setString(determineWinner());
		actionLogText.setString("ROUND ENDED. CLICK DEAL NEXT FOR NEW ROUND.");
		return;
	}

	if (playersActed >= 4) {
		currentPhase = DEALING;
		actionLogText.setString("BETTING COMPLETE. CLICK 'DEAL NEXT' TO CONTINUE.");
	}
	else {
		// Move to the next player
		activePlayerIndex = (activePlayerIndex + 1) % 4;
		cpuThinkTimer.restart();

		// Skip folded players automatically
		while (turnOrder[activePlayerIndex]->getFoldStatus() && playersActed < 4) {
			playersActed++;
			activePlayerIndex = (activePlayerIndex + 1) % 4;
		}

		if (playersActed >= 4) {
			currentPhase = DEALING;
			actionLogText.setString("BETTING COMPLETE. CLICK 'DEAL NEXT' TO CONTINUE.");
		}
	}
}

void TexasHoldem::advanceRoundStage()
{
	if (currentStage == PRE_FLOP)
	{
		for (int i = 0; i < 3; i++) {
			boardCards[i] = deck.drawCard();
			sf::Sprite sprite(getCardTexture(boardCards[i]));
			sprite.setScale({ 1.5f, 1.5f });
			sprite.setPosition({ 300.f + (i * 120.f), 250.f });
			boardSprites.push_back(sprite);
		}
		currentStage = FLOP;
	}
	else if (currentStage == FLOP)
	{
		boardCards[3] = deck.drawCard();
		sf::Sprite sprite(getCardTexture(boardCards[3]));
		sprite.setScale({ 1.5f, 1.5f });
		sprite.setPosition({ 300.f + (3 * 120.f), 250.f });
		boardSprites.push_back(sprite);
		currentStage = TURN;
	}
	else if (currentStage == TURN)
	{
		boardCards[4] = deck.drawCard();
		sf::Sprite sprite(getCardTexture(boardCards[4]));
		sprite.setScale({ 1.5f, 1.5f });
		sprite.setPosition({ 300.f + (4 * 120.f), 250.f });
		boardSprites.push_back(sprite);
		currentStage = RIVER;
	}
	else if (currentStage == RIVER)
	{
		currentPhase = SHOWDOWN;
		winnerTextDisplay.setString(determineWinner());
		actionLogText.setString("SHOWDOWN! CLICK 'DEAL NEXT' TO PLAY AGAIN.");
		return;
	}

	// Reset for the new betting round
	currentPhase = BETTING;
	playersActed = 0;
	activePlayerIndex = 0;
	actionLogText.setString("NEW CARDS DEALT! WAITING FOR BETS...");

	// Ensure we don't start the betting round on a folded player
	while (turnOrder[activePlayerIndex]->getFoldStatus() && playersActed < 4) {
		playersActed++;
		activePlayerIndex = (activePlayerIndex + 1) % 4;
	}

	updateUI();
}

void TexasHoldem::runApp()
{
	while (window.isOpen() && currentState != EXITING)
	{
		processEvents();
		update();
		render();
	}
}

/*void TexasHoldem::displayMenu()
{
	std::system("cls");

	cout << "TEXAS HOLD'EM" << endl << endl;

	cout << "1. PLAY" << endl;
	cout << "2. RULES" << endl;
	cout << "3. TIP DEVS" << endl;
	cout << "4. CREDITS" << endl;
	cout << "5. EXIT" << endl;
}

// Dealer shifts left every round
void TexasHoldem::rotateDealer()
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

	turnOrder[0] = player1;
	turnOrder[1] = player2;
	turnOrder[2] = player3;
	turnOrder[3] = Dealer; 
}

std::string TexasHoldem::getHandName(int score)
{
	if (score == 9) return "ROYAL FLUSH!";
	if (score == 8) return "STRAIGHT FLUSH!";
	if (score == 7) return "FOUR OF A KIND!";
	if (score == 6) return "FULL HOUSE!";
	if (score == 5) return "FLUSH!";
	if (score == 4) return "STRAIGHT!";
	if (score == 3) return "THREE OF A KIND!";
	if (score == 2) return "TWO PAIR!";
	if (score == 1) return "PAIR!";
	return "HIGH CARD...";
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
		return scores[0].player->getPlayerID() + " WINS BY DEFAULT!";
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
	prizePool = 0;
	updateUI();

	// Return the specific string formats written by team member
	return winner->getPlayerID() + " WINS WITH A \n" + getHandName(winner->score(boardCards));
}

//playGame function used for main game loop. Used for both singleplayer and multiplayer
void TexasHoldem::playGame(Player* p1, Player* p2, Player* p3, Player* p4)
{
	int playAgain = 0, round = 1, foldCount = 0;
	char YorN = '\0';
	float prizePool = 0, currentBet = 5;
	Deck deck;
	Card Board[5];
	Player* Dealer, * player1, * player2, * player3;

	do //play again loop
	{
		round = 1; //set round to 1

		do //round loop
		{
			if (round == 1)//STAGE ONE: SET UP--------------------------------------------------------------------
			{
				//buy in ($5)
				prizePool += 20;
				p1->setBalance(-5);
				p2->setBalance(-5);
				p3->setBalance(-5);
				p4->setBalance(-5);

				//deck is shuffled
				deck.shuffleDeck();

				cout << "The deck is shuffled. \n\n";

				//dealer and player pointers are set
				if (p1->checkIfDealer() == 1)
				{
					Dealer = p1;
					player1 = p2;
					player2 = p3;
					player3 = p4;
				}
				else if (p2->checkIfDealer() == 1)
				{
					Dealer = p2;
					player1 = p3;
					player2 = p4;
					player3 = p1;
				}
				else if (p3->checkIfDealer() == 1)
				{
					Dealer = p3;
					player1 = p4;
					player2 = p1;
					player3 = p2;
				}
				else if (p4->checkIfDealer() == 1)
				{
					Dealer = p4;
					player1 = p1;
					player2 = p2;
					player3 = p3;
				}

				//increment round
				++round;
			}
			else if (round == 2)//STAGE 2: BLINDS------------------------------------------------------------------------------------------------------
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
		} while (round < 9 && foldCount != 3); //loop while the showdown has not been reached and while at least two players remain in play

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
		if (p1->checkIfDealer() == 1)
		{
			p1->setDealer(0);
			Dealer = p2;
			player1 = p3;
			player2 = p4;
			player3 = p1;
			p2->setDealer(1);
		}
		else if (p2->checkIfDealer() == 1)
		{
			p2->setDealer(0);
			Dealer = p3;
			player1 = p4;
			player2 = p1;
			player3 = p2;
			p3->setDealer(1);
		}
		else if (p3->checkIfDealer() == 1)
		{
			p3->setDealer(0);
			Dealer = p4;
			player1 = p1;
			player2 = p2;
			player3 = p3;
			p4->setDealer(1);
		}
		else if (p4->checkIfDealer() == 1)
		{
			p4->setDealer(0);
			Dealer = p1;
			player1 = p2;
			player2 = p3;
			player3 = p4;
			p1->setDealer(1);
		}

		if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButton->button == sf::Mouse::Button::Left)
			{
				if (currentState == TITLE_SCREEN)
				{
					currentState = TRANSITIONING;
					fadingToBlack = true;
					fadeAlpha = 0.f;
				}
				else if (currentState == MAIN_MENU)
				{
					if (playButton->isMouseOver(window))
					{
						currentState = PLAYING;
						backgroundSprite.setTexture(gameBgTexture, true);

						// Resize the background texture cause it's smaller
						sf::Vector2u textureSize = gameBgTexture.getSize();
						backgroundSprite.setScale({ 1280.f / textureSize.x, 720.f / textureSize.y });
					}
					else if (rulesButton->isMouseOver(window)) std::cout << "Test -> Rules clicked\n";
					else if (tipButton->isMouseOver(window)) std::cout << "Test -> Tip devs clicked\n";
					else if (creditsButton->isMouseOver(window)) std::cout << "Test -> Credits clicked\n";
					else if (exitButton->isMouseOver(window)) currentState = EXITING;
				}
				else if (currentState == PLAYING)
				{
					// Wait for player input if dealing cards
					if ((currentPhase == DEALING || currentPhase == SHOWDOWN) && dealButton->isMouseOver(window))
					{
						if (currentStage == WAITING_TO_START || currentPhase == SHOWDOWN)
						{
							startNewRound();
						}
						else
						{
							advanceRoundStage();
						}
					}

					// Wait for player choices before continuing
					if (currentPhase == BETTING && turnOrder[activePlayerIndex] == &p1 && !p1.getFoldStatus())
					{
						if (callButton->isMouseOver(window)) {
							p1.play(prizePool, currentBet, boardCards, 1);
							actionLogText.setString("YOU CALLED.");
							advanceTurn();
						}
						else if (raiseButton->isMouseOver(window)) {
							p1.play(prizePool, currentBet, boardCards, 2);
							actionLogText.setString("YOU RAISED!");
							advanceTurn();
						}
						else if (foldButton->isMouseOver(window)) {
							p1.play(prizePool, currentBet, boardCards, 3);
							actionLogText.setString("YOU FOLDED.");
							advanceTurn();
						}
					}
				}
			}
		}
	}
}

void TexasHoldem::update()
{
	if (currentState == TRANSITIONING)
	{
		float fadeSpeed = 8.5f;

		if (fadingToBlack) {
			fadeAlpha += fadeSpeed;
			if (fadeAlpha >= 255.f) {
				fadeAlpha = 255.f;
				fadingToBlack = false;
				backgroundSprite.setTexture(menuTexture, true);

				// Resize the background texture cause it's smaller
				sf::Vector2u textureSize = menuTexture.getSize();
				backgroundSprite.setScale({ 1280.f / textureSize.x, 720.f / textureSize.y });
			}
		}
		else {
			fadeAlpha -= fadeSpeed;
			if (fadeAlpha <= 0.f) {
				fadeAlpha = 0.f;
				currentState = MAIN_MENU;
			}
		}

		// Apply the new alpha to the shape
		fadeShape.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(fadeAlpha)));
	}
	else if (currentState == MAIN_MENU)
	{
		playButton->setHoverEffect(playButton->isMouseOver(window));
		rulesButton->setHoverEffect(rulesButton->isMouseOver(window));
		tipButton->setHoverEffect(tipButton->isMouseOver(window));
		creditsButton->setHoverEffect(creditsButton->isMouseOver(window));
		exitButton->setHoverEffect(exitButton->isMouseOver(window));
	}
	else if (currentState == PLAYING)
	{
		if (dealButton->isMouseOver(window)) dealButton->setTextColor(sf::Color::White);
		else dealButton->setTextColor(sf::Color::Black);

		if (callButton->isMouseOver(window)) callButton->setTextColor(sf::Color::White);
		else callButton->setTextColor(sf::Color::Black);

		if (raiseButton->isMouseOver(window)) raiseButton->setTextColor(sf::Color::White);
		else raiseButton->setTextColor(sf::Color::Black);

		if (foldButton->isMouseOver(window)) foldButton->setTextColor(sf::Color::White);
		else foldButton->setTextColor(sf::Color::Black);

		// Automate CPU turns
		// If betting and the player isn't 0
		if (currentPhase == BETTING && turnOrder[activePlayerIndex] != &p1)
		{
			Player* cpuPlayer = turnOrder[activePlayerIndex];

			// Delay CPU action so it doesn't end right away if folding
			if (cpuThinkTimer.getElapsedTime().asSeconds() > 3.f)
			{
				if (!cpuPlayer->getFoldStatus())
				{
					float oldBet = currentBet;

					// Call the backend CPU logic
					float result = cpuPlayer->play(prizePool, currentBet, boardCards, 0);

					// Update the UI log with what the CPU decided to do
					if (result == 0 && cpuPlayer->getFoldStatus()) {
						actionLogText.setString(cpuPlayer->getPlayerID() + " FOLDED.");
					}
					else if (currentBet > oldBet) {
						actionLogText.setString(cpuPlayer->getPlayerID() + " RAISED TO $" + std::to_string((int)currentBet));
					}
					else {
						actionLogText.setString(cpuPlayer->getPlayerID() + " CALLED.");
					}
				}
				advanceTurn();
			}
		}
	}
}

void TexasHoldem::render()
{
	window.clear();
	window.draw(backgroundSprite);

	if (currentState == TRANSITIONING)
	{
		window.draw(fadeShape);
	}
	else if (currentState == MAIN_MENU)
	{
		playButton->drawTo(window);
		rulesButton->drawTo(window);
		tipButton->drawTo(window);
		creditsButton->drawTo(window);
		exitButton->drawTo(window);
	}
	else if (currentState == PLAYING)
	{
		// Draw the GUI text elements
		window.draw(potText);
		window.draw(betText);
		window.draw(balanceText);
		window.draw(actionLogText);

		// Draw cards
		for (const auto& sprite : boardSprites) window.draw(sprite);
		for (const auto& sprite : playerHandSprites) window.draw(sprite);

		if (currentPhase == SHOWDOWN)
		{
			window.draw(winnerTextDisplay);
			dealButton->drawTo(window);
		}
		// Deals community cards
		else if (currentPhase == DEALING)
		{
			dealButton->drawTo(window);
		}
		else if (currentPhase == BETTING && turnOrder[activePlayerIndex] == &p1 && !p1.getFoldStatus())
		{
			// Show betting buttons only on a human player's turn
			callButton->drawTo(window);
			raiseButton->drawTo(window);
			foldButton->drawTo(window);
		}
	}

	window.display();
}
