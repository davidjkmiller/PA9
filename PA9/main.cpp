#include "TexasHoldem.hpp"
#include "Deck.hpp"
#include "Tests.hpp"

int main()
{
	// Run unit tests before starting the game
	runAllTests();

	TexasHoldem game;

	game.runApp();

	return 0;
}