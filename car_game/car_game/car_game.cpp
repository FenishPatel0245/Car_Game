#include "car_game.h"

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

void TwoLaneCarGame::drawPlayerCar()
{
    if (!gameOver)
    {
        int x = getLaneX(playerCar.lane);
        int y = playerCar.y;

        // Draw the exact car design:
        //   0=[_]=0
        //     /|\
        //    |(o)|
        //   []=V=[]
        //      v
        if (y >= 0 && y + 5 < SCREEN_HEIGHT)
        {
            // Row 1: "0=[_]=0"  (7 chars: x-3 .. x+3)
            if (x - 3 >= ROAD_LEFT && x + 3 < ROAD_RIGHT)
            {
                screen[y][x - 3] = '0';
                screen[y][x - 2] = '=';
                screen[y][x - 1] = '[';
                screen[y][x + 0] = '_';
                screen[y][x + 1] = ']';
                screen[y][x + 2] = '=';
                screen[y][x + 3] = '0';
            }

            // Row 2: "/|\"
            if (x - 1 >= ROAD_LEFT && x + 1 < ROAD_RIGHT)
            {
                screen[y + 1][x - 1] = '/';
                screen[y + 1][x + 0] = '|';
                screen[y + 1][x + 1] = '\\';
            }

            // Row 3: "|(o)|"
            if (x - 2 >= ROAD_LEFT && x + 2 < ROAD_RIGHT)
            {
                screen[y + 2][x - 2] = '|';
                screen[y + 2][x - 1] = '(';
                screen[y + 2][x + 0] = 'o';
                screen[y + 2][x + 1] = ')';
                screen[y + 2][x + 2] = '|';
            }

            // Row 4: "[]=V=[]"  (7 chars: x-3 .. x+3)
            if (x - 3 >= ROAD_LEFT && x + 3 < ROAD_RIGHT)
            {
                screen[y + 3][x - 3] = '[';
                screen[y + 3][x - 2] = ']';
                screen[y + 3][x - 1] = '=';
                screen[y + 3][x + 0] = 'V';
                screen[y + 3][x + 1] = '=';
                screen[y + 3][x + 2] = '[';
                screen[y + 3][x + 3] = ']';
            }

            // Row 5: "v" - centered
            if (x >= ROAD_LEFT && x < ROAD_RIGHT)
            {
                screen[y + 4][x] = 'v';
            }
        }
    }
}

void TwoLaneCarGame::drawEnemyCars()
{
    for (const auto& car : enemyCars)
    {
        if (car.active && car.y >= -2 && car.y < SCREEN_HEIGHT)
        {
            int x = getLaneX(car.lane);
            int y = car.y;

            if (y >= 0 && y + 3 < SCREEN_HEIGHT)
            {
                // A simple 4x4 star block car:
                // ****
                //  ** 
                // ****
                //  ** 
                screen[y][x] = '*';
                screen[y][x + 1] = '*';
                screen[y][x + 2] = '*';
                screen[y][x + 3] = '*';

                screen[y + 1][x + 1] = '*';
                screen[y + 1][x + 2] = '*';

                screen[y + 2][x] = '*';
                screen[y + 2][x + 1] = '*';
                screen[y + 2][x + 2] = '*';
                screen[y + 2][x + 3] = '*';

                screen[y + 3][x + 1] = '*';
                screen[y + 3][x + 2] = '*';
            }
        }
    }
}
void TwoLaneCarGame::render()
{
    clearScreen();
    drawBackground();
    drawUI();

    if (!gameOver)
    {
        drawRoadLines();
        drawPlayerCar();
        drawEnemyCars();
    }
    else
    {
        drawGameOver();
    }

    // Move the console cursor to the top-left before printing the buffer
    SetConsoleCursorPosition(hConsole, dwCursorPosition);

    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        std::cout << screen[y];
        if (y < SCREEN_HEIGHT - 1)
        {
            std::cout << '\n';
        }
    }

    std::cout.flush();
}
void TwoLaneCarGame::handleInput()
{
    if (_kbhit())
    {
        int key = _getch();

        // Handle extended / special keys (arrows, etc.)
        if (key == 224 || key == 0)
        {
            int specialKey = _getch();

            if (!gameOver)
            {
                if (specialKey == 75) // Left Arrow
                {
                    playerCar.lane = 0;
                }
                else if (specialKey == 77) // Right Arrow
                {
                    playerCar.lane = 1;
                }
            }
        }
        else
        {
            // Normalize uppercase letters to lowercase
            if (key >= 'A' && key <= 'Z')
            {
                key = key + ('a' - 'A');
            }

            if (!gameOver)
            {
                if (key == 'a')
                {
                    playerCar.lane = 0;
                }
                else if (key == 'd')
                {
                    playerCar.lane = 1;
                }
                else if (key == 27) // ESC
                {
                    saveScore();
                    showLeaderboard();
                    exit(0);
                }
            }
            else
            {
                if (key == 'r')
                {
                    saveScore();
                    showLeaderboard();
                    restart();
                }
                else if (key == 27) // ESC
                {
                    saveScore();
                    showLeaderboard();
                    exit(0);
                }
            }
        }
    }
}
void TwoLaneCarGame::updateGame()
{
    auto now = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
    auto gameTime = std::chrono::duration_cast<std::chrono::seconds>(now - gameStartTime);

    // Increase difficulty every 25 seconds
    int newDifficulty = (static_cast<int>(gameTime.count()) / 25) + 1;
    if (newDifficulty > difficultyLevel)
    {
        difficultyLevel = newDifficulty;
    }

    // Game speed decreases as score/difficulty rise, but not below 30 ms per tick
    int gameSpeed = std::max(30, 150 - (score / 50) - (difficultyLevel * 10));
    if (deltaTime.count() < gameSpeed)
    {
        return;
    }

    lastUpdate = now;

    if (gameOver)
    {
        return;
    }

    // Scroll road lines
    roadLineOffset = (roadLineOffset + 1) % (SCREEN_HEIGHT * 2);

    // Move enemy cars downward; despawn when off-screen; award score
    for (auto& car : enemyCars)
    {
        if (car.active)
        {
            car.y += 1;

            if (car.y > SCREEN_HEIGHT + 2)
            {
                car.active = false;
                score += 10;
            }
        }
    }

    // Remove inactive cars
    enemyCars.erase(
        std::remove_if(
            enemyCars.begin(),
            enemyCars.end(),
            [](const Car& c)
            {
                return !c.active;
            }),
        enemyCars.end()
    );

    // Spawn new enemy car based on dynamic spawn delay
    auto spawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastEnemySpawn);
    int  spawnDelay = std::max(200, baseSpawnDelay - (score / 20) - (difficultyLevel * 100));

    if (spawnTime.count() > spawnDelay)
    {
        spawnEnemyCar();
        lastEnemySpawn = now;
    }

    // Collision check and passive score gain
    checkCollisions();
    score += 1;
}
void TwoLaneCarGame::spawnEnemyCar()
{
    // Randomly choose a lane: 0 (left) or 1 (right)
    std::uniform_int_distribution<int> dist(0, 1);
    int lane = dist(rng);

    // 30% chance to mirror the player's current lane (adds challenge)
    std::uniform_int_distribution<int> challengeDist(0, 100);
    if (challengeDist(rng) < 30)
    {
        lane = playerCar.lane;
    }

    // Spawn just above the visible screen (y = -2) so it scrolls in
    enemyCars.emplace_back(lane, -2);
}

void TwoLaneCarGame::checkCollisions()
{
    for (const auto& car : enemyCars)
    {
        // Check if the car is in the same lane and close vertically
        if (car.active && car.lane == playerCar.lane)
        {
            if (abs(playerCar.y - car.y) < 3)
            {
                gameOver = true;  // Collision detected
                break;
            }
        }
    }
}
void TwoLaneCarGame::showLeaderboard()
{
    // Clear console screen
    system("cls");

    // Header
    std::cout << "=== LEADERBOARD FOR " << username << " ===" << std::endl;
    std::cout << "=====================================" << std::endl << std::endl;

    // Open the user's score file
    std::string filename = username + "_scores.txt";
    std::ifstream file(filename);
    std::vector<int> scores;

    if (file.is_open())
    {
        int score;
        while (file >> score)
        {
            scores.push_back(score);
        }
        file.close();
    }

    // Display scores
    if (scores.empty())
    {
        std::cout << "No previous scores found." << std::endl;
    }
    else
    {
        // Sort in descending order and show top 5
        std::sort(scores.begin(), scores.end(), std::greater<int>());

        std::cout << "Top 5 Highest Scores:" << std::endl;
        std::cout << "=====================" << std::endl;

        int displayCount = std::min(5, static_cast<int>(scores.size()));

        for (int i = 0; i < displayCount; ++i)
        {
            std::cout << std::setw(2) << (i + 1) << ". "
                << std::setw(8) << scores[i] << " points" << std::endl;
        }
    }

    // Wait for user input before continuing
    std::cout << std::endl << "Press any key to continue..." << std::endl;
    _getch();
}
void TwoLaneCarGame::saveScore()
{
    // Append the current score to the user's score file
    std::string filename = username + "_scores.txt";
    std::ofstream file(filename, std::ios::app);

    if (file.is_open())
    {
        file << score << std::endl;
        file.close();
    }
}
void TwoLaneCarGame::restart()
{
    // Reset game state
    gameOver = false;
    score = 0;
    enemyCars.clear();
    playerCar.lane = 0;
    roadLineOffset = 0;
    difficultyLevel = 1;

    // Reset timing anchors
    lastUpdate = std::chrono::steady_clock::now();
    lastEnemySpawn = std::chrono::steady_clock::now();
    gameStartTime = std::chrono::steady_clock::now();
}
void TwoLaneCarGame::run()
{
    while (true)
    {
        handleInput();  // Process key input
        updateGame();   // Update game logic
        render();       // Draw frame
        Sleep(30);      // Control loop timing (~30 FPS)
    }
}
