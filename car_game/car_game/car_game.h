// car_game.h
#ifndef CAR_GAME_H
#define CAR_GAME_H

#include <iostream>
#include <windows.h>
#include <vector>
#include <random>
#include <chrono>
#include <string>

class TwoLaneCarGame {
public:
    TwoLaneCarGame(const std::string& user);
    void run();

private:
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

    
};


#endif // CAR_GAME_H
