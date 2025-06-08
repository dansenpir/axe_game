#include "raylib.h"
int main()
{
    int width{800};
    int height{450};
    const char *title{"Dan's Window"};

    InitWindow(width, height, title);

    // circle coordinates and radius
    int circle_x{200};
    int circle_y{200};
    int circle_radius{25};
    // circle edges
    int l_circle_x{circle_x - circle_radius};
    int r_circle_x{circle_x + circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int b_circle_y{circle_y + circle_radius};

    // axe dimensions
    int axe_x{300};
    int axe_y{0};
    int axe_length{50};
    // axe edges
    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_length};
    int u_axe_y{axe_y};
    int b_axe_y{axe_y + axe_length};

    int direction{10};

    bool collision_with_axe{(b_axe_y >= u_circle_y) &&
                            (u_axe_y <= b_circle_y) &&
                            (r_axe_x >= l_circle_x) &&
                            (l_axe_x <= r_circle_x)};
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (collision_with_axe)
        {
            DrawText("Game Over", 400, 200, 20, RED);
        }
        else
        {

            // update circle edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            b_circle_y = circle_y + circle_radius;

            // update axe edges
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_length;

            // update collision detection
            collision_with_axe = (b_axe_y >= u_circle_y) &&
                                 (u_axe_y <= b_circle_y) &&
                                 (r_axe_x >= l_circle_x) &&
                                 (l_axe_x <= r_circle_x);

            DrawCircle(circle_x, circle_y, circle_radius, PURPLE);
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            axe_y += direction;
            if (axe_y > height - axe_length || axe_y < 0)
            {
                direction = -direction;
            }

            if ((b_axe_y >= u_circle_y) &&
                (u_axe_y <= b_circle_y) &&
                (r_axe_x >= l_circle_x) &&
                (l_axe_x <= r_circle_x))
            {
                collision_with_axe = true;
            }

            if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && circle_x < width - circle_radius)
            {
                circle_x += 10;
            }
            if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && circle_x > circle_radius)
            {
                circle_x -= 10;
            }
            if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && circle_y > circle_radius)
            {
                circle_y -= 10;
            }
            if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && circle_y < height - circle_radius)
            {
                circle_y += 10;
            }
        }
        EndDrawing();
    }
}
