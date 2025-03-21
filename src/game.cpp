#include "game.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

// Input Handler Implementation
InputHandler::InputHandler() : attackPressed(false), pausePressed(false) {}

bool InputHandler::IsMovingRight() const {
    return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
}

bool InputHandler::IsMovingLeft() const {
    return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
}

bool InputHandler::IsMovingUp() const {
    return IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
}

bool InputHandler::IsMovingDown() const {
    return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
}

bool InputHandler::IsAttacking() const {
    return attackPressed;
}

bool InputHandler::IsPausePressed() const {
    return pausePressed;
}

void InputHandler::Update() {
    attackPressed = IsKeyPressed(KEY_SPACE);
    pausePressed = IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE);
}

// Config Manager Implementation
ConfigManager::ConfigManager() {
    LoadConfigs();
}

void ConfigManager::LoadConfigs() {
    // Player config
    playerConfig.size = 50.0f;
    playerConfig.speed = 5.0f;
    playerConfig.health = 3;
    playerConfig.color = BLUE;
    
    // Enemy configs for different waves
    enemyConfigs.resize(3); // For 3 waves of enemies
    
    // Wave 1 enemies
    enemyConfigs[0].size = 50.0f;
    enemyConfigs[0].speed = 1.0f;
    enemyConfigs[0].health = 1;
    enemyConfigs[0].color = RED;
    
    // Wave 2 enemies
    enemyConfigs[1].size = 45.0f;
    enemyConfigs[1].speed = 2.0f;
    enemyConfigs[1].health = 2;
    enemyConfigs[1].color = MAROON;
    
    // Wave 3 enemies
    enemyConfigs[2].size = 40.0f;
    enemyConfigs[2].speed = 3.0f;
    enemyConfigs[2].health = 3;
    enemyConfigs[2].color = DARKPURPLE;
    
    // Scenario config
    scenario.totalEnemies = 12;
    scenario.currentWave = 0;
    scenario.maxWaves = 3;
    scenario.enemiesPerWave = 4;
    scenario.baseEnemyHealth = 1;
    scenario.enemyHealthMultiplier = 1.5f;
    scenario.enemyColor = RED;
}

EntityConfig ConfigManager::GetPlayerConfig() const {
    return playerConfig;
}

EntityConfig ConfigManager::GetEnemyConfig(int wave) const {
    if (wave < 0 || wave >= static_cast<int>(enemyConfigs.size())) {
        return enemyConfigs[0]; // Return default if out of range
    }
    return enemyConfigs[wave];
}

Scenario ConfigManager::GetScenario() const {
    return scenario;
}

// Asset Manager Implementation
AssetManager::AssetManager() {
    // Initialize asset manager
}

void AssetManager::LoadTextures() {
    // Placeholder for loading textures
}

void AssetManager::LoadSounds() {
    // Placeholder for loading sounds
}

// Player class implementation
Player::Player(const EntityConfig& config, float startX, float startY)
    : player{ startX, startY, config.size, config.size }, 
      speed(config.speed),
      attacking(false),
      health(config.health),
      attackRadius(config.size * 1.5f),
      invulnerabilityTimer(0.0f),
      isInvulnerable(false),
      rotation(0.0f),
      position{ startX + config.size/2, startY + config.size/2 },
      velocity{ 0, 0 },
      rotationSpeed(1.0f),
      acceleration(0.2f),
      drag(0.98f) {}

