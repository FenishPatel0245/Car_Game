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

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Name:Arman/Date:2025-07-28
chore: define core game state and Car struct

- Added HANDLE and COORD for Windows console rendering
- Defined screen buffer array for full-screen ASCII output
- Created Car struct to represent enemy and player cars
- Initialized game state variables: score, timers, difficulty, etc.
- Set up RNG and timing logic for spawning and game progression



