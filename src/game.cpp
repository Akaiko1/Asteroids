#include "game.h"
#include <iostream>
#include <random>

// Player class implementation
Player::Player(float x, float y, float size, float speed)
    : player{ x, y, size, size }, speed(speed) {}

// Update the player's position based on key input
void Player::Update() {
    if (IsKeyDown(KEY_RIGHT)) player.x += speed;
    if (IsKeyDown(KEY_LEFT)) player.x -= speed;
    if (IsKeyDown(KEY_UP)) player.y -= speed;
    if (IsKeyDown(KEY_DOWN)) player.y += speed;

    attacking = IsKeyPressed(KEY_SPACE);
}

// Draw the player on the screen
void Player::Draw() const {
    DrawRectangleRec(player, BLUE);

    if (attacking) {
        DrawCircle(player.x + player.width / 2, player.y + player.height / 2, player.width, Fade(RED, 0.3f));
    }
}

// Get the player's rectangle
Rectangle Player::GetRectangle() const {
    return player;
}

// Check if the player is attacking
bool Player::IsAttacking() const {
    return attacking;
}

// Enemy class implementation

Enemy::Enemy(float x, float y, float size, float speedX, float speedY, int health, Color color)
    : enemy{ x, y, size, size }, speedX(speedX), speedY(speedY), health(health), color(color) {}

// Update the enemy's position and reverse direction upon hitting screen bounds
void Enemy::Update() {
    enemy.x += speedX;
    enemy.y += speedY;

    if (enemy.x <= 0 || enemy.x + enemy.width >= GetScreenWidth()) speedX = -speedX;
    if (enemy.y <= 0 || enemy.y + enemy.height >= GetScreenHeight()) speedY = -speedY;
}

// Draw the enemy on the screen
void Enemy::Draw() const {
    DrawRectangleRec(enemy, color);
}

// Get the enemy's rectangle
Rectangle Enemy::GetRectangle() const {
    return enemy;
}

// Draw the strong enemy on the screen
int Enemy::GetHealth() const {
    return health;
}

// Handle hit on the strong enemy
void Enemy::OnHit() {
    if (--health <= 0) {
        // Perform any death logic here if necessary
    }
}

// Game class implementation

Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      scenario{ 4 },
      player(std::make_unique<Player>(screenWidth / 2.0f - 25, screenHeight / 2.0f - 25, 50, 5.0f)) {
    SpawnEnemies(2, 1, RED); // Spawn two enemies
}

// Initialize enemies with random positions and speeds
void Game::SpawnEnemies(int numEnemies, int health, Color color) {
    std::random_device rd; // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister random number engine
    std::uniform_real_distribution<> disPosX(0, screenWidth - 50);
    std::uniform_real_distribution<> disPosY(0, screenHeight - 50);
    std::uniform_real_distribution<> disSpeed(1, 5);

    for (int i = 0; i < numEnemies; ++i) {
        float x = disPosX(gen);
        float y = disPosY(gen);
        float speedX = disSpeed(gen);
        float speedY = disSpeed(gen);
        enemies.emplace_back(std::make_unique<Enemy>(x, y, 50, speedX, speedY, health, color));
    }
}

// Check for collisions between player attacks and enemies
void Game::CheckCollisions() {
    if (!player->IsAttacking()) {
        return;
    }

    int spawnCount = 0;

    Rectangle attackArea = GetAttackArea(player->GetRectangle());

    for (auto& enemy : enemies) {
        if (CheckCollisionRecs(attackArea, enemy->GetRectangle())) {
            HandleEnemyHit(enemy);
        }
        if (enemy->GetHealth() <= 0) {
            if (--scenario.totalEnemies > 0) {
                spawnCount++;
            }
        }
    }

    RemoveDeadEnemies();

    if (spawnCount > 0) {
        SpawnEnemies(spawnCount, 2, MAROON);
    }
}

// Get the player's attack area
Rectangle Game::GetAttackArea(const Rectangle& playerRect) {
    return {
        playerRect.x - playerRect.width,
        playerRect.y - playerRect.height,
        playerRect.width * 3,
        playerRect.height * 3
    };
}

// Handle an enemy being hit
void Game::HandleEnemyHit(std::unique_ptr<Enemy>& enemy) {
    enemy->OnHit();
}

// Remove dead enemies from the list
void Game::RemoveDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return enemy->GetHealth() <= 0; }),
        enemies.end()
    );
}

// Run the game loop
void Game::Run() {
    InitWindow(screenWidth, screenHeight, "Move the Player with Arrow Keys");
    SetTargetFPS(60);
    srand(time(NULL));

    while (!WindowShouldClose()) {
        player->Update();
        CheckCollisions();

        for (auto& enemy : enemies) {
            enemy->Update();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        player->Draw();

        for (const auto& enemy : enemies) {
            enemy->Draw();
        }

        EndDrawing();
    }

    CloseWindow();
}