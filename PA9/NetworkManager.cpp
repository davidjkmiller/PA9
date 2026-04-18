#include "NetworkManager.hpp"

NetworkManager::NetworkManager()
{
	mpIsHost = false;
	mpPlayerCount = 0;

	mpHostSocket.setBlocking(false); //stops blocker for mpHostSocket
	for (int i = 0; i < 4; i++) //stops blocker for all mpClientSockets
	{
		mpClientSockets[i].setBlocking(false);
	}
}

NetworkManager::~NetworkManager()
{
	if (mpIsHost == true) //if the host exists
	{
		mpListener.close(); //close the port we used for our game
	}
}

bool NetworkManager::startHost()
{
	//check to see if the user trying to start host is the host
	if (mpIsHost == true)
	{
		return false; //can't host twice
	}

	mpListener.setBlocking(false);

	if (mpListener.listen(7777) != sf::Socket::Status::Done) //open port 7777, and if it is not open
	{
		return false; //port failed to open
	}

	mpIsHost = true;
	mpPlayerCount = 1;
	return true;
}

bool NetworkManager::joinGame(sf::IpAddress ipAddress)
{
	if (mpIsHost == true) //if we are hosting we cant join
	{
		return false;
	}

	if (mpHostSocket.connect(ipAddress, 7777) == sf::Socket::Status::Done)
	{
		//player connected successfully
		mpPlayerCount++;
		return true;
	}

	return false; //connection failed
}

bool NetworkManager::sendPacket(std::string data)
{
	sf::Packet packet;
	packet << data; //put the data into the packet

	//send data to clients if youre the host, or if you're a client, send your data to the host
	if (mpIsHost == true) //host sends data to clients
	{
		for (int i = 0; i < mpPlayerCount; i++) //all players send packet
		{
			if (mpClientSockets[i].send(packet) != sf::Socket::Status::Done) //if packets sending fail
			{
				return false; //return false
			}
		}
		return true; //return true if all packets were successfully sent
	}
	else //host sends packets to clients
	{
		if (mpHostSocket.send(packet) == sf::Socket::Status::Done)
		{
			return true;
		}
	}
	return false;
}

std::string NetworkManager::receivePacket()
{
	sf::Packet packet;
	std::string data;

	if (mpIsHost == true) //host receives packets from clients
	{
		for (int i = 0; i < mpPlayerCount; i++) //loop through the clients
		{
			if (mpClientSockets[i].receive(packet) == sf::Socket::Status::Done) //if a client sent a packet
			{
				packet >> data;
				return data; //returns the first packet found
			}
		}
	}
	else //clients recive the host's packets
	{
		mpHostSocket.receive(packet);
		packet >> data;
	}
	return data; //returns the packet
}

void NetworkManager::update()
{
	sf::Packet packet;
	if (mpIsHost == true) //update checking the network if there is a server host
	{
		if (mpListener.accept(mpClientSockets[mpPlayerCount]) == sf::Socket::Status::Done) //check if a new client is trying to connect
		{
			//client connected
			mpPlayerCount++;
		}

		for (int i = 0; i < mpPlayerCount; i++) //check if client sends host packets
		{
			sf::Socket::Status status = mpClientSockets[i].receive(packet); //get the status of the clients
			
			if (status == sf::Socket::Status::Done)
			{
				//client sent packets to host
				std::cout << "Client " << i << " sent packets to the host." << std::endl;
			}
			else if (status == sf::Socket::Status::Disconnected) //check if a client disconnects
			{
				//client disconnected
				mpPlayerCount--;
			}
		}
	}
	else
	{
		sf::Socket::Status status = mpHostSocket.receive(packet); //get the status of the host
		
		if (status == sf::Socket::Status::Done) //check if host sent a packet
		{
			std::cout << "Host sent packets to the clients." << std::endl;
		}

		if (status == sf::Socket::Status::Disconnected) //check if host disconnects
		{
			//host disconnected
			mpIsHost = false;
			mpPlayerCount = 0;
		}
	}
}
