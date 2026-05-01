#pragma once

//Headers
#include <SFML/Graphics.hpp>
#include "Deck.hpp"
#include "Button.hpp"
#include "ImageButton.hpp"
#include "UIElement.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "NetworkManager.hpp"
#include "NetworkPlayer.hpp"
#include "Constants.hpp"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <random>

//enums for organization
enum OPTIONS : int { PLAY = 1, RULES, TIP_DEVS, CREDITS, EXIT };
enum PLAY_OPTIONS : int { SINGLEPLAYER = 1, MULTIPLAYER = 2, BACK = 3 };

enum GameState { TITLE_SCREEN, TRANSITIONING, MAIN_MENU, PLAYING, EXITING };
enum PlayPhase { DEALING, BETTING, SHOWDOWN };
enum RoundStage { WAITING_TO_START, PRE_FLOP, FLOP, TURN, RIVER };

class TexasHoldem
{
public:
	TexasHoldem();
	~TexasHoldem();

	//runs our Texas Hold'em game
	void runApp();

	//displays the menu
	void displayMenu();

	//randomly selects the dealer from the four players
	void chooseDealer(Player& p1, Player& p2, Player& p3, Player& p4);

	//determines the winner and awards the prize pool
	void determineWinner(Player* players[], Card* Board, int numPlayers, float prizePool);

	//game logic used in both singleplayer and multiplayer
	void playGame(Player* p1, Player* p2, Player* p3, Player* p4);

private:
	NetworkManager mpNetwork;

	int roundNumber;
	int activePlayerIndex;
	int playersActed;
	sf::RectangleShape fadeShape;
	float fadeAlpha;
	bool fadingToBlack;

	void processEvents();
	void update();
	void render();
	void loadAssets();
	std::string getFilenameForCard(Card card);
	sf::Texture& getCardTexture(Card card);

	// Logic for in game
	void startNewRound();
	void advanceRoundStage();
	void advanceTurn();
	void chooseDealerRandomly();
	void rotateDealer();
	void assignTurnOrder();
	std::string getHandName(int score);
	std::string determineWinner();
	void updateUI();
	void initMusic();

	sf::RenderWindow window;
	sf::Music backgroundMusic;

	GameState currentState;
	PlayPhase currentPhase;
	RoundStage currentStage;

	sf::Clock cpuThinkTimer;

	// Textures
	sf::Texture titleTexture;
	sf::Texture menuTexture;
	sf::Texture gameBgTexture;
	sf::Font mainFont;
	sf::Texture cardBackTexture;

	sf::Texture playButtonTexture;
	sf::Texture rulebuttonTexture;
	sf::Texture tipDevsButtonTexture;
	sf::Texture creditsButtonTexture;
	sf::Texture exitButtonTexture;
	// sf::Texture multiButtonTexture;
	Button* multiButton;

	std::map<std::string, sf::Texture> cardTextures;
	sf::Sprite backgroundSprite;

	// Vector to hold all UIElements for easy rendering and cleanup
	std::vector<UIElement*> uiElements;

	// Main menu buttons (kept for easy input checking)
	ImageButton* playButton;
	ImageButton* rulesButton;
	ImageButton* tipButton;
	ImageButton* creditsButton;
	ImageButton* exitButton;
	//ImageButton* multiButton;

	Button* dealButton;
	Button* callButton;
	Button* raiseButton;
	Button* foldButton;

	// GUI text displays
	sf::Text winnerTextDisplay;
	sf::Text actionLogText;
	sf::Text potText;
	sf::Text betText;
	sf::Text balanceText;

	// Game state trackers
	Deck deck;
	HumanPlayer p1;
	CPU c2, c3, c4;
	Player* players[4];
	Card boardCards[5];
	std::vector<bool> playerCardVisible;

	Player* Dealer;
	Player* player1;
	Player* player2;
	Player* player3;
	Player* turnOrder[4];

	float prizePool;
	float currentBet;

	std::vector<sf::Sprite> playerHandSprites;
	std::vector<sf::Sprite> boardSprites;
};