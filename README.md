
# Asteroids-Like Game Template

## Overview

This project is a simple game built with C++ using the raylib library. It includes basic functionality such as player movement, enemy spawning, and collision detection.

## Directory Structure

```
.
├── CMakeLists.txt    # Build configuration file for CMake
├── src               # Source code directory
│   ├── game.h        # Header file for game classes
│   ├── game.cpp      # Implementation file for game classes
│   └── main.cpp      # Main entry point for the game
└── __MACOSX          # System folder (can be ignored)
```

## Getting Started

### Prerequisites

- CMake version 3.10 or higher
- A C++ compiler (e.g., g++, clang++)

### Building the Project

1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/yourproject.git
   cd yourproject
   ```

2. Create a build directory and navigate into it:
   ```sh
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```sh
   cmake ..
   ```

4. Build the project:
   ```sh
   make
   ```

### Running the Project

After building, you can run the executable generated in the `build` directory:
   ```sh
   ./main
   ```

## Code Overview

### Player Class

Manages player attributes and behaviors:
- `Player(float x, float y, float size, float speed)`: Constructor to initialize player.
- `void Update()`: Updates the player's state.
- `void Draw() const`: Draws the player on the screen.
- `Rectangle GetRectangle() const`: Returns the player's bounding box.
- `bool IsAttacking() const`: Checks if the player is attacking.

### Enemy Class

Manages enemy attributes and behaviors:
- `Enemy(float x, float y, float size, float speedX, float speedY, int health, Color color)`: Constructor to initialize an enemy.
- `void Update()`: Updates the enemy's state.
- `void Draw() const`: Draws the enemy on the screen.
- `void OnHit()`: Handles logic when an enemy is hit.
- `Rectangle GetRectangle() const`: Returns the enemy's bounding box.
- `int GetHealth() const`: Returns the enemy's health.

### Game Class

Controls the overall game logic:
- `Game()`: Constructor to initialize the game.
- `void Run()`: Runs the main game loop.
- `void SpawnEnemies(int count, int health, Color color)`: Spawns a specified number of enemies.
- `void CheckCollisions()`: Checks for collisions between player attacks and enemies.
- `Rectangle GetAttackArea(const Rectangle& playerRect)`: Gets the player's attack area.
- `void HandleEnemyHit(std::unique_ptr<Enemy>& enemy)`: Handles logic when an enemy is hit.
- `void RemoveDeadEnemies()`: Removes dead enemies from the list.

## Contributing

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Open a pull request.

## License

This project is licensed under the MIT License.
