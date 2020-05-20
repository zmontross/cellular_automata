/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Logo Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2020 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "raygui.h"
#include "ricons.h"

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Logo screen global variables
static int framesCounter;
static int finishScreen;


int logoPositionX;
int logoPositionY;

int lettersCount = 0;

int topSideRecWidth = 16;
int leftSideRecHeight = 16;

int bottomSideRecWidth = 16;
int rightSideRecHeight = 16;

int state = 0;                  // Tracking animation states (State Machine)
float alpha = 1.0f;             // Useful for fading


//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

// Logo Screen Initialization logic
void InitLogoScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;

    logoPositionX = screen.width.half - 128;
    logoPositionY = screen.height.half - 128;
}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{
    
    // Quick Skip

    if(IsKeyPressed(KEY_ESCAPE)){
        finishScreen = true;
    }



    // Update
    //----------------------------------------------------------------------------------
    if (state == 0)                 // State 0: Small box blinking
    {
        framesCounter++;

        if (framesCounter == 120)
        {
            state = 1;
            framesCounter = 0;      // Reset counter... will be used later...
        }
    }
    else if (state == 1)            // State 1: Top and left bars growing
    {
        topSideRecWidth += 4;
        leftSideRecHeight += 4;

        if (topSideRecWidth == 256) state = 2;
    }
    else if (state == 2)            // State 2: Bottom and right bars growing
    {
        bottomSideRecWidth += 4;
        rightSideRecHeight += 4;

        if (bottomSideRecWidth == 256) state = 3;
    }
    else if (state == 3)            // State 3: Letters appearing (one by one)
    {
        framesCounter++;

        if (framesCounter/12)       // Every 12 frames, one more letter!
        {
            lettersCount++;
            framesCounter = 0;
        }

        if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
        {
            alpha -= 0.02f;

            if (alpha <= 0.0f)
            {
                alpha = 0.0f;
                state = 4;
                framesCounter = 0;
            }
        }
    }
    else if (state == 4)            // State 4: Reset and Replay
    {
        framesCounter++;
        if(framesCounter == 30){
            finishScreen = true;
        }
        // if (IsKeyPressed('R'))
        // {
        //     framesCounter = 0;
        //     lettersCount = 0;

        //     topSideRecWidth = 16;
        //     leftSideRecHeight = 16;

        //     bottomSideRecWidth = 16;
        //     rightSideRecHeight = 16;

        //     alpha = 1.0f;
        //     state = 0;          // Return to State 0
        // }
    }


}

// Logo Screen Draw logic
void DrawLogoScreen(void)
{
    DrawRectangle(0,0,screen.width.full,screen.height.full, DARKGRAY);

    if (state == 0)
    {
        if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
    }
    else if (state == 1)
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
    }
    else if (state == 2)
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

        DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
    }
    else if (state == 3)
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

        DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

        DrawRectangle(screen.width.half - 112, screen.height.half - 112, 224, 224, Fade(RAYWHITE, alpha));

        DrawText(TextSubtext("raylib", 0, lettersCount), screen.width.half - 44, screen.height.half + 48, 50, Fade(BLACK, alpha));
    }
    else if (state == 4)
    {
        //DrawText("[R] REPLAY", 340, 200, 20, GRAY);
        //DrawRectangle(0, 0, screen.width.full, screen.height.full, Fade(BLACK, (framesCounter/60) ));
    }
}

// Logo Screen Unload logic
void UnloadLogoScreen(void)
{
}

// Logo Screen should finish?
int FinishLogoScreen(void)
{
    return finishScreen;
}