void Player::Update(const InputHandler& input) {
    // Handle rotation based on direction
    bool isMoving = false;
    
    // Calculate target rotation based on input
    float targetRotation = rotation;
    
    if (input.IsMovingRight() && input.IsMovingUp()) {
        targetRotation = 315.0f; // Up-right
        isMoving = true;
    } else if (input.IsMovingRight() && input.IsMovingDown()) {
        targetRotation = 45.0f; // Down-right
        isMoving = true;
    } else if (input.IsMovingLeft() && input.IsMovingUp()) {
        targetRotation = 225.0f; // Up-left
        isMoving = true;
    } else if (input.IsMovingLeft() && input.IsMovingDown()) {
        targetRotation = 135.0f; // Down-left
        isMoving = true;
    } else if (input.IsMovingRight()) {
        targetRotation = 0.0f; // Right
        isMoving = true;
    } else if (input.IsMovingLeft()) {
        targetRotation = 180.0f; // Left
        isMoving = true;
    } else if (input.IsMovingUp()) {
        targetRotation = 270.0f; // Up
        isMoving = true;
    } else if (input.IsMovingDown()) {
        targetRotation = 90.0f; // Down
        isMoving = true;
    }
    
    // Smoothly rotate towards target direction
    if (isMoving) {
        // Find the shortest path to rotate
        float diff = targetRotation - rotation;
        if (diff > 180.0f) diff -= 360.0f;
        if (diff < -180.0f) diff += 360.0f;
        
        // Apply rotation with the rotation speed
        rotation += diff * 0.1f * rotationSpeed;
        
        // Keep rotation in 0-360 range
        if (rotation < 0) rotation += 360.0f;
        if (rotation >= 360.0f) rotation -= 360.0f;
        
        // Apply acceleration in the direction of rotation
        float radians = rotation * DEG2RAD;
        velocity.x += cos(radians) * acceleration;
        velocity.y += sin(radians) * acceleration;
    }
    
    // Apply drag to slow down
    velocity.x *= drag;
    velocity.y *= drag;
    
    // Update position
    position.x += velocity.x;
    position.y += velocity.y;
    
    // Update rectangle position for collision detection
    player.x = position.x - player.width/2;
    player.y = position.y - player.height/2;
    
    // Keep player within screen bounds
    if (position.x < player.width/2) {
        position.x = player.width/2;
        velocity.x = 0;
    }
    if (position.x > GetScreenWidth() - player.width/2) {
        position.x = GetScreenWidth() - player.width/2;
        velocity.x = 0;
    }
    if (position.y < player.height/2) {
        position.y = player.height/2;
        velocity.y = 0;
    }
    if (position.y > GetScreenHeight() - player.height/2) {
        position.y = GetScreenHeight() - player.height/2;
        velocity.y = 0;
    }
    
    // Set attacking state
    attacking = input.IsAttacking();
    
    // Update invulnerability timer
    if (isInvulnerable) {
        invulnerabilityTimer -= GetFrameTime();
        if (invulnerabilityTimer <= 0) {
            isInvulnerable = false;
        }
    }
}

void Player::Draw() const {
    // Define the triangular ship vertices
    Vector2 v1, v2, v3;
    float shipSize = player.width * 0.8f;
    float radians = rotation * DEG2RAD;
    
    // Calculate vertices for a triangle pointing in the direction of rotation
    v1.x = position.x + cos(radians) * shipSize;
    v1.y = position.y + sin(radians) * shipSize;
    
    v2.x = position.x + cos(radians - 2.5f) * (shipSize * 0.6f);
    v2.y = position.y + sin(radians - 2.5f) * (shipSize * 0.6f);

    v3.x = position.x + cos(radians + 2.5f) * (shipSize * 0.6f);
    v3.y = position.y + sin(radians + 2.5f) * (shipSize * 0.6f);
    
    // Draw the triangle ship
    Color shipColor = isInvulnerable ? 
        ((static_cast<int>(invulnerabilityTimer * 10) % 2 == 0) ? BLUE : SKYBLUE) : 
        BLUE;
    
    DrawTriangle(v1, v2, v3, shipColor);
    
    // Draw health indicators
    for (int i = 0; i < health; i++) {
        DrawRectangle(10 + i * 30, 10, 20, 20, RED);
    }
    
    // Draw attack radius if attacking
    if (attacking) {
        for (int i = 0; i < 3; i++) {
            DrawCircleLines(position.x, position.y, attackRadius + i, RED);
        }
    }
}

Rectangle Player::GetRectangle() const {
    return player;
}

