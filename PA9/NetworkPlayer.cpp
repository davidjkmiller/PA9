#include "NetworkPlayer.hpp"

NetworkPlayer::NetworkPlayer(NetworkManager& network) : HumanPlayer(), mpNetwork(network)
{
	mIsHost = network.getmpIsHost();
}

float NetworkPlayer::play(float& prizePool, float& currentBet, Card* Board, int uiChoice) 
{
	if (mIsHost) //host receives client's action (host side)
	{
		std::cout << "Waiting for player's action..." << std::endl;

		std::string action;
		action = mpNetwork.receivePacket(); //get the action the player selects

		if (action == "call")
		{
			setBalance(-currentBet);
			prizePool += currentBet;
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
		do
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
