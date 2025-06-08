#include "raylib.h" // Include the Raylib library for game development functionalities

// Player structure to encapsulate the player's properties and behaviors.
// Using a struct helps organize related data and functions, improving code readability and maintainability.
struct Player {
    int x;       // X position of the circle's center on the screen
    int y;       // Y position of the circle's center on the screen
    int radius;  // Radius of the circular player
    Color color; // Color of the player

    // Draw the player on the screen. This method knows how to render itself.
    void Draw() {
        DrawCircle(x, y, radius, color);
    }

    // Move the player based on keyboard input, respecting screen boundaries.
    // 'speed' is now defined in pixels per second.
    void Move(int screenWidth, int screenHeight, float speed) {
        float deltaTime = GetFrameTime(); // Get the time elapsed since the last frame in seconds.
        // Calculate movement in pixels per frame based on speed (pixels/second) and deltaTime (seconds).
        // This ensures frame-rate independent movement: (pixels/second) * (seconds/frame) = pixels/frame.
        // The 60.0f multiplier was removed as deltaTime already handles frame rate variations.
        int movementAmount = static_cast<int>(speed * deltaTime); 

        // Update position based on input with boundary checks.
        // Boundary checks prevent the player from moving off-screen.
        if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && x < screenWidth - radius) {
            x += movementAmount;
        }
        if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && x > radius) {
            x -= movementAmount;
        }
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && y > radius) {
            y -= movementAmount;
        }
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && y < screenHeight - radius) {
            y += movementAmount;
        }
    }
};

// Axe structure to encapsulate the obstacle's properties and behaviors.
// Similar to Player, using a struct for Axe promotes modularity.
struct Axe {
    int x;       // X position of the top-left corner of the square axe
    int y;       // Y position of the top-left corner of the square axe
    int length;  // Side length of the square axe
    float speed; // Movement speed of the axe (pixels per second)
    Color color; // Color of the axe

    // Draw the axe on the screen.
    void Draw() {
        DrawRectangle(x, y, length, length, color);
    }

    // Move the axe vertically, bouncing off screen edges.
    void Move(int screenHeight) {
        float deltaTime = GetFrameTime(); // Crucial for frame-rate independent movement.
        y += static_cast<int>(speed * deltaTime); // Update Y position based on speed and deltaTime.

        // Reverse direction if axe hits top or bottom edge.
        // This creates a simple bouncing behavior.
        if (y > screenHeight - length || y < 0) {
            speed = -speed; // Invert the speed to change direction
        }
    }
};

// Function to check collision between the player (circle) and the axe (rectangle).
// This function acts as a wrapper for Raylib's optimized collision detection.
bool CheckCollision(Player player, Axe axe) {
    // Convert the axe's properties into a Raylib Rectangle type.
    // Raylib's collision functions often require specific data types.
    Rectangle axeRect = {static_cast<float>(axe.x), static_cast<float>(axe.y),
                         static_cast<float>(axe.length), static_cast<float>(axe.length)};
    
    // Use Raylib's optimized function for circle-rectangle collision detection.
    // CheckCollisionCircleRec is efficient and accurate for these primitive shapes.
    return CheckCollisionCircleRec(Vector2{static_cast<float>(player.x), static_cast<float>(player.y)},
                                   static_cast<float>(player.radius), axeRect);
}

// Enum to manage different distinct states of the game.
// Using an enum for game states is a common and effective way to structure game logic,
// creating a simple state machine that controls what happens at any given moment.
enum GameState {
    MENU,       // Initial game state: displays a start screen.
    PLAYING,    // Active gameplay state: player and axe move, collision is checked.
    GAME_OVER   // State after a collision occurs: displays game over message and restart option.
};

int main() {
    // Window configuration constants.
    const int screenWidth = 800;
    const int screenHeight = 450;
    const char* windowTitle = "Dan's Axe Game";

    // Initialize the game window with specified dimensions and title.
    InitWindow(screenWidth, screenHeight, windowTitle); 
    // Set the target frames per second (FPS) to 60.
    // This helps ensure consistent game speed across different machines,
    // especially when coupled with deltaTime.
    SetTargetFPS(60); 

    // Initialize game entities with their starting properties.
    // Player starts in the center, axe at the top.
    Player player = {screenWidth / 2, screenHeight / 2, 25, PURPLE}; 
    Axe axe = {300, 0, 50, 200.0f, RED}; // Axe speed adjusted to 200 pixels/second.

    // Set the initial game state to MENU.
    GameState currentState = MENU; 

    // Variable to track if a collision was detected (for debug visualization).
    bool collisionDetected = false;

    // Main game loop. Continues as long as the window is not closed.
    // This loop is the heart of the game, handling updates and rendering.
    while (!WindowShouldClose()) { 
        BeginDrawing(); // Start the drawing phase. All drawing commands go between BeginDrawing and EndDrawing.
        ClearBackground(WHITE); // Clear the screen with a white color for a fresh frame.

        // Game logic and rendering based on the current game state.
        // The switch statement effectively manages the game's flow.
        switch (currentState) {
            case MENU:
                // Display instructions for starting the game.
                DrawText("Press SPACE to Start", screenWidth / 2 - MeasureText("Press SPACE to Start", 20) / 2, screenHeight / 2 - 10, 20, BLACK);
                if (IsKeyPressed(KEY_SPACE)) {
                    currentState = PLAYING; // Transition to the PLAYING state.
                }
                break;

            case PLAYING:
                // Update game entities' positions.
                // Player speed set to 300 pixels/second.
                player.Move(screenWidth, screenHeight, 300.0f); 
                axe.Move(screenHeight); // Axe moves based on its own speed.

                // Check for collision between player and axe.
                collisionDetected = CheckCollision(player, axe);
                if (collisionDetected) {
                    currentState = GAME_OVER; // If collision detected, transition to GAME_OVER state.
                }

                // Draw game entities with debug visualization for collision.
                player.Draw(); // Render the player.
                axe.Draw();    // Render the axe.
                // If collision is detected, draw an outline around the player and axe for visual confirmation.
                // This is a valuable debugging technique to confirm collision detection.
                if (collisionDetected) {
                    DrawCircleLines(player.x, player.y, player.radius, BLACK);
                    DrawRectangleLines(axe.x, axe.y, axe.length, axe.length, BLACK);
                }
                break;

            case GAME_OVER:
                // Display game over messages.
                DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 50, 40, RED);
                DrawText("Press R to Restart", screenWidth / 2 - MeasureText("Press R to Restart", 20) / 2, screenHeight / 2 + 10, 20, BLACK);
                
                // Reset game state on 'R' key press.
                if (IsKeyPressed(KEY_R)) {
                    // Reset player and axe positions to their initial values.
                    player.x = screenWidth / 2;
                    player.y = screenHeight / 2;
                    axe.y = 0;
                    axe.speed = 200.0f; // Reset axe speed to its initial value (positive to start moving down).
                    currentState = PLAYING; // Return to PLAYING state to restart the game.
                }
                break;
        }

        EndDrawing(); // End the drawing phase and display the frame.
    }

    CloseWindow(); // Close the window and release Raylib resources.
    return 0;      // Return 0 to indicate successful execution.
}