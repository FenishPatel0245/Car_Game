#include "car_game.h"
#include <iomanip>  // for std::setw

std::string getUsername()
{
    std::string username;

    system("cls");  // Clear screen

    std::cout << "*===========================================================================*" << std::endl;
    std::cout << "|                               F1 RACING                                   |" << std::endl;
    std::cout << "*===========================================================================*" << std::endl;
    std::cout << "|                                                                           |" << std::endl;
    std::cout << "|   Please enter your username to start the race!                           |" << std::endl;
    std::cout << "|                                                                           |" << std::endl;
    std::cout << "|   ______________________________                                          |" << std::endl;
    std::cout << "|  | Username (no spaces):       |                                          |" << std::endl;
    std::cout << "|  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                                          |" << std::endl;
    std::cout << "|  | ";
    std::getline(std::cin, username);
    std::cout << "|  |_____________________________|                                          |" << std::endl;
    std::cout << "|                                                                           |" << std::endl;
    std::cout << "*===========================================================================*" << std::endl;

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
        std::string username = getUsername(); // Get the username from the user

        // === DO NOT clear screen ===
        // Show the rest of the welcome screen in continuation

        std::cout << "\n*===========================================================================*" << std::endl;
        std::cout << "/                                                                           \\" << std::endl;
        std::cout << "/                    *~~~~~~~~~~~~~~~~~~~~~~~*                              \\" << std::endl;
        std::cout << "/                        Welcome, " << std::setw(37) << std::left << username << "     \\" << std::endl;
        std::cout << "/                    *~~~~~~~~~~~~~~~~~~~~~~~*                              \\" << std::endl;
        std::cout << "/                                                                           \\" << std::endl;
        std::cout << "/  Objective: Avoid enemy cars by switching lanes.                          \\" << std::endl;
        std::cout << "/                                                                           \\" << std::endl;
        std::cout << "/==========================  Game Controls  ================================\\" << std::endl;
        std::cout << "/   A or < Left Arrow   - Move to Left Lane                                 \\" << std::endl;
        std::cout << "/   D or > Right Arrow  - Move to Right Lane                                \\" << std::endl;
        std::cout << "/   ESC                 - Exit Game                                         \\" << std::endl;
        std::cout << "/   R                   - Restart Game                                      \\" << std::endl;
        std::cout << "/                                                                           \\" << std::endl;
        std::cout << "/  Press any key to start...                                                \\" << std::endl;
        std::cout << "*===========================================================================*" << std::endl;

        _getch(); // Wait for key press

        TwoLaneCarGame* game = new TwoLaneCarGame(username);
        game->run();
        delete game;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
