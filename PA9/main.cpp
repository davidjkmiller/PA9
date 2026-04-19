#include "NetworkManager.hpp"

int main()
{
    NetworkManager nm;
    std::string choice = "";

    std::cout << "1. Host Game\n2. Join Game\n" << std::endl;
    std::cin >> choice;

    if (choice == "1")
    {
        if (nm.startHost() == true)
        {
            std::cout << "Hosting on port 7777. Waiting for players..." << std::endl;
            while (true)
            {
                nm.update();
            }
        }
        else
        {
            std::cout << "Failed to host a game." << std::endl;
        }
    }
    else
    {
        std::string ipString = "";
        std::cout << "Please enter the host ip address: ";
        std::cin >> ipString;

        auto hostIp = sf::IpAddress::fromString(ipString);
        if (nm.joinGame(hostIp.value()))
        {
            std::cout << "Connected to host." << std::endl;
            while (true)
            {
                nm.update();
            }
        }
        else
        {
            std::cout << "Failed to connect to host." << std::endl;
        }
    }
    return 0;
}