bool Player::IsAttacking() const {
    return attacking;
}

void Player::TakeDamage() {
    if (!isInvulnerable) {
        health--;
        isInvulnerable = true;
        invulnerabilityTimer = 2.0f; // 2 seconds of invulnerability
    }
}

int Player::GetHealth() const {
    return health;
}

bool Player::IsAlive() const {
    return health > 0;
}

// Enemy class implementation
Enemy::Enemy(const EntityConfig& config, float x, float y, float speedX, float speedY)
    : enemy{ x, y, config.size, config.size }, 
      color(config.color),
      speedX(speedX + config.speed), // Add base speed to random speed
      speedY(speedY + config.speed), 
      health(config.health),
      points(health * 100) {}

void Enemy::Update(float deltaTime) {
    // Update position
    enemy.x += speedX * deltaTime * 60.0f; // Scale by deltaTime for consistent movement
    enemy.y += speedY * deltaTime * 60.0f;
    
    // Bounce off screen edges
    if (enemy.x <= 0 || enemy.x + enemy.width >= GetScreenWidth()) {
        speedX = -speedX;
        // Correct position to avoid getting stuck at screen edge
        if (enemy.x <= 0) enemy.x = 0;
        if (enemy.x + enemy.width >= GetScreenWidth()) enemy.x = GetScreenWidth() - enemy.width;
    }
    
    if (enemy.y <= 0 || enemy.y + enemy.height >= GetScreenHeight()) {
        speedY = -speedY;
        // Correct position to avoid getting stuck at screen edge
        if (enemy.y <= 0) enemy.y = 0;
        if (enemy.y + enemy.height >= GetScreenHeight()) enemy.y = GetScreenHeight() - enemy.height;
    }
}

void Enemy::Draw() const {

    DrawCircle(
        enemy.x + enemy.width/2, 
        enemy.y + enemy.height/2, 
        enemy.width/2, 
        color
    );
    
    // Draw health bar above enemy
    Rectangle healthBar = { enemy.x, enemy.y - 10, enemy.width, 5 };
    DrawRectangleRec(healthBar, GRAY);
    Rectangle currentHealth = { 
        enemy.x, 
        enemy.y - 10, 
        (enemy.width * health) / (points / 100), // Scale based on initial health (which is points/100)
        5 
    };
    DrawRectangleRec(currentHealth, GREEN);
}

void Enemy::OnHit(int damage) {
    health -= damage;
}

Rectangle Enemy::GetRectangle() const {
    return enemy;
}

int Enemy::GetHealth() const {
    return health;
}

int Enemy::GetPoints() const {
    return points;
}

// Game class implementation
Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), 
      screenHeight(screenHeight),
      gameState(GameState::MENU),
      score(0),
      gameTimer(0.0f) {
    Initialize();
}

Game::~Game() {
    // Clean up resources if needed
}

void Game::Initialize() {
    // Initialize handlers and managers
    inputHandler = std::make_unique<InputHandler>();
    configManager = std::make_unique<ConfigManager>();
    assetManager = std::make_unique<AssetManager>();
    
    // Get scenario configuration
    scenario = configManager->GetScenario();
    
    // Initialize player at center of screen
    const EntityConfig& playerConfig = configManager->GetPlayerConfig();
    player = std::make_unique<Player>(
        playerConfig,
        screenWidth / 2.0f - playerConfig.size / 2,
        screenHeight / 2.0f - playerConfig.size / 2
    );
    score = 0; // Reset score
    
    // Clear enemies vector to start fresh
    enemies.clear();
}

void Game::Run() {
    InitWindow(screenWidth, screenHeight, "Asteroids!");
    SetTargetFPS(60);
    
    // Game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Update input handler
        inputHandler->Update();
        
        // Update game based on current state
        Update(deltaTime);
        
        // Draw everything
        Draw();
    }
    
    CloseWindow();
}

