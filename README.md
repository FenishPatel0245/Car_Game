Car_Game



-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Fenish/Date:2025-07-27 : I added main files to work on it.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Darshita/Date:2025-07-28 :

Defined the TwoLaneCarGame class interface in the car_game.h header file.

This includes:
- Public constructor and run() method for launching the game loop.
- Private utility methods for rendering, input handling, updating game state, drawing game objects, 
  collision detection, score saving, and leaderboard display.
- Encapsulated screen rendering logic and gameplay management within clearly separated methods.

This interface sets the foundation for the game's modular design and improves maintainability 
by separating declarations from implementation.

Prepares the project for modular compilation and team-based development.

File: car_game.h

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Pratham/Date:2025-07-28 :
Refactor: Centralized constants and added essential headers

- Moved all game dimension and lane-related constants into one place:
  SCREEN_WIDTH, SCREEN_HEIGHT, PANEL_WIDTH, LANE_WIDTH, etc.
- Defined NOMINMAX to prevent macro conflicts with min/max in <windows.h>
- Included required headers: <conio.h>, <fstream>, <sstream>, <iomanip>, <cstdlib>, etc.
- This setup ensures consistent and reusable configuration across the game files

File: car_game.h
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Arman/Date:2025-07-28
chore: define core game state and Car struct

- Added HANDLE and COORD for Windows console rendering
- Defined screen buffer array for full-screen ASCII output
- Created Car struct to represent enemy and player cars
- Initialized game state variables: score, timers, difficulty, etc.
- Set up RNG and timing logic for spawning and game progression

File: car_game.h
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Fenish/Date:2025-07-29

- Added Utility function into header file
  
File: car_game.h
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Fenish/Date:2025-07-29
add constructors and initialize console, screen buffer, and timing

  - Implement Car ctor (lane, y, active=true)
  - Implement TwoLaneCarGame ctor:
  - Seed RNG and set defaults (score=0, difficulty=1, baseSpawnDelay=2000, roadLineOffset=0)
  - Initialize screen buffer and null-terminate each row
  - Configure Windows console: fullscreen mode, buffer size, window size
  - Hide cursor for cleaner render
  - Precompute road divider lines (every 4 rows)
  - Set timing anchors (lastUpdate, lastEnemySpawn, gameStartTime)
  - Position player car at left lane near bottom"

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Darshita/Date:2025-07-29

implement screen clear and background rendering (panels + lane divider)

- clearScreen(): fill screen buffer with spaces and null-terminate each row for safe C-string output
- drawBackground(): render left/right UI panels with dotted pattern and vertical borders
- drawBackground(): render road area with a central lane divider between panels"

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Pratham/Date:2025-07-30

render dashed lane divider and panel UI text

- drawRoadLines(): animate center lane using roadLines + roadLineOffset
  - Wrap over 2× screen height for longer cycle
  - Skip every 3rd row to create dashed effect
- drawUI(): populate left/right panels without touching borders
  - Left: Game, Score, Time (since start), Level, controls, restart/exit
  - Right: control hints and difficulty tip (“every 25s!”)
  - Respect LEFT_PANEL_WIDTH/RIGHT_PANEL_WIDTH bounds and ROAD_RIGHT offset"

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Arman/Date:2025-07-30

add centered game over overlay with final score and restart/exit instructions

- drawGameOver(): renders an 8-line overlay at the screen center
- Displays \"GAME OVER\", \"Cars Collided!\", and the player's final score
- Includes instructions: press R to restart or ESC to exit
- Horizontally centers each line and keeps text within the ROAD_LEFT/ROAD_RIGHT bounds"
 
    File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Arman/Date:2025-07-30

add getLaneX() helper to calculate car's X position

- Returns horizontal center position for a given lane index
- Lane 0: LEFT_LANE_CENTER - 1
- Lane 1: RIGHT_LANE_CENTER - 1
- Used by player and enemy car drawing functions for consistent placement

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  

Name:Darshita/Date:2025-07-30 :

render ASCII sprites for player and enemy cars

- drawPlayerCar(): draws a 5-row detailed ASCII car
  - Rows include: \"0=[_]=0\", \"/|\\\", \"|(o)|\", \"[]=V=[]\", and a centered \"v\"
  - Uses getLaneX() and bounds checks against ROAD_LEFT/ROAD_RIGHT
  - Skips drawing when gameOver is true
- drawEnemyCars(): renders a compact 4x4 star-block car
  - Pattern:
    ****
     ** 
    ****
     ** 
  - Draws only when active and within visible screen rows"
 
File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Pratham/Date:2025-07-30 :

add full render pipeline with screen buffer flush

- Clears buffer, draws background and UI each frame
- When running: draws road lines, player car, and enemy cars
- When game over: draws centered game-over overlay
- Resets cursor to (0,0) with SetConsoleCursorPosition
- Prints each row of the char buffer and flushes output to update the console

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Name:Fenish/Date:2025-07-30 :

add update loop (timing, spawn, collisions) and input handling

- updateGame():
  - Scale difficulty every 25s and compute dynamic tick speed
  - Scroll road lines and move/despawn enemies; award points
  - Dynamic enemy spawn using baseSpawnDelay, score, and difficulty
  - Collision detection and passive score increment
- handleInput():
  - Support A/D and ←/→ for lane switching
  - ESC saves score, shows leaderboard, and exits
  - 'r' (on game over) saves score, shows leaderboard, and restarts
  - Normalizes uppercase keys

    File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Pratham/Date:2025-07-31 :

add enemy car spawn logic with random and challenge-based lane selection

- Randomly selects lane (0 or 1) for new enemy car
- 30% chance to match player's current lane for increased difficulty
- Spawns enemy car just above screen at y = -2 for smooth entry
- Uses uniform_int_distribution with Mersenne Twister RNG

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Arman/Date:2025-07-31 :

add collision detection between player and enemy cars

- Iterates active enemy cars and checks lane equality
- Triggers gameOver when vertical distance to player is < 3
- Early-exits loop after first detected collision

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Darshita/Date:2025-07-31 :

add leaderboard view for current user

- Clears screen and prints username-scoped leaderboard header
- Reads scores from <username>_scores.txt
- Sorts in descending order and displays top 5 with alignment
- Handles empty history with a friendly message
- Waits for key press before returning

File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Fenish/Date:2025-07-30 :

 add score persistence for current user

- saveScore(): appends the current score to <username>_scores.txt
- Opens file in append mode and writes one score per line
- Closes file after successful write

  File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Darshita/Date:2025-08-01 :

add main game loop and restart functionality

- restart(): resets game state after game over
  - Clears enemies, resets score, difficulty, and player position
  - Resets timers for consistent timing on replay
- run(): main game loop with input, update, and render cycle
  - Runs continuously with 30ms delay per frame
 
File: car_game.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Fenish/Date:2025-07-31 :

add username input prompt and cleanup

- Displays game title and asks user for a name
- Removes all non-alphanumeric characters from input
- Defaults to \"Player\" if input is empty
- Ensures username is safe for use in file naming (e.g., score file)

File: main.cpp

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
