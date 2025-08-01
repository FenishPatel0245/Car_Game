#include "car_game.h"

std::string getUsername()
{
    std::string username;

    // Display game title
    std::cout << "=== F1 RACING GAME ===" << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << std::endl;

    // Prompt for username input
    std::cout << "Enter your username (no spaces): ";
    std::getline(std::cin, username);

    // Remove any non-alphanumeric characters
    username.erase(
        std::remove_if(
            username.begin(),
            username.end(),
            [](char c)
            {
                return !std::isalnum(static_cast<unsigned char>(c));
            }),
        username.end()
    );

    // Fallback name if input is empty
    if (username.empty())
    {
        username = "Player";
    }

    return username;
}
