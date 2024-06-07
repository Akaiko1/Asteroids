#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>

struct Scenario {
    int totalEnemies;
};

class Player {
public:
    Player(float x, float y, float size, float speed);
    void Update();
    void Draw() const;
    Rectangle GetRectangle() const;
    bool IsAttacking() const;

private:
    Rectangle player;
    float speed;
    bool attacking;
};

class Enemy {
public:
    Enemy(float x, float y, float size, float speedX, float speedY, int health, Color color);
    void Update();
    void Draw() const;
    void OnHit();
    Rectangle GetRectangle() const;
    int GetHealth() const;

private:
    Rectangle enemy;
    Color color;
    float speedX;
    float speedY;
    int health;
};

class Game {
public:
    Game(int screenWidth, int screenHeight);
    void Run();

private:
    int screenWidth;
    int screenHeight;
    Scenario scenario;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;

    void SpawnEnemies(int, int, Color);
    void CheckCollisions();
    Rectangle GetAttackArea(const Rectangle&);
    void HandleEnemyHit(std::unique_ptr<Enemy>&);
    void RemoveDeadEnemies();
};

#endif // GAME_H
