/*
Author: Owen Tweedt
Created: 4/18/26
Last Modified: 4/18/26
Desc: Class for managing network aspects. Uses classes from sf::Packet, sf::TcpSocket and sf::TcpListener.
Currently includes a temp main for testing
*/

#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class NetworkManager
{
public:
	NetworkManager();	
	~NetworkManager();
	
	bool getmpIsHost();
	int getmpPlayerCount();

	bool startHost(); //opens the port for connection & starts listening for tcp packets
	bool joinGame(sf::IpAddress ipAddress); //connects to host
	bool sendPacket(std::string data); //sends the game data
	std::string receivePacket(); //receives the game data
	void update(); //updates the game each loop in order to check for changes regarding packets and the network


private:
	bool mpIsHost;
	sf::TcpListener mpListener;
	sf::TcpSocket mpClientSockets[4];
	sf::TcpSocket mpHostSocket;
	sf::SocketSelector mpSelector;
	int mpPlayerCount;
};