#include "TexasHoldem.hpp"
#include <optional>
#include <algorithm>
#include <cctype>

TexasHoldem::TexasHoldem() : window(sf::VideoMode({ 1920, 1080 }), "Texas Hold'em"), backgroundSprite(titleTexture),
	winnerTextDisplay(mainFont, "", 35), actionLogText(mainFont, "", 24),
	potText(mainFont, "", 20), betText(mainFont, "", 20), balanceText(mainFont, "", 20) {

	window.setFramerateLimit(60);

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

	players[0] = &p1;
	players[1] = &c2;
	players[2] = &c3;
	players[3] = &c4;

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

void TexasHoldem::startNewRound()
{
	playerHandSprites.clear();
	boardSprites.clear();
	for (int i = 0; i < 5; i++) boardCards[i] = Card();
	winnerTextDisplay.setString("");

	// Reset pool and start betting
	prizePool = 20;
	currentBet = 5;
	for (int i = 0; i < 4; i++) {
		players[i]->setBalance(-5);
		players[i]->setFoldStatus(0);
	}

	deck.shuffleDeck();

	// Choose the dealer based on the round number
	if (roundNumber == 1) chooseDealerRandomly();
	else rotateDealer();
	assignTurnOrder();

	deck.deal(player1, player2, player3, Dealer);

	// Fixed minor math error
	prizePool += (currentBet * 1.5f);
	player1->setBalance(-(currentBet / 2.0f));
	player2->setBalance(-currentBet);

	actionLogText.setString("ROUND " + std::to_string(roundNumber) + "! " + player1->getPlayerID() + " POSTS SMALL BLIND, " + player2->getPlayerID() + " POSTS BIG BLIND.");

	// Render player's cards
	for (int i = 0; i < 2; i++) {
		sf::Sprite sprite(getCardTexture(p1.getHand()[i]));
		sprite.setScale({ 2.f, 2.f });
		sprite.setPosition({ 500.f + (i * 150.f), 500.f });
		playerHandSprites.push_back(sprite);
	}

	roundNumber++;
	currentStage = PRE_FLOP;
	currentPhase = BETTING;
	activePlayerIndex = 0;
	playersActed = 0;

	updateUI();
}

void TexasHoldem::advanceTurn()
{
	playersActed++;
	updateUI();

	// Check if everyone folded except one person
	int activeCount = 0;
	for (int i = 0; i < 4; i++) {
		if (!players[i]->getFoldStatus()) activeCount++;
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
}*/

void TexasHoldem::chooseDealerRandomly()
{
	for (int i = 0; i < 4; i++) players[i]->setDealer(0);
	int dealer = rand() % 4;
	players[dealer]->setDealer(1);
}

// Dealer shifts left every round
void TexasHoldem::rotateDealer()
{
	if (p1.checkIfDealer() == 1) { p1.setDealer(0); c2.setDealer(1); }
	else if (c2.checkIfDealer() == 1) { c2.setDealer(0); c3.setDealer(1); }
	else if (c3.checkIfDealer() == 1) { c3.setDealer(0); c4.setDealer(1); }
	else if (c4.checkIfDealer() == 1) { c4.setDealer(0); p1.setDealer(1); }
}

void TexasHoldem::assignTurnOrder()
{
	if (p1.checkIfDealer() == 1)
	{
		Dealer = &p1; player1 = &c2; player2 = &c3; player3 = &c4;
	}
	else if (c2.checkIfDealer() == 1)
	{
		Dealer = &c2; player1 = &c3; player2 = &c4; player3 = &p1;
	}
	else if (c3.checkIfDealer() == 1)
	{
		Dealer = &c3; player1 = &c4; player2 = &p1; player3 = &c2;
	}
	else if (c4.checkIfDealer() == 1)
	{
		Dealer = &c4; player1 = &p1; player2 = &c2; player3 = &c3;
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

// Refactored the original method to work with the new game logic
// Still use the original logic so I kept the above comment block
std::string TexasHoldem::determineWinner()
{
	Player* winner = players[0];
	struct PlayerScore { Player* player; int score; };
	PlayerScore scores[4];
	int nonFoldedCount = 0;

	for (int i = 0; i < 4; ++i) {
		if (!players[i]->getFoldStatus()) {
			scores[nonFoldedCount].player = players[i];
			scores[nonFoldedCount].score = players[i]->score(boardCards);
			++nonFoldedCount;
		}
	}

	if (nonFoldedCount == 1) {
		scores[0].player->setBalance(prizePool);
		return scores[0].player->getPlayerID() + " WINS BY DEFAULT!";
	}

	int maxScore = scores[0].score;
	for (int i = 1; i < nonFoldedCount; ++i) {
		if (scores[i].score > maxScore) maxScore = scores[i].score;
	}

	int maxTieScore = 0;
	for (int i = 0; i < nonFoldedCount; ++i) {
		if (scores[i].score == maxScore) {
			if (scores[i].player->getHandScore() > maxTieScore) {
				maxTieScore = scores[i].player->getHandScore();
				winner = scores[i].player;
			}
		}
	}

	winner->setBalance(prizePool);
	prizePool = 0;
	updateUI();

	// Return the specific string formats written by team member
	return winner->getPlayerID() + " WINS WITH A \n" + getHandName(winner->score(boardCards));
}

void TexasHoldem::processEvents()
{
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>()) window.close();

		if (event->is<sf::Event::KeyPressed>())
		{
			if (currentState == TITLE_SCREEN)
			{
				// Fade to main menu
				currentState = TRANSITIONING;
				fadingToBlack = true;
				fadeAlpha = 0.f;
			}
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