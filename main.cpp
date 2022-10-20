/*******************************************************************************************
*
*   raylib [core] example - 2d camera
*
*   This example has been created using raylib 1.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <string.h>
#include <stdlib.h>

#define MAX_CHARS   255

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920/2;
    const int screenHeight = 1080/2;

    InitWindow(screenWidth, screenHeight, "The absolute, bare minimum [working]");

    SetTargetFPS(240);                   // Set our game to run at 60 frames-per-second

    //Load tank sprite
    Texture2D sprite = LoadTexture("tankSprite.png");

    //Initial location for sprite and some other objects.
    int x = screenWidth/2 - 80;
    int y = screenHeight/2;

    //Confirm / Apply button and default tank speed
    Rectangle confirmButton = {screenWidth/2+120, screenHeight/2-170, 80, 40};
    int speed = 1;

    //Setting up array to store entered speed 
    char inputArray[MAX_CHARS + 1] = {speed + '0', '\0'}; //Default value displayed
    int charCount = 0;
    Rectangle textBox = {screenWidth/2-100, screenHeight/2-170, 200, 40};

    //Animate button and default projectile position
    Rectangle animateButton = {screenWidth/2+220, screenHeight/2-170, 120, 40};
    bool animating = false;
    Vector2 projectilePosition = {0, 0};

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        //Only animate projectile IF animating currently
        if (animating) {
            projectilePosition.x++;
            projectilePosition.y++;
            if (projectilePosition.y >=500) animating = false; //location of line, alternatively could detect collision
        } 

        //Check if mouse is over box; capture numbers
        if (CheckCollisionPointRec(GetMousePosition(), textBox)){
            
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                if ((key >= 48) && (key <= 57) && (charCount < MAX_CHARS))
                {
                    inputArray[charCount] = (char)key;
                    inputArray[charCount+1] = '\0'; // Add null terminator at the end of the string.
                    charCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE)){
                charCount--;
                if (charCount < 0) charCount = 0;
                inputArray[charCount] = '\0';
            } else if (IsKeyPressed(KEY_H)) { //H resets tank to center in case you lose it lol. Still need cursor on box though
                x = screenWidth/2 - 80;
                y = screenHeight/2;
            }
        }

        //Check if apply is clicked
        if (CheckCollisionPointRec(GetMousePosition(), confirmButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            speed = atoi(inputArray); //Safe because array can only ever contain digits 0-9
        }

        //Check if animate is clicked
        if (CheckCollisionPointRec(GetMousePosition(), animateButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            animating = true;
            projectilePosition.x = screenWidth/2 - 80;
            projectilePosition.y = screenHeight/2;
        }

        //Move to right or left
        if (IsKeyDown(KEY_A)) x-=speed;
        else if (IsKeyDown(KEY_D)) x+=speed;

        //Move up or down
        if (IsKeyDown(KEY_W)) y-=speed;
        else if (IsKeyDown(KEY_S)) y+=speed;

        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            //Draw tank
            DrawTexture(sprite, x, y, WHITE);

            //Show title text
            DrawText("Tankinematics!", screenWidth/2 - 80, screenHeight/2-240, 20, DARKGREEN);
            DrawText(TextFormat("Position: %i, %i [x, y]", x, y), screenWidth/2 - 120, screenHeight/2 - 200, 20, DARKGRAY);

            //Speed modifier textBox, it's current value and label (to left)
            DrawText("Speed Modifier:", textBox.x - 200, textBox.y + 8, 25, DARKGRAY);
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawText(inputArray, textBox.x + 5, textBox.y + 8, 25, BLACK);
            
            //Apply button and text
            DrawRectangleRec(confirmButton, RED);
            DrawText("Apply", confirmButton.x + 7, confirmButton.y + 6, 25, DARKGRAY);

            //Animate button and text
            DrawRectangleRec(animateButton, GREEN);
            DrawText("Animate", animateButton.x + 14, animateButton.y + 7, 25, DARKGRAY);

            //Green line at bottom 
            DrawLine(0, 500, 960, 505, GREEN);

            //If animating, make projectile visible, otherwise invisible
            if (animating) {
                DrawCircle(projectilePosition.x, projectilePosition.y, 10.0f, BLACK);
            } else {
                DrawCircle(projectilePosition.x, projectilePosition.y, 10.0f, BLANK);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(sprite);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
