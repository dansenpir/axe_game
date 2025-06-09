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
    // 'speed' is defined in pixels per second.
    void Move(int screenWidth, int screenHeight, float speed) {
        float deltaTime = GetFrameTime(); // Get the time elapsed since the last frame in seconds.
                                          // This is crucial for frame-rate independent movement.
                                          // Movement is calculated based on time, not frames,
                                          // ensuring consistent speed regardless of FPS fluctuations.
        // Calculate movement in pixels per frame based on speed and deltaTime.
        // The result is cast to int because pixel positions are typically integer values.
        int movementAmount = static_cast<int>(speed * deltaTime); 

        // Update position based on input with boundary checks.
        // The player's center (x, y) must always be within the screen bounds,
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
// Similar to Player, using a struct for Axe promotes modularity and encapsulates
// all axe-related data and functions within a single, coherent unit.
struct Axe {
    int x;       // X position of the top-left corner of the square axe
    int y;       // Y position of the top-left corner of the square axe
    int length;  // Side length of the square axe
    float speedX; // Horizontal movement speed of the axe (pixels per second).
                  // Using float for speed allows for more precise, fractional speed values.
    float speedY; // Vertical movement speed of the axe (pixels per second).
    Color color; // Color of the axe

    // Draw the axe on the screen.
    void Draw() {
        DrawRectangle(x, y, length, length, color);
    }

    // Move the axe in both horizontal and vertical directions, bouncing off all screen edges.
    // The movement is time-based using deltaTime to ensure smooth and consistent motion
    // regardless of the game's frame rate.
    void Move(int screenWidth, int screenHeight) {
        float deltaTime = GetFrameTime(); // Crucial for frame-rate independent movement.
        x += static_cast<int>(speedX * deltaTime); // Update X position based on horizontal speed.
        y += static_cast<int>(speedY * deltaTime); // Update Y position based on vertical speed.

        // Reverse horizontal direction if axe hits left or right edge.
        // The axe's x-coordinate refers to its top-left corner.
        // So, for the right edge, we check x + length.
        if (x + length > screenWidth || x < 0) {
            speedX = -speedX; // Invert horizontal speed to bounce.
        }
        // Reverse vertical direction if axe hits top or bottom edge.
        // Similar logic applies for the bottom edge: y + length.
        if (y + length > screenHeight || y < 0) {
            speedY = -speedY; // Invert vertical speed to bounce.
        }
    }
};

// Function to check collision between the player (circle) and the axe (rectangle).
// This function acts as a wrapper for Raylib's optimized collision detection.
// It takes the Player and Axe structs directly, making the call site cleaner.
bool CheckCollision(Player player, Axe axe) {
    // Convert the axe's properties into a Raylib Rectangle type.
    // Raylib's collision functions often require its specific data structures.
    Rectangle axeRect = {static_cast<float>(axe.x), static_cast<float>(axe.y),
                         static_cast<float>(axe.length), static_cast<float>(axe.length)};
    
    // Use Raylib's optimized function for circle-rectangle collision detection.
    // Vector2 is used for the circle's center.
    return CheckCollisionCircleRec(Vector2{static_cast<float>(player.x), static_cast<float>(player.y)},
                                   static_cast<float>(player.radius), axeRect);
}

// Enum to manage different distinct states of the game.
// Using an enum for game states is a common and effective way to structure game logic,
// making the code more readable, maintainable, and less prone to errors
// compared to using magic numbers or boolean flags for state.
enum GameState {
    MENU,       // Initial game state: displays a start screen.
    PLAYING,    // Active gameplay state: player and axe move, collision is checked, score updates.
    GAME_OVER   // State after a collision occurs: displays game over message and restart option.
};

int main() {
    // Window configuration constants. Using 'const' ensures these values cannot be accidentally changed,
    // improving code robustness and readability.
    const int screenWidth = 800;
    const int screenHeight = 450;
    const char* windowTitle = "Dan's Axe Game";

    // Initialize the game window with specified dimensions and title.
    InitWindow(screenWidth, screenHeight, windowTitle); 
    // Set the target frames per second (FPS) to 60. This helps ensure consistent game speed
    // and smooth animation, especially when combined with deltaTime-based movement.
    SetTargetFPS(60); 

    // Initialize game entities with their starting properties.
    Player player = {screenWidth / 2, screenHeight / 2, 25, PURPLE}; 
    // Axe starts with initial horizontal speed (speedX) and vertical speed (speedY),
    // creating an immediate diagonal movement.
    Axe axe = {300, 0, 50, 150.0f, 200.0f, RED}; 

    // Set the initial game state to MENU.
    GameState currentState = MENU; 

    // Variables for scoring system.
    int score = 0;         // Current score based on survival time (1 point per second).
    int highScore = 0;     // Highest score achieved in any game session so far (in-memory).
    float scoreTimer = 0.0f; // Timer to accumulate time for scoring (in seconds).
    // Tracks the score at which the axe's speed was last increased.
    // This prevents the speed from increasing multiple times for the same score point
    // if the game loop runs very fast.
    int lastSpeedIncreaseScore = 0; 

    // Variable to track if a collision was detected (primarily for debug visualization).
    bool collisionDetected = false;

    // Main game loop. Continues as long as the window is not closed.
    // This loop handles game state updates, input processing, and rendering for each frame.
    while (!WindowShouldClose()) { 
        BeginDrawing(); // Start the drawing phase. All drawing commands between BeginDrawing()
                        // and EndDrawing() are buffered and then drawn to the screen.
        ClearBackground(WHITE); // Clear the screen with a white color for a fresh frame.
                                // This prevents "smearing" or drawing artifacts from previous frames.

        // Game logic and rendering based on the current game state.
        // The switch statement elegantly manages transitions and behaviors for different game phases.
        switch (currentState) {
            case MENU:
                // Display instructions for starting the game.
                // MeasureText is used to center the text dynamically.
                DrawText("Press SPACE to Start", screenWidth / 2 - MeasureText("Press SPACE to Start", 20) / 2, screenHeight / 2 - 10, 20, BLACK);
                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset game state variables for a new game.
                    // This ensures a fresh start each time the game is played.
                    player.x = screenWidth / 2;
                    player.y = screenHeight / 2;
                    axe.x = 300;
                    axe.y = 0;
                    axe.speedX = 150.0f; // Initial horizontal speed.
                    axe.speedY = 200.0f; // Initial vertical speed.
                    score = 0; 
                    scoreTimer = 0.0f; 
                    lastSpeedIncreaseScore = 0;
                    currentState = PLAYING; // Transition to the PLAYING state.
                }
                break;

            case PLAYING:
                // Update game entities' positions.
                player.Move(screenWidth, screenHeight, 300.0f); // Player speed set to 300 pixels/second.
                axe.Move(screenWidth, screenHeight); // Axe moves in both directions based on its speeds.

                // Update score based on survival time (1 point per second).
                scoreTimer += GetFrameTime(); // Accumulate time. GetFrameTime() provides the time
                                              // elapsed since the last frame, ensuring time-based scoring.
                if (scoreTimer >= 1.0f) { // Every second, increment score.
                    score += 1;
                    scoreTimer -= 1.0f; // Subtract 1.0f to maintain precision for remaining time.
                                        // This prevents scoreTimer from growing indefinitely and losing precision.
                }

                // Increase axe speed every 10 points to make the game progressively harder.
                // This dynamic difficulty scaling keeps the game challenging as the player improves.
                if (score > lastSpeedIncreaseScore && score % 10 == 0) {
                    // Increase speed by 10% each time, up to a maximum.
                    // Capping the speed prevents the "tunneling" effect (where objects move so fast
                    // they pass through others without collision detection) and keeps the game playable.
                    if (axe.speedX < 300.0f) { // Maximum horizontal speed.
                        axe.speedX *= 1.1f; // Increase by 10%.
                    }
                    if (axe.speedY < 400.0f) { // Maximum vertical speed.
                        axe.speedY *= 1.1f; // Increase by 10%.
                    }
                    lastSpeedIncreaseScore = score; // Update the last score at which speed was increased.
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
                    // This is helpful during development to verify collision logic.
                    DrawCircleLines(player.x, player.y, player.radius, BLACK);
                    DrawRectangleLines(axe.x, axe.y, axe.length, axe.length, BLACK);
                }
                // Display current score in the top-left corner.
                // TextFormat is a convenient Raylib function for creating formatted strings.
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
                    // This prepares the game for a new round.
                    player.x = screenWidth / 2;
                    player.y = screenHeight / 2;
                    axe.x = 300;
                    axe.y = 0;
                    axe.speedX = 150.0f; // Reset horizontal speed.
                    axe.speedY = 200.0f; // Reset vertical speed.
                    score = 0; // Reset score for new game.
                    scoreTimer = 0.0f; // Reset timer for scoring.
                    lastSpeedIncreaseScore = 0; // Reset speed increase tracker.
                    currentState = PLAYING; // Return to PLAYING state to restart the game.
                }
                break;
        }

        EndDrawing(); // End the drawing phase and display the frame.
    }

    CloseWindow(); // Close the window and release Raylib resources.
    return 0;      // Return 0 to indicate successful execution.
}