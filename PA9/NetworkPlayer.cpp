#include "NetworkPlayer.hpp"

//constructor - checks to see if there is a host and sets mIsHost to what getmpIsHost returns
NetworkPlayer::NetworkPlayer(NetworkManager& network) : HumanPlayer(), mpNetwork(network)
{
	mIsHost = network.getmpIsHost(); 
}

//sends multiplayer packets based on what actions they do
float NetworkPlayer::play(float& prizePool, float& currentBet, Card* Board, int uiChoice) 
{
	if (mIsHost) //host receives client's action (host side)
	{
		std::cout << "Waiting for player's action..." << std::endl;

		std::string action;
		action = mpNetwork.receivePacket(); //get the action the player selects

		if (action == "call")
		{
			setBalance(-currentBet); //take bet away from balance
			prizePool += currentBet; //prize pool increase
			return currentBet;
		}
		else if (action == "raise")
		{
			currentBet *= 2;
			setBalance(-currentBet);
			prizePool += currentBet;
			return currentBet;
		}
		else //fold
		{
			setFoldStatus(1);
			return 0;
		}
	}
	else //client side
	{
		int choice = 0;
		do //gets user's choice regarding what they want to do
		{
			std::cout << "1. Call\n2. Raise\n3. Fold\n";
			std::cin >> choice;
		} while (choice < 1 || choice > 3);

		if (choice == 1)
		{
			mpNetwork.sendPacket("call");
			setBalance(-currentBet);
			prizePool += currentBet;
			return currentBet;
		}
		else if (choice == 2)
		{
			mpNetwork.sendPacket("raise");
			currentBet *= 2;
			setBalance(-currentBet);
			prizePool += currentBet;
			return currentBet;
		}
		else
		{
			mpNetwork.sendPacket("fold");
			setFoldStatus(1);
			return 0;
		}
	}
}
