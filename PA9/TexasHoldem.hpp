#pragma once

//Headers
#include "Deck.hpp"
#include "NetworkManager.hpp"
#include "NetworkPlayer.hpp"

//enums for organization
enum OPTIONS : int {PLAY = 1, RULES, TIP_DEVS, CREDITS, EXIT};
enum PLAY_OPTIONS: int {SINGLEPLAYER = 1, MULTIPLAYER = 2, BACK = 3};

class TexasHoldem
{
public:

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

};
