#pragma once

//Headers
#include "Deck.hpp"

//enums for organization
enum OPTIONS : int {PLAY = 1, RULES, TIP_DEVS, CREDITS, EXIT};
enum PLAY_OPTIONS: int {SINGLEPLAYER, MULTIPLAYER, BACK};

class TexasHoldem
{
public:

	//runs our Texas Hold'em game
	void runApp();

	//displays the menu
	void displayMenu();

	//randomly selects the dealer from the four players
	void chooseDealer(Player& p1, Player& p2, Player& p3, Player& p4);

	
};
