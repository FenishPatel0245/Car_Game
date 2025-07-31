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
void TwoLaneCarGame::drawUI()
{
    // Compute elapsed game time in seconds
    auto now = std::chrono::steady_clock::now();
    auto gameTime = std::chrono::duration_cast<std::chrono::seconds>(now - gameStartTime);
    int seconds = static_cast<int>(gameTime.count());

    // -------------------------------
    // LEFT PANEL TEXT (row -> text)
    // -------------------------------
    std::vector<std::pair<int, std::string>> leftTexts = {
        {  1, "Game" },
        {  3, "Score: " + std::to_string(score) },
        {  5, "Time: " + std::to_string(seconds) + "s" },
        {  7, "Level: " + std::to_string(difficultyLevel) },
        {  9, "Control" },
        { 11, "Left" },
        { 12, "Right" },
        { 15, "ESC - Exit" },
        { 16, "R - Restart" }
    };

    // Render left panel lines inside the left panel width
    for (const auto& text : leftTexts)
    {
        int row = text.first;
        if (row < SCREEN_HEIGHT)
        {
            for (int i = 0;
                i < static_cast<int>(text.second.length()) && i < LEFT_PANEL_WIDTH - 2;
                ++i)
            {
                // Offset by 1 to avoid overwriting the panel border at x == 0
                screen[row][i + 1] = text.second[i];
            }
        }
    }

    // -------------------------------
    // RIGHT PANEL TEXT (row -> text)
    // -------------------------------
    std::vector<std::pair<int, std::string>> rightTexts = {
        {  1, "Car" },
        {  3, "Score" },
        {  5, "Control" },
        {  7, "A Key -" },
        {  8, "D Key -" },
        { 10, "Avoid cars" },
        { 11, "by switching" },
        { 12, "lanes!" },
        { 15, "Speed up" },
        { 16, "every 25s!" }
    };

    // Render right panel lines starting just to the right of ROAD_RIGHT border
    for (const auto& text : rightTexts)
    {
        int row = text.first;
        if (row < SCREEN_HEIGHT)
        {
            for (int i = 0;
                i < static_cast<int>(text.second.length()) && i < RIGHT_PANEL_WIDTH - 2;
                ++i)
            {
                // Start at ROAD_RIGHT + 1 to keep the road/panel border intact
                screen[row][ROAD_RIGHT + 1 + i] = text.second[i];
            }
        }
    }
}
void TwoLaneCarGame::drawRoadLines()
{
    // Center lane divider column
    int centerX = ROAD_LEFT + LANE_WIDTH;

    // Animate dashed lane divider using roadLines and roadLineOffset
    for (size_t i = 0; i < roadLines.size(); ++i)
    {
        // Wrap pattern over twice the screen height for a longer cycle
        int lineY = (roadLines[i] + roadLineOffset) % (SCREEN_HEIGHT * 2);

        // Only draw visible rows, skip every 3rd row to make it dashed
        if (lineY >= 0 && lineY < SCREEN_HEIGHT && lineY % 3 != 0)
        {
            screen[lineY][centerX] = ':';
        }
    }
}

void TwoLaneCarGame::drawGameOver()
{
    // Center of the screen
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // Lines to display in the game-over overlay
    std::vector<std::string> text = {
        "GAME OVER",
        "",
        "Cars Collided!",
        "",
        "Final Score: " + std::to_string(score),
        "",
        "Press R to Restart",
        "Press ESC to Exit"
    };

    // Render each line centered horizontally, and vertically around centerY
    for (size_t i = 0; i < text.size(); ++i)
    {
        int startX = centerX - static_cast<int>(text[i].length()) / 2;
        int y = centerY - static_cast<int>(text.size()) / 2 + static_cast<int>(i);

        if (y >= 0 && y < SCREEN_HEIGHT)
        {
            for (size_t j = 0; j < text[i].length(); ++j)
            {
                int x = startX + static_cast<int>(j);

                // Keep the text inside the road area
                if (x >= ROAD_LEFT && x < ROAD_RIGHT)
                {
                    screen[y][x] = text[i][j];
                }
            }
        }
    }
}

int TwoLaneCarGame::getLaneX(int lane)
{
    return (lane == 0)
        ? LEFT_LANE_CENTER - 1
        : RIGHT_LANE_CENTER - 1;
}