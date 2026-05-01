#include "TexasHoldem.hpp"
#include <optional>
#include <algorithm>
#include <cctype>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void pressAnyKey();

TexasHoldem::TexasHoldem() : window(sf::VideoMode({ Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT }), "Texas Hold'em"), backgroundSprite(titleTexture),
winnerTextDisplay(mainFont, "", 35), actionLogText(mainFont, "", 24),
potText(mainFont, "", 20), betText(mainFont, "", 20), balanceText(mainFont, "", 20) {

	window.setFramerateLimit(60);

	currentState = TITLE_SCREEN;
	currentPhase = DEALING;
	currentStage = WAITING_TO_START;

	// Invisible box to fake a fade transition
	fadeShape.setSize({ Constants::SCREEN_WIDTH_FLOAT, (float)Constants::SCREEN_HEIGHT });
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

	Dealer = nullptr;
	player1 = nullptr;
	player2 = nullptr;
	player3 = nullptr;

	for (int i = 0; i < 4; i++) {
		turnOrder[i] = nullptr;
	}

	loadAssets();
}

TexasHoldem::~TexasHoldem() {
	for (UIElement* element : uiElements)
	{
		delete element;
	}
	uiElements.clear();
}

void TexasHoldem::loadAssets()
{
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

	winnerTextDisplay.setFont(mainFont);
	actionLogText.setFont(mainFont);
	potText.setFont(mainFont);
	betText.setFont(mainFont);
	balanceText.setFont(mainFont);

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

	playButton = new ImageButton(playButtonTexture, { 640.f, 80.f });
	rulesButton = new ImageButton(rulebuttonTexture, { 640.f, 200.f });
	tipButton = new ImageButton(tipDevsButtonTexture, { 640.f, 350.f });
	creditsButton = new ImageButton(creditsButtonTexture, { 640.f, 500.f });
	exitButton = new ImageButton(exitButtonTexture, { 640.f, 650.f });

	dealButton = new Button(mainFont, "DEAL NEXT", { 150.f, 50.f }, 75, sf::Color::Transparent, sf::Color::Black);
	dealButton->setPosition({ 1050.f, 600.f });

	callButton = new Button(mainFont, "CALL", { 100.f, 50.f }, 75, sf::Color(50, 150, 50), sf::Color::White);
	callButton->setPosition({ 850.f, 500.f });

	raiseButton = new Button(mainFont, "RAISE", { 100.f, 50.f }, 75, sf::Color(150, 100, 50), sf::Color::White);
	raiseButton->setPosition({ 850.f, 550.f });

	foldButton = new Button(mainFont, "FOLD", { 100.f, 40.f }, 75, sf::Color(150, 50, 50), sf::Color::White);
	foldButton->setPosition({ 850.f, 600.f });

	uiElements.push_back(playButton);
	uiElements.push_back(rulesButton);
	uiElements.push_back(tipButton);
	uiElements.push_back(creditsButton);
	uiElements.push_back(exitButton);
	uiElements.push_back(dealButton);
	uiElements.push_back(callButton);
	uiElements.push_back(raiseButton);
	uiElements.push_back(foldButton);
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
	balanceText.setString("YOUR BALANCE: $" + std::to_string((int)p1.getBalance()));
}

void TexasHoldem::startNewRound()
{
	deck.initDeck();
	deck.shuffleDeck();
	rotateDealer();
	deck.deal(player1, player2, player3, Dealer);

	playerHandSprites.clear();
	boardSprites.clear();
	actionLogText.setString("ROUND " + std::to_string(roundNumber) + "! " + player1->getPlayerID() + " POSTS SMALL BLIND.");

	for (int i = 0; i < 2; i++) {
		sf::Sprite sprite(getCardTexture(p1.getHand()[i]));
		sprite.setScale({ 1.5f, 1.5f });
		sprite.setPosition({ 500.f + (i * 120.f), 500.f });
		playerHandSprites.push_back(sprite);
	}

	currentStage = PRE_FLOP;
	currentPhase = BETTING;
	playersActed = 0;
	activePlayerIndex = 0;

	updateUI();
}

void TexasHoldem::advanceTurn()
{
	int activeCount = 0;
	for (int i = 0; i < 4; i++) {
		if (turnOrder[i] && !turnOrder[i]->getFoldStatus()) activeCount++;
	}

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
		activePlayerIndex = (activePlayerIndex + 1) % 4;
		cpuThinkTimer.restart();

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

	currentPhase = BETTING;
	playersActed = 0;
	activePlayerIndex = 0;
	actionLogText.setString("NEW CARDS DEALT! WAITING FOR BETS...");

	while (turnOrder[activePlayerIndex]->getFoldStatus() && playersActed < 4) {
		playersActed++;
		activePlayerIndex = (activePlayerIndex + 1) % 4;
	}

	updateUI();
}

void TexasHoldem::processEvents()
{
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			currentState = EXITING;
		}
		else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
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
						sf::Vector2u textureSize = gameBgTexture.getSize();
						backgroundSprite.setScale({ Constants::SCREEN_WIDTH_FLOAT / textureSize.x, Constants::SCREEN_HEIGHT / (float)textureSize.y });
					}
					else if (rulesButton->isMouseOver(window)) std::cout << "Test -> Rules clicked\n";
					else if (tipButton->isMouseOver(window)) std::cout << "Test -> Tip devs clicked\n";
					else if (creditsButton->isMouseOver(window)) std::cout << "Test -> Credits clicked\n";
					else if (exitButton->isMouseOver(window)) currentState = EXITING;
				}
				else if (currentState == PLAYING)
				{
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

void TexasHoldem::runApp()
{
	while (window.isOpen() && currentState != EXITING)
	{
		processEvents();
		update();
		render();
	}
}

void TexasHoldem::displayMenu()
{
	int choice = 0;
	std::system("cls");

	cout << "TEXAS HOLD'EM (CLI MODE)" << endl << endl;
	cout << "1. PLAY SINGLEPLAYER" << endl;
	cout << "2. PLAY MULTIPLAYER" << endl;
	cout << "3. EXIT" << endl;

	do {
		cin >> choice;
	} while (choice < 1 || choice > 3);

	switch (choice)
	{
	case SINGLEPLAYER:
		system("cls");
		chooseDealer(p1, c2, c3, c4);
		playGame(&p1, &c2, &c3, &c4);
		break;
	case MULTIPLAYER:
	{
		int multiChoice = 0;
		system("cls");
		std::cout << "1. Host Game\n2. Join Game\n";
		std::cin >> multiChoice;

		if (multiChoice == 1)
		{
			if (mpNetwork.startHost())
			{
				std::cout << "Hosting on port 7777. Waiting for players..." << std::endl;
				int numPlayers = 0;
				do {
					std::cout << "How many players? (2-4): ";
					std::cin >> numPlayers;
				} while (numPlayers < 2 || numPlayers > 4);

				while (mpNetwork.getmpPlayerCount() < numPlayers) {
					mpNetwork.update();
					cout << "Waiting for players... " << mpNetwork.getmpPlayerCount() << "/" << numPlayers << "\r";
				}
				std::cout << "\nPlayers connected! Starting Game..." << std::endl;

				NetworkPlayer np1(mpNetwork);
				NetworkPlayer np2(mpNetwork);
				NetworkPlayer np3(mpNetwork);

				chooseDealer(p1, np1, np2, np3);
				playGame(&p1, &np1, &np2, &np3);
			}
			else {
				std::cout << "Failed to host game." << std::endl;
			}
		}
		else if (multiChoice == 2)
		{
			std::string ipString;
			std::cout << "Enter host's IP address: ";
			std::cin >> ipString;

			auto hostIp = sf::IpAddress::resolve(ipString);
			if (hostIp && mpNetwork.joinGame(hostIp.value())) {
				std::cout << "Connected to host." << std::endl;
			}
			else {
				std::cout << "Failed to connect to host." << std::endl;
			}
		}
		break;
	}
	case BACK:
		break;
	}
}

void TexasHoldem::rotateDealer()
{
	srand((unsigned)time(NULL));

	int dealerNum = rand() % 4 + 1;

	// Reset dealer status
	p1.setDealer(0);
	c2.setDealer(0);
	c3.setDealer(0);
	c4.setDealer(0);

	if (dealerNum == 1) {
		p1.setDealer(1);
		Dealer = &p1;
		player1 = &c2;
		player2 = &c3;
		player3 = &c4;
	}
	else if (dealerNum == 2) {
		c2.setDealer(1);
		Dealer = &c2;
		player1 = &c3;
		player2 = &c4;
		player3 = &p1;
	}
	else if (dealerNum == 3) {
		c3.setDealer(1);
		Dealer = &c3;
		player1 = &c4;
		player2 = &p1;
		player3 = &c2;
	}
	else if (dealerNum == 4) {
		c4.setDealer(1);
		Dealer = &c4;
		player1 = &p1;
		player2 = &c2;
		player3 = &c3;
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

std::string TexasHoldem::determineWinner()
{
	Player* winner = nullptr;

	struct PlayerScore {
		Player* player;
		int score;
	};

	PlayerScore scores[4];
	int nonFoldedCount = 0;

	for (int i = 0; i < 4; ++i)
	{
		if (turnOrder[i] && !turnOrder[i]->getFoldStatus())
		{
			scores[nonFoldedCount].player = turnOrder[i];
			scores[nonFoldedCount].score = turnOrder[i]->score(boardCards);
			++nonFoldedCount;
		}
	}

	if (nonFoldedCount == 1)
	{
		scores[0].player->setBalance(prizePool);
		prizePool = 0;
		updateUI();
		return scores[0].player->getPlayerID() + " WINS BY DEFAULT!";
	}

	int maxScore = scores[0].score;
	for (int i = 1; i < nonFoldedCount; ++i)
	{
		if (scores[i].score > maxScore)
		{
			maxScore = scores[i].score;
		}
	}

	int maxTieScore = 0;
	for (int i = 0; i < nonFoldedCount; ++i)
	{
		if (scores[i].score == maxScore)
		{
			if (scores[i].player->getHandScore() >= maxTieScore)
			{
				maxTieScore = scores[i].player->getHandScore();
				winner = scores[i].player;
			}
		}
	}

	if (winner)
	{
		winner->setBalance(prizePool);
		prizePool = 0;
		updateUI();
		return winner->getPlayerID() + " WINS WITH A \n" + getHandName(winner->score(boardCards));
	}

	return "TIE OR ERROR!";
}

void TexasHoldem::determineWinner(Player* activePlayers[], Card* Board, int numPlayers, float prizePoolArg)
{
	Player* winner = nullptr;

	struct PlayerScore {
		Player* player;
		int score;
	};

	PlayerScore scores[4];
	int nonFoldedCount = 0;

	for (int i = 0; i < numPlayers; ++i)
	{
		if (!activePlayers[i]->getFoldStatus())
		{
			scores[nonFoldedCount].player = activePlayers[i];
			scores[nonFoldedCount].score = activePlayers[i]->score(Board);
			++nonFoldedCount;
		}
	}

	if (nonFoldedCount == 1)
	{
		cout << "\nAll other players folded!" << endl;
		cout << scores[0].player->getPlayerID() << " wins the pot of $" << prizePoolArg << "!" << endl;
		scores[0].player->setBalance(prizePoolArg);
		return;
	}

	int maxScore = scores[0].score;
	for (int i = 1; i < nonFoldedCount; ++i)
	{
		if (scores[i].score > maxScore)
		{
			maxScore = scores[i].score;
		}
	}

	int maxTieScore = 0;

	for (int i = 0; i < nonFoldedCount; ++i)
	{
		if (scores[i].score == maxScore)
		{
			if (scores[i].player->getHandScore() > maxTieScore)
			{
				maxTieScore = scores[i].player->getHandScore();
				winner = scores[i].player;
			}
		}
	}

	cout << "\n===== SHOWDOWN =====" << endl;
	for (int i = 0; i < nonFoldedCount; ++i)
	{
		cout << scores[i].player->getPlayerID() << " had a ";
		cout << getHandName(scores[i].score) << endl;
		scores[i].player->viewHand();
		cout << endl;
	}
	cout << "===================" << endl;

	cout << "\n\n" << winner->getPlayerID() << " wins!" << endl;
	cout << "Prize pool: $" << prizePoolArg << endl;
	winner->setBalance(prizePoolArg);
}

void TexasHoldem::playGame(Player* p1Ptr, Player* p2Ptr, Player* p3Ptr, Player* p4Ptr)
{
	int playAgain = 0, round = 1, foldCount = 0;
	char YorN = '\0';
	float currentPrizePool = 0, currentBetAmount = 5;
	Deck localDeck;
	Card Board[5];
	Player* localDealer = nullptr, * localPlayer1 = nullptr, * localPlayer2 = nullptr, * localPlayer3 = nullptr;

	do
	{
		round = 1;

		do
		{
			if (round == 1)
			{
				currentPrizePool += 20;
				p1Ptr->setBalance(-5);
				p2Ptr->setBalance(-5);
				p3Ptr->setBalance(-5);
				p4Ptr->setBalance(-5);

				localDeck.shuffleDeck();

				cout << "The deck is shuffled. \n\n";

				if (p1Ptr->checkIfDealer() == 1)
				{
					localDealer = p1Ptr;
					localPlayer1 = p2Ptr;
					localPlayer2 = p3Ptr;
					localPlayer3 = p4Ptr;
				}
				else if (p2Ptr->checkIfDealer() == 1)
				{
					localDealer = p2Ptr;
					localPlayer1 = p3Ptr;
					localPlayer2 = p4Ptr;
					localPlayer3 = p1Ptr;
				}
				else if (p3Ptr->checkIfDealer() == 1)
				{
					localDealer = p3Ptr;
					localPlayer1 = p4Ptr;
					localPlayer2 = p1Ptr;
					localPlayer3 = p2Ptr;
				}
				else if (p4Ptr->checkIfDealer() == 1)
				{
					localDealer = p4Ptr;
					localPlayer1 = p1Ptr;
					localPlayer2 = p2Ptr;
					localPlayer3 = p3Ptr;
				}
				++round;
			}
			else if (round == 2)
			{
				cout << localPlayer1->getPlayerID() << " must post the small blind and " << localPlayer2->getPlayerID() << " must post the big blind." << endl << endl;
				cout << localPlayer1->getPlayerID() << " bets $" << currentBetAmount / 2 << ". Player 2 bets $" << currentBetAmount << "." << endl;

				currentPrizePool += currentBetAmount * 1.5f;
				localPlayer1->setBalance(currentBetAmount / 2.0f);
				localPlayer2->setBalance(currentBetAmount);

				pressAnyKey();
				system("cls");
				++round;
			}
			else if (round == 3)
			{
				localDeck.deal(localPlayer1, localPlayer2, localPlayer3, localDealer);
				cout << "Cards have been dealt. Players may now take their first action." << endl;
				pressAnyKey();
				system("cls");
				++round;
			}
			else if (round == 4)
			{
				localPlayer1->play(currentPrizePool, currentBetAmount, Board);
				localPlayer2->play(currentPrizePool, currentBetAmount, Board);
				localPlayer3->play(currentPrizePool, currentBetAmount, Board);
				localDealer->play(currentPrizePool, currentBetAmount, Board);
				++round;
			}
			else if (round == 5 && foldCount != 3)
			{
				system("cls");
				for (int i = 0; i < 3; ++i)
				{
					Board[i] = localDeck.drawCard();
				}

				cout << "The flop has been drawn." << endl << endl
					<< "The board: " << endl
					<< "            " << Board[0] << endl
					<< "            " << Board[1] << endl
					<< "            " << Board[2] << endl;

				pressAnyKey();

				if (!localPlayer1->getFoldStatus()) localPlayer1->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer2->getFoldStatus()) localPlayer2->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer3->getFoldStatus()) localPlayer3->play(currentPrizePool, currentBetAmount, Board);
				if (!localDealer->getFoldStatus()) localDealer->play(currentPrizePool, currentBetAmount, Board);

				++round;
			}
			else if (round == 6 && foldCount != 3)
			{
				system("cls");
				Board[3] = localDeck.drawCard();

				cout << "The turn has been drawn." << endl << endl
					<< "The board: " << endl
					<< "            " << Board[0] << endl
					<< "            " << Board[1] << endl
					<< "            " << Board[2] << endl
					<< "            " << Board[3] << endl;

				if (!localPlayer1->getFoldStatus()) localPlayer1->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer2->getFoldStatus()) localPlayer2->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer3->getFoldStatus()) localPlayer3->play(currentPrizePool, currentBetAmount, Board);
				if (!localDealer->getFoldStatus()) localDealer->play(currentPrizePool, currentBetAmount, Board);

				++round;
			}
			else if (round == 7 && foldCount != 3)
			{
				system("cls");
				Board[4] = localDeck.drawCard();

				cout << "The river has been drawn." << endl << endl
					<< "The board: " << endl
					<< "            " << Board[0] << endl
					<< "            " << Board[1] << endl
					<< "            " << Board[2] << endl
					<< "            " << Board[3] << endl
					<< "            " << Board[4] << endl;

				if (!localPlayer1->getFoldStatus()) localPlayer1->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer2->getFoldStatus()) localPlayer2->play(currentPrizePool, currentBetAmount, Board);
				if (!localPlayer3->getFoldStatus()) localPlayer3->play(currentPrizePool, currentBetAmount, Board);
				if (!localDealer->getFoldStatus()) localDealer->play(currentPrizePool, currentBetAmount, Board);

				++round;
			}
			else if (round == 8 && foldCount != 3)
			{
				system("cls");

				Player* activePlayers[4] = { localPlayer1, localPlayer2, localPlayer3, localDealer };
				determineWinner(activePlayers, Board, 4, currentPrizePool);
				currentPrizePool = 0;

				pressAnyKey();
				++round;
			}
		} while (round < 9 && foldCount != 3);

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

		if (p1Ptr->checkIfDealer() == 1)
		{
			p1Ptr->setDealer(0);
			p2Ptr->setDealer(1);
		}
		else if (p2Ptr->checkIfDealer() == 1)
		{
			p2Ptr->setDealer(0);
			p3Ptr->setDealer(1);
		}
		else if (p3Ptr->checkIfDealer() == 1)
		{
			p3Ptr->setDealer(0);
			p4Ptr->setDealer(1);
		}
		else if (p4Ptr->checkIfDealer() == 1)
		{
			p4Ptr->setDealer(0);
			p1Ptr->setDealer(1);
		}

	} while (playAgain == 1);
}