void Game::Update(float deltaTime) {
    switch (gameState) {
        case GameState::MENU:
            HandleMenuState();
            break;
            
        case GameState::PLAYING:
            HandlePlayingState(deltaTime);
            break;
            
        case GameState::PAUSED:
            HandlePausedState();
            break;
            
        case GameState::GAME_OVER:
            HandleGameOverState();
            break;
            
        case GameState::VICTORY:
            HandleVictoryState();
            break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    switch (gameState) {
        case GameState::MENU:
            // Menu UI
            DrawText("ASTEROIDS!", screenWidth / 2 - MeasureText("ASTEROIDS!", 40) / 2, screenHeight / 4, 40, BLACK);
            DrawText("Press ENTER to start", screenWidth / 2 - MeasureText("Press ENTER to start", 20) / 2, screenHeight / 2, 20, DARKGRAY);
            DrawText("Move with WASD or Arrow Keys", screenWidth / 2 - MeasureText("Move with WASD or Arrow Keys", 20) / 2, screenHeight / 2 + 40, 20, DARKGRAY);
            DrawText("Attack with SPACE", screenWidth / 2 - MeasureText("Attack with SPACE", 20) / 2, screenHeight / 2 + 70, 20, DARKGRAY);
            DrawText("Pause with P or ESC", screenWidth / 2 - MeasureText("Pause with P or ESC", 20) / 2, screenHeight / 2 + 100, 20, DARKGRAY);
            break;
            
        case GameState::PLAYING:
            // Draw game entities
            player->Draw();
            
            for (const auto& enemy : enemies) {
                enemy->Draw();
            }
            
            // Draw game UI
            DrawUI();
            break;
            
        case GameState::PAUSED:
            // Draw game entities (as background)
            player->Draw();
            
            for (const auto& enemy : enemies) {
                enemy->Draw();
            }
            
            // Draw pause overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
            DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2, screenHeight / 2 - 40, 40, WHITE);
            DrawText("Press P to resume", screenWidth / 2 - MeasureText("Press P to resume", 20) / 2, screenHeight / 2 + 20, 20, WHITE);
            break;
            
        case GameState::GAME_OVER:
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, screenHeight / 3, 60, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - MeasureText(TextFormat("Final Score: %d", score), 30) / 2, screenHeight / 2, 30, BLACK);
            DrawText("Press ENTER to restart", screenWidth / 2 - MeasureText("Press ENTER to restart", 20) / 2, screenHeight * 2 / 3, 20, DARKGRAY);
            break;
            
        case GameState::VICTORY:
            DrawText("VICTORY!", screenWidth / 2 - MeasureText("VICTORY!", 60) / 2, screenHeight / 3, 60, GREEN);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - MeasureText(TextFormat("Final Score: %d", score), 30) / 2, screenHeight / 2, 30, BLACK);
            DrawText("Press ENTER to restart", screenWidth / 2 - MeasureText("Press ENTER to restart", 20) / 2, screenHeight * 2 / 3, 20, DARKGRAY);
            break;
    }
    
    EndDrawing();
}

void Game::SpawnEnemies(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disPosX(0, screenWidth - 50);
    std::uniform_real_distribution<> disPosY(0, screenHeight - 50);
    std::uniform_real_distribution<> disSpeed(0, 2);
    
    const EntityConfig& enemyConfig = configManager->GetEnemyConfig(scenario.currentWave);
    
    for (int i = 0; i < count; ++i) {
        float x = disPosX(gen);
        float y = disPosY(gen);
        
        // Ensure enemy doesn't spawn too close to player
        Rectangle playerRect = player->GetRectangle();
        float minDistance = 150.0f;
        
        // Keep generating positions until we find one far enough away
        while (std::sqrt(std::pow(x - playerRect.x, 2) + std::pow(y - playerRect.y, 2)) < minDistance) {
            x = disPosX(gen);
            y = disPosY(gen);
        }
        
        float speedX = disSpeed(gen);
        float speedY = disSpeed(gen);
        
        // 50% chance to reverse direction
        if (std::uniform_int_distribution<>(0, 1)(gen)) speedX = -speedX;
        if (std::uniform_int_distribution<>(0, 1)(gen)) speedY = -speedY;
        
        enemies.emplace_back(std::make_unique<Enemy>(enemyConfig, x, y, speedX, speedY));
    }
}

