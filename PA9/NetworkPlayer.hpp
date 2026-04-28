#pragma once
#include "Player.hpp"
#include "NetworkManager.hpp"

class NetworkPlayer : public HumanPlayer
{
public:
	NetworkPlayer(NetworkManager& network);
	
	//override the play function for sending packets
	float play(float& prizePool, float& currentBet, Card* Board) override;

private:
	NetworkManager& mpNetwork;
	bool mIsHost;
};