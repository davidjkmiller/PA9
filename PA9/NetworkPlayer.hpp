/*
Author: James Owen Tweedt
Created: 4/27/26
Desc: Class for managing players on the network. 
Play is used to send packets to the host or receive packets from the 
clients based on what actions the player takes.
It inherits from HumanPlayer so that it can use the same hand and balance management as a normal player, 
but overrides play to send and receive packets instead of taking user input.
*/


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