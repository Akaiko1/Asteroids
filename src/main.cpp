#include "game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    Game game(screenWidth, screenHeight);
    game.Run();

    return 0;
}