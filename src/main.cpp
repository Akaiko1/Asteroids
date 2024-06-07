#include "game.h"

// Main function to start the game
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    Game game(screenWidth, screenHeight);
    game.Run();

    return 0;
}
