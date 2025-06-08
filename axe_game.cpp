#include "raylib.h" // Include the Raylib library for game development functionalities

// Player structure to encapsulate the player's properties and behaviors.
// Using a struct helps organize related data and functions, improving code readability and maintainability.
// It promotes encapsulation by bundling data (x, y, radius, color) and functions (Draw, Move) that operate on that data.
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
    // 'speed' is defined in pixels per second.
    void Move(int screenWidth, int screenHeight, float speed) {
        // Get the time elapsed since the last frame in seconds.
        // This is crucial for frame-rate independent movement.
        // Without deltaTime, movement speed would vary with FPS.
        float deltaTime = GetFrameTime(); 
        
        // Calculate movement in pixels per frame based on speed and deltaTime.
        // static_cast<int> is used for explicit type conversion from float to int.
        int movementAmount = static_cast<int>(speed * deltaTime); 

        // Update position based on input with boundary checks.
        // The player's center (x, y) must stay within screen boundaries,
        // considering its radius to prevent drawing outside the window.
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
// Similar to Player, using a struct for Axe promotes modularity and organization.
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
        // The axe's y position refers to its top-left corner.
        // So, y + length is its bottom edge.
        if (y > screenHeight - length || y < 0) {
            speed = -speed; // Invert the speed to change direction (bounce effect)
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
    // This function is highly optimized for performance.
    return CheckCollisionCircleRec(Vector2{static_cast<float>(player.x), static_cast<float>(player.y)},
                                   static_cast<float>(player.radius), axeRect);
}

// Enum to manage different distinct states of the game.
// Using an enum for game states is a common and highly recommended design pattern (State Machine).
// It makes the game logic clear, organized, and easy to manage transitions between different behaviors.
enum GameState {
    MENU,       // Initial game state: displays a start screen.
    PLAYING,    // Active gameplay state: player and axe move, collision is checked, score updates.
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
    // This helps ensure consistent game speed across different machines
    // and provides a stable deltaTime for calculations.
    SetTargetFPS(60); 

    // Initialize game entities with their starting properties.
    Player player = {screenWidth / 2, screenHeight / 2, 25, PURPLE}; 
    Axe axe = {300, 0, 50, 200.0f, RED}; // Axe speed adjusted to 200 pixels/second.

    // Set the initial game state to MENU.
    GameState currentState = MENU; 

    // Variables for scoring system.
    int score = 0;         // Current score based on survival time (1 point per second).
    int highScore = 0;     // Highest score achieved in any game session so far (in-memory).
    float scoreTimer = 0.0f; // Timer to accumulate time for scoring (in seconds).

    // Variable to track if a collision was detected (for debug visualization).
    bool collisionDetected = false;

    // Main game loop. Continues as long as the window is not closed.
    // This loop handles game updates (logic) and rendering (drawing) for each frame.
    while (!WindowShouldClose()) { 
        BeginDrawing(); // Start the drawing phase. All drawing commands must be placed between BeginDrawing() and EndDrawing().
        ClearBackground(WHITE); // Clear the screen with a white color for a fresh frame.
                                // This prevents "smearing" effects from previous frames.

        // Game logic and rendering based on the current game state.
        switch (currentState) {
            case MENU:
                // Display instructions for starting the game.
                // MeasureText helps center the text by calculating its width.
                DrawText("Press SPACE to Start", screenWidth / 2 - MeasureText("Press SPACE to Start", 20) / 2, screenHeight / 2 - 10, 20, BLACK);
                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset game state variables for a new game.
                    // This ensures a clean start from the beginning.
                    player.x = screenWidth / 2;
                    player.y = screenHeight / 2;
                    axe.y = 0;
                    axe.speed = 200.0f; 
                    score = 0; 
                    scoreTimer = 0.0f; 
                    currentState = PLAYING; // Transition to the PLAYING state.
                }
                break;

            case PLAYING:
                // Update game entities' positions.
                player.Move(screenWidth, screenHeight, 300.0f); // Player speed set to 300 pixels/second.
                axe.Move(screenHeight); // Axe moves based on its own speed.

                // Update score based on survival time (1 point per second).
                scoreTimer += GetFrameTime(); // Accumulate time.
                if (scoreTimer >= 1.0f) { // Every second, increment score.
                    score += 1;
                    scoreTimer -= 1.0f; // Subtract 1.0f to maintain precision for remaining time.
                                        // e.g., if scoreTimer is 1.1s, it becomes 0.1s for the next second.
                }

                // Check for collision between player and axe.
                collisionDetected = CheckCollision(player, axe);
                if (collisionDetected) {
                    currentState = GAME_OVER; // Transition to GAME_OVER state on collision.
                }

                // Draw game entities with debug visualization for collision.
                player.Draw(); // Render the player.
                axe.Draw();    // Render the axe.
                if (collisionDetected) {
                    // Draw outlines around colliding objects for visual debugging.
                    DrawCircleLines(player.x, player.y, player.radius, BLACK);
                    DrawRectangleLines(axe.x, axe.y, axe.length, axe.length, BLACK);
                }
                // Display current score in the top-left corner.
                // TextFormat is used to create a formatted string with the current score.
                DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
                break;

            case GAME_OVER:
                // Update high score if current score is higher.
                if (score > highScore) {
                    highScore = score;
                }
                // Display game over messages with current and high score.
                DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 50, 40, RED);
                DrawText(TextFormat("Your Score: %i", score), screenWidth / 2 - MeasureText(TextFormat("Your Score: %i", score), 20) / 2, screenHeight / 2 - 10, 20, BLACK);
                DrawText(TextFormat("High Score: %i", highScore), screenWidth / 2 - MeasureText(TextFormat("High Score: %i", highScore), 20) / 2, screenHeight / 2 + 20, 20, BLACK);
                DrawText("Press R to Restart", screenWidth / 2 - MeasureText("Press R to Restart", 20) / 2, screenHeight / 2 + 50, 20, BLACK);
                
                // Reset game state on 'R' key press.
                if (IsKeyPressed(KEY_R)) {
                    // Reset player and axe positions to their initial values.
                    player.x = screenWidth / 2;
                    player.y = screenHeight / 2;
                    axe.y = 0;
                    axe.speed = 200.0f; // Reset axe speed to its initial value.
                    score = 0; // Reset score for new game.
                    scoreTimer = 0.0f; // Reset timer for scoring.
                    currentState = PLAYING; // Return to PLAYING state to restart the game.
                }
                break;
        }

        EndDrawing(); // End the drawing phase and display the frame.
                      // This swaps the back buffer to the front, making all drawn elements visible.
    }

    CloseWindow(); // Close the window and release Raylib resources.
                   // This is important for proper cleanup and avoiding resource leaks.
    return 0;      // Return 0 to indicate successful execution.
}