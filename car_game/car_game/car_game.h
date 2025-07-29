
#ifndef CAR_GAME_H
#define CAR_GAME_H

#include <iostream>
#define NOMINMAX 
#include <windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>

class TwoLaneCarGame {

private:
    static const int SCREEN_WIDTH = 120;
    static const int SCREEN_HEIGHT = 30;
    static const int LEFT_PANEL_WIDTH = 20;
    static const int RIGHT_PANEL_WIDTH = 20;
    static const int ROAD_LEFT = LEFT_PANEL_WIDTH;
    static const int ROAD_RIGHT = SCREEN_WIDTH - RIGHT_PANEL_WIDTH;
    static const int LANE_WIDTH = (ROAD_RIGHT - ROAD_LEFT) / 4;
    static const int LEFT_LANE_CENTER = ROAD_LEFT + LANE_WIDTH / 2;
    static const int RIGHT_LANE_CENTER = ROAD_LEFT + LANE_WIDTH + LANE_WIDTH / 2;

    HANDLE hConsole;
    COORD dwCursorPosition;
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1];

    struct Car {
        int lane; // 0 = left lane, 1 = right lane
        int y;
        bool active;
        Car(int pLane, int py) : lane(pLane), y(py), active(true) {}
    };

    Car playerCar;
    std::vector<Car> enemyCars;
    std::vector<int> roadLines;

    int score;
    bool gameOver;
    std::mt19937 rng;
    std::chrono::steady_clock::time_point lastUpdate;
    std::chrono::steady_clock::time_point lastEnemySpawn;
    std::chrono::steady_clock::time_point gameStartTime;
    int roadLineOffset;
    std::string username;
    int difficultyLevel;
    int baseSpawnDelay;

    void clearScreen();
    void drawBackground();
    void drawUI();
    void drawRoadLines();
    int getLaneX(int lane);
    void drawPlayerCar();
    void drawEnemyCars();
    void drawGameOver();
    void render();
    void handleInput();
    void updateGame();
    void spawnEnemyCar();
    void checkCollisions();
    void saveScore();
    void showLeaderboard();
    void restart();

public:
	TwoLaneCarGame(const std::string& user); // Constructor to initialize the game with a username
    void run();
    
};


#endif // CAR_GAME_H
