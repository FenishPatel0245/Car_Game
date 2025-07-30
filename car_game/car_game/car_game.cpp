#include "car_game.h"

// ---------------------------
// Car constructor
// ---------------------------
TwoLaneCarGame::Car::Car(int pLane, int py)
    : lane(pLane), y(py), active(true)
{
}

// ---------------------------
// TwoLaneCarGame constructor
// ---------------------------
TwoLaneCarGame::TwoLaneCarGame(const std::string& user)
    : playerCar(0, SCREEN_HEIGHT - 8),
    score(0),
    gameOver(false),
    rng(static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count())),
    roadLineOffset(0),
    username(user),
    difficultyLevel(1),
    baseSpawnDelay(2000)
{
    // ---------------------------------------------
    // Initialize the screen buffer with spaces
    // and null-terminate each row for safe printing
    // ---------------------------------------------
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            screen[y][x] = ' ';
        }
        screen[y][SCREEN_WIDTH] = '\0';
    }

    // ---------------------------------------------
    // Console setup (Windows API)
    // ---------------------------------------------
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    dwCursorPosition.X = 0;
    dwCursorPosition.Y = 0;

    // Fullscreen display mode (3rd param is an optional out pointer)
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, nullptr);

    // Match buffer to desired game resolution
    COORD bufferSize = {
        static_cast<SHORT>(SCREEN_WIDTH),
        static_cast<SHORT>(SCREEN_HEIGHT)
    };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Match window to buffer
    SMALL_RECT windowSize = {
        0,
        0,
        static_cast<SHORT>(SCREEN_WIDTH - 1),
        static_cast<SHORT>(SCREEN_HEIGHT - 1)
    };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    // Hide the blinking cursor for a cleaner look
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // ---------------------------------------------
    // Initialize road divider positions (every 4 rows)
    // ---------------------------------------------
    for (int i = 0; i < 20; ++i)
    {
        roadLines.push_back(i * 4);
    }

    // ---------------------------------------------
    // Timing anchors for game loop and events
    // ---------------------------------------------
    lastUpdate = std::chrono::steady_clock::now();
    lastEnemySpawn = std::chrono::steady_clock::now();
    gameStartTime = std::chrono::steady_clock::now();
}

void TwoLaneCarGame::clearScreen()
{
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            screen[y][x] = ' ';
        }
        // Null-terminate each row so it can be safely printed as a C-string
        screen[y][SCREEN_WIDTH] = '\0';
    }
}

void TwoLaneCarGame::drawBackground()
{
    // Iterate over every row of the screen
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        // -------------------------------
        // LEFT PANEL (UI Panel on the left)
        // -------------------------------
        for (int x = 0; x < LEFT_PANEL_WIDTH; ++x)
        {
            if (x == LEFT_PANEL_WIDTH - 1)
            {
                // Draw vertical border of the left panel
                screen[y][x] = '|';
            }
            else if ((x + y) % 2 == 0)
            {
                // Add a dotted pattern
                screen[y][x] = '.';
            }
            else
            {
                // Fill with spaces
                screen[y][x] = ' ';
            }
        }

        // -------------------------------
        // ROAD SECTION (between panels)
        // -------------------------------
        for (int x = ROAD_LEFT; x < ROAD_RIGHT; ++x)
        {
            if (x == ROAD_LEFT + LANE_WIDTH)
            {
                // Draw lane divider line
                screen[y][x] = '|';
            }
            else
            {
                // Clear road space
                screen[y][x] = ' ';
            }
        }

        // -------------------------------
        // RIGHT PANEL (UI Panel on the right)
        // -------------------------------
        for (int x = ROAD_RIGHT; x < SCREEN_WIDTH; ++x)
        {
            if (x == ROAD_RIGHT)
            {
                // Draw vertical border of the right panel
                screen[y][x] = '|';
            }
            else if ((x + y) % 2 == 0)
            {
                // Add a dotted pattern
                screen[y][x] = '.';
            }
            else
            {
                // Fill with spaces
                screen[y][x] = ' ';
            }
        }
    }
}