void Game::CheckAttackCollisions() {
    if (!player->IsAttacking()) {
        return;
    }
    
    Rectangle attackArea = GetAttackArea(player->GetRectangle());
    
    for (auto& enemy : enemies) {
        if (CheckCollisionRecs(attackArea, enemy->GetRectangle())) {
            HandleEnemyHit(enemy);
        }
    }
    
    RemoveDeadEnemies();
    
    // Check if all enemies are defeated to start new wave
    if (enemies.empty()) {
        if (scenario.currentWave < scenario.maxWaves - 1) {
            StartNewWave();
        } else {
            Victory();
        }
    }
}

void Game::CheckPlayerEnemyCollisions() {
    if (!player->IsAlive()) return;
    
    Rectangle playerRect = player->GetRectangle();
    
    for (const auto& enemy : enemies) {
        if (CheckCollisionRecs(playerRect, enemy->GetRectangle())) {
            player->TakeDamage();
            
            // Check if player died
            if (!player->IsAlive()) {
                GameOver();
                break;
            }
        }
    }
}

Rectangle Game::GetAttackArea(const Rectangle& playerRect) {
    float attackRadius = playerRect.width * 1.5f;
    return {
        playerRect.x + playerRect.width / 2 - attackRadius,
        playerRect.y + playerRect.height / 2 - attackRadius,
        attackRadius * 2,
        attackRadius * 2
    };
}

void Game::HandleEnemyHit(std::unique_ptr<Enemy>& enemy) {
    enemy->OnHit();
    
    if (enemy->GetHealth() <= 0) {
        score += enemy->GetPoints() * (scenario.currentWave + 1);
    }
}

void Game::RemoveDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return enemy->GetHealth() <= 0; }),
        enemies.end()
    );
}

void Game::StartNewWave() {
    scenario.currentWave++;
    SpawnEnemies(scenario.enemiesPerWave);
}

void Game::GameOver() {
    gameState = GameState::GAME_OVER;
}

void Game::Victory() {
    gameState = GameState::VICTORY;
}

void Game::DrawUI() {
    // Draw score
    DrawText(TextFormat("Score: %d", score), 10, 40, 20, BLACK);
    
    // Draw wave information
    DrawText(TextFormat("Wave: %d/%d", scenario.currentWave + 1, scenario.maxWaves), 10, 70, 20, BLACK);
    
    // Draw enemies remaining
    DrawText(TextFormat("Enemies: %d", static_cast<int>(enemies.size())), 10, 100, 20, BLACK);
}

void Game::HandleMenuState() {
    if (IsKeyPressed(KEY_ENTER)) {
        // Reset game state
        Initialize();
        
        // Start first wave
        SpawnEnemies(
            scenario.enemiesPerWave
        );
        
        gameState = GameState::PLAYING;
    }
}

void Game::HandlePlayingState(float deltaTime) {
    // Update game time
    gameTimer += deltaTime;
    
    // Update player
    player->Update(*inputHandler);
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime);
    }
    
    // Check for pause
    if (inputHandler->IsPausePressed()) {
        gameState = GameState::PAUSED;
        return;
    }
    
    // Check collisions
    CheckAttackCollisions();
    CheckPlayerEnemyCollisions();
}

void Game::HandlePausedState() {
    if (inputHandler->IsPausePressed()) {
        gameState = GameState::PLAYING;
    }
}

void Game::HandleGameOverState() {
    if (IsKeyPressed(KEY_ENTER)) {
        // Reset game and go to menu
        Initialize();
        gameState = GameState::MENU;
    }
}

void Game::HandleVictoryState() {
    if (IsKeyPressed(KEY_ENTER)) {
        // Reset game and go to menu
        Initialize();
        gameState = GameState::MENU;
    }
}