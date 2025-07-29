#include "car_game.h"

// ---------------------------
// Car constructor
// ---------------------------
TwoLaneCarGame::Car::Car(int pLane, int py)
    : lane(pLane),
    y(py),
    active(true)   // Car is active by default
{
}

// ---------------------------
// TwoLaneCarGame constructor
// ---------------------------
TwoLaneCarGame::TwoLaneCarGame(const std::string& user)
    : playerCar(0, SCREEN_HEIGHT - 8),           // Player car starts in left lane, near bottom
    score(0),
    gameOver(false),
    rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())),  // Random seed
    roadLineOffset(0),
    username(user),
    difficultyLevel(1),
    baseSpawnDelay(2000)                       // Enemy spawn delay in milliseconds
{
    // --------------------------------------------------
    // Initialize the screen buffer with empty spaces
    // --------------------------------------------------
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            screen[y][x] = ' ';   // Fill screen with blank spaces
        }
        screen[y][SCREEN_WIDTH] = '\0';  // Null-terminate each line
    }

    // --------------------------------------------------
    // Setup the console for fullscreen rendering
    // --------------------------------------------------
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get console handle
    dwCursorPosition.X = 0;
    dwCursorPosition.Y = 0;

    // Switch console to fullscreen mode
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0);

    // Configure buffer size (so screen fits the window)
    COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set window size to match buffer
    SMALL_RECT windowSize = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    // --------------------------------------------------
    // Hide the console cursor for a cleaner game screen
    // --------------------------------------------------
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;                 // Hide cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // --------------------------------------------------
    // Initialize the road divider lines (every 4 rows)
    // --------------------------------------------------
    for (int i = 0; i < 20; i++)
    {
        roadLines.push_back(i * 4);   // Each divider appears 4 rows apart
    }

    // --------------------------------------------------
    // Set up timing anchors for game loop
    // --------------------------------------------------
    lastUpdate = std::chrono::steady_clock::now();     // Last frame update time
    lastEnemySpawn = std::chrono::steady_clock::now(); // Last enemy spawn time
    gameStartTime = std::chrono::steady_clock::now();  // Game start time (used for difficulty)
}
