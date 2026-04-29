#pragma once
#include "Player.hpp"
#include "NetworkManager.hpp"

class NetworkPlayer : public HumanPlayer
{
public:
	//constrcutor
	NetworkPlayer(NetworkManager& network); 
	
	//override the play function for sending packets
	float play(float& prizePool, float& currentBet, Card* Board, int uiChoice = 0) override;

private:
	NetworkManager& mpNetwork;
	bool mIsHost;
};