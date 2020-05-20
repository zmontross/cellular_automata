/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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

// Title screen global variables
static int framesCounter;
static int finishScreen;


//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;

}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // Derp
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);

    DrawText(applicationName, (screen.width.half - MeasureText(applicationName, 40)/2), screen.height.quart, 40, LIGHTGRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    if (GuiButton((Rectangle){ (screen.width.half - gd.bWidths.half), (screen.height.half - gd.bHeights.full), gd.bWidths.full, gd.bHeights.full }, GuiIconText(RICON_OK_TICK, "Play"))) finishScreen = 1;
    if (GuiButton((Rectangle){ (screen.width.half - gd.bWidths.half), (screen.height.half + gd.bHeights.full), gd.bWidths.full, gd.bHeights.full }, GuiIconText(RICON_HELP, "Options"))) finishScreen = 2;
    if (GuiButton((Rectangle){ (screen.width.half - gd.bWidths.half), (screen.height.half + gd.bHeights.full*3), gd.bWidths.full, gd.bHeights.full }, GuiIconText(RICON_CROSS, "Exit"))) finishScreen = 3;
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}