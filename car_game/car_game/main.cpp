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

int main()
{
    try
    {
        std::string username = getUsername();

        // Welcome screen
        std::cout << std::endl;
        std::cout << "Welcome, " << username << "!" << std::endl;
        std::cout << std::endl;

        // Game controls and instructions
        std::cout << "Game Controls:" << std::endl;
        std::cout << "A or Left Arrow  - Switch to Left Lane" << std::endl;
        std::cout << "D or Right Arrow - Switch to Right Lane" << std::endl;
        std::cout << "ESC - Exit Game" << std::endl;
        std::cout << "R - Restart (when game over)" << std::endl;
        std::cout << std::endl;
        std::cout << "Objective: Avoid enemy cars by switching lanes!" << std::endl;
        std::cout << "Difficulty increases every 25 seconds." << std::endl;
        std::cout << std::endl;
        std::cout << "The game will now switch to fullscreen mode." << std::endl;
        std::cout << "Press any key to start..." << std::endl;

        // Wait for key press
        if (_kbhit())
        {
            _getch(); // Clear any buffered input
        }
        _getch();

        // Start the game
        TwoLaneCarGame game(username);
        game.run();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press any key to exit..." << std::endl;

        if (_kbhit())
        {
            _getch(); // Clear any buffered input
        }
        _getch();

        return -1;
    }
}