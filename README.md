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

