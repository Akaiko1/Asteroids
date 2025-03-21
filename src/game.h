#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

// Forward declarations
class InputHandler;
class ConfigManager;
class AssetManager;

// Game states
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
};

// Configuration struct for game entities
struct EntityConfig {
    float size;
    float speed;
    int health;
    Color color;
};

// Game scenario definition
struct Scenario {
    int totalEnemies;
    int currentWave;
    int maxWaves;
    int enemiesPerWave;
    int baseEnemyHealth;
    float enemyHealthMultiplier;
    Color enemyColor;
};

// Input handler class to decouple input from game logic
class InputHandler {
public:
    InputHandler();
    
    bool IsMovingRight() const;
    bool IsMovingLeft() const;
    bool IsMovingUp() const;
    bool IsMovingDown() const;
    bool IsAttacking() const;
    bool IsPausePressed() const;
    void Update();

private:
    bool attackPressed;
    bool pausePressed;
};

// Configuration manager class
class ConfigManager {
public:
    ConfigManager();
    
    EntityConfig GetPlayerConfig() const;
    EntityConfig GetEnemyConfig(int wave) const;
    Scenario GetScenario() const;

private:
    EntityConfig playerConfig;
    std::vector<EntityConfig> enemyConfigs;
    Scenario scenario;
    
    void LoadConfigs();
};

// Asset manager class (placeholder for future texture/sound loading)
class AssetManager {
public:
    AssetManager();
    
    // Placeholder methods for future implementation
    void LoadTextures();
    void LoadSounds();
    
    // Method to get textures when implemented
    // Texture2D GetTexture(const std::string& name) const;
};

class Player {
public:
    Player(const EntityConfig& config, float startX, float startY);
    void Update(const InputHandler& input);
    void Draw() const;
    Rectangle GetRectangle() const;
    bool IsAttacking() const;
    void TakeDamage();
    int GetHealth() const;
    bool IsAlive() const;

private:
    Rectangle player;
    float speed;
    bool attacking;
    int health;
    float attackRadius;
    float invulnerabilityTimer;
    bool isInvulnerable;
    float rotation;      // Rotation angle in degrees
    Vector2 position;    // Center position of the player
    Vector2 velocity;    // Current velocity vector
    float rotationSpeed; // How quickly the ship rotates
    float acceleration;  // Movement acceleration
    float drag;          // Deceleration factor
};

class Enemy {
public:
    Enemy(const EntityConfig& config, float x, float y, float speedX, float speedY);
    void Update(float deltaTime);
    void Draw() const;
    void OnHit(int damage = 1);
    Rectangle GetRectangle() const;
    int GetHealth() const;
    int GetPoints() const;

private:
    Rectangle enemy;
    Color color;
    float speedX;
    float speedY;
    int health;
    int points;
};

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();
    void Run();

private:
    int screenWidth;
    int screenHeight;
    Scenario scenario;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<InputHandler> inputHandler;
    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<AssetManager> assetManager;
    GameState gameState;
    int score;
    float gameTimer;
    
    void Initialize();
    void Update(float deltaTime);
    void Draw();
    void SpawnEnemies(int count);
    void CheckAttackCollisions();
    void CheckPlayerEnemyCollisions();
    Rectangle GetAttackArea(const Rectangle& playerRect);
    void HandleEnemyHit(std::unique_ptr<Enemy>& enemy);
    void RemoveDeadEnemies();
    void StartNewWave();
    void GameOver();
    void Victory();
    void DrawUI();
    void HandleMenuState();
    void HandlePlayingState(float deltaTime);
    void HandlePausedState();
    void HandleGameOverState();
    void HandleVictoryState();
};

#endif // GAME_H