void TexasHoldem::chooseDealer(Player& p1Ref, Player& p2Ref, Player& p3Ref, Player& p4Ref)
{
	srand((unsigned)time(NULL));
	int dealer = rand() % 4 + 1;

	if (dealer == 1) {
		p1Ref.setDealer(1);
		cout << "Player 1 is the dealer!" << endl;
	}
	else if (dealer == 2) {
		p2Ref.setDealer(1);
		cout << "Player 2 is the dealer!" << endl;
	}
	else if (dealer == 3) {
		p3Ref.setDealer(1);
		cout << "Player 3 is the dealer!" << endl;
	}
	else if (dealer == 4) {
		p4Ref.setDealer(1);
		cout << "Player 4 is the dealer!" << endl;
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

				sf::Vector2u textureSize = menuTexture.getSize();
				backgroundSprite.setScale({ Constants::SCREEN_WIDTH_FLOAT / textureSize.x, Constants::SCREEN_HEIGHT / (float)textureSize.y });
			}
		}
		else {
			fadeAlpha -= fadeSpeed;
			if (fadeAlpha <= 0.f) {
				fadeAlpha = 0.f;
				currentState = MAIN_MENU;
			}
		}

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
		// Text hover effects for gameplay buttons
		if (dealButton->isMouseOver(window)) dealButton->setTextColor(sf::Color::White);
		else dealButton->setTextColor(sf::Color::Black);

		if (callButton->isMouseOver(window)) callButton->setTextColor(sf::Color::White);
		else callButton->setTextColor(sf::Color::Black);

		if (raiseButton->isMouseOver(window)) raiseButton->setTextColor(sf::Color::White);
		else raiseButton->setTextColor(sf::Color::Black);

		if (foldButton->isMouseOver(window)) foldButton->setTextColor(sf::Color::White);
		else foldButton->setTextColor(sf::Color::Black);

		// CPU turns
		if (currentPhase == BETTING && turnOrder[activePlayerIndex] != &p1)
		{
			Player* cpuPlayer = turnOrder[activePlayerIndex];

			if (cpuThinkTimer.getElapsedTime().asSeconds() > 3.f)
			{
				if (!cpuPlayer->getFoldStatus())
				{
					float oldBet = currentBet;
					float result = cpuPlayer->play(prizePool, currentBet, boardCards, 0);

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
		window.draw(potText);
		window.draw(betText);
		window.draw(balanceText);
		window.draw(actionLogText);

		for (const auto& sprite : boardSprites) window.draw(sprite);
		for (const auto& sprite : playerHandSprites) window.draw(sprite);

		if (currentPhase == SHOWDOWN || currentPhase == DEALING)
		{
			if (currentPhase == SHOWDOWN) window.draw(winnerTextDisplay);
			dealButton->drawTo(window);
		}
		else if (currentPhase == BETTING && turnOrder[activePlayerIndex] == &p1 && !p1.getFoldStatus())
		{
			callButton->drawTo(window);
			raiseButton->drawTo(window);
			foldButton->drawTo(window);
		}
	}

	window.display();
}

void TexasHoldem::chooseDealerRandomly() {}
void TexasHoldem::assignTurnOrder() {}