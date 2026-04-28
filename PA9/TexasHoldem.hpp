#pragma once

//Headers
#include <SFML/Graphics.hpp>
#include "Deck.hpp"
#include "Button.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>

//enums for organization
enum OPTIONS : int {PLAY = 1, RULES, TIP_DEVS, CREDITS, EXIT};
enum PLAY_OPTIONS: int {SINGLEPLAYER, MULTIPLAYER, BACK};

enum GameState { TITLE_SCREEN, TRANSITIONING, MAIN_MENU, PLAYING, EXITING };
enum PlayPhase { DEALING, BETTING, SHOWDOWN };
enum RoundStage { WAITING_TO_START, PRE_FLOP, FLOP, TURN, RIVER };

class TexasHoldem
{
public:
	TexasHoldem();

	//runs our Texas Hold'em game
	void runApp();

private:
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

	sf::RenderWindow window;

	GameState currentState;
	PlayPhase currentPhase;
	RoundStage currentStage;

	sf::Clock cpuThinkTimer;

	// Assets
	sf::Texture titleTexture;
	sf::Texture menuTexture;
	sf::Texture gameBgTexture;
	sf::Font mainFont;

	std::map<std::string, sf::Texture> cardTextures;
	sf::Sprite backgroundSprite;

	// UI Elements 
	Button* playButton;
	Button* exitButton;
	Button* dealButton;
	Button* callButton;
	Button* raiseButton;
	Button* foldButton;

	// GUI Text Displays
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
