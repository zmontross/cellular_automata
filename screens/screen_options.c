/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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

// BUGS
//
//x // - Font size glitches following resolution changes. Fixes itself when leaving the menu. 'GuiSetStyle(DEFAULT, TEXT_SIZE, 20);' ???
//


#include "raylib.h"
#include "screens.h"
#include "raygui.h"
#include "ricons.h" // <----- THIS FUCKER RIGHT HERE

#define N_RESOLUTIONS 9

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------


// Options screen global variables
static int framesCounter;
static int finishScreen;


typedef struct Resolution{
    int width;
    int height;
    const char* label;
} Resolution;

// Array of resolution structs.
Resolution resolutions[N_RESOLUTIONS] = {
    { .width=3840, .height=1600, .label="3840 x 1600" },
    { .width=3440, .height=1440, .label="3440 x 1440" },
    { .width=2560, .height=1600, .label="2560 x 1600" },
    { .width=2560, .height=1440, .label="2560 x 1440" },
    { .width=2560, .height=1080, .label="2560 x 1080" },
    { .width=1920, .height=1200, .label="1920 x 1200" },
    { .width=1920, .height=1080, .label="1920 x 1080" },
    { .width=1680, .height=1050, .label="1680 x 1050" },
    { .width=1600, .height=900,  .label="1600 x 900" },
    // { .width=1440, .height=960,  .label="1440 x 960" },
    // { .width=1440, .height=900,  .label="1440 x 900" },
    // { .width=1280, .height=1024, .label="1280 x 1024" },
    // { .width=1280, .height=960,  .label="1280 x 960" },
    // { .width=1024, .height=768,  .label="1024 x 768" },
    // { .width=800, .height=600,   .label="800 x 600" },
};

const char* resLabels[N_RESOLUTIONS];


static int resLastAccepted;
static int resLastSelected;
static int resListViewFocus = -1;
static int resListViewScrollIndex = 0;


static bool applyChanges = false;

static bool setFullscreen = false;


//----------------------------------------------------------------------------------
// Options Screen Functions Definition
//----------------------------------------------------------------------------------

// Options Screen Initialization logic
void InitOptionsScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;

    applyChanges = false;
    setFullscreen = IsWindowFullscreen();

    // Get a pointer to the positions of the labels in the Resolutions struct array.
    for(int i=0; i<N_RESOLUTIONS; i++){
        resLabels[i] = *(&resolutions[i].label);
    }

    // Figure out which list item should be selected based on current resolution.
    resLastSelected = 0;
    for(int i=0; i<N_RESOLUTIONS; i++){
        if( (resolutions[i].width == useropts.screenWidth) && (resolutions[i].height == useropts.screenHeight) ){
            resLastSelected = i;
            break;
        }
    }

    // Check if we're already fullscreen.
    if( useropts.fullscreen || IsWindowFullscreen() ) setFullscreen = true;

}

// Options Screen Update logic
void UpdateOptionsScreen(void)
{
    if(IsKeyPressed(KEY_ESCAPE)) finishScreen = true;

    if(applyChanges){
        applyChanges = false;

        useropts.fullscreen = setFullscreen;
        useropts.screenWidth = resolutions[resLastAccepted].width;
        useropts.screenHeight = resolutions[resLastAccepted].height;

        SaveSettings(settings_file, &useropts);

        if( (useropts.fullscreen && !IsWindowFullscreen()) || (!useropts.fullscreen && IsWindowFullscreen()) ) ToggleFullscreen();

        UpdateResolution(useropts.screenWidth, useropts.screenHeight);

        UpdateGuiSizing();
    }
}

// Options Screen Draw logic
void DrawOptionsScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);
    
    DrawText("OPTIONS SCREEN", 20, 20, 40, LIGHTGRAY);


    // Rectangle


    // Resolution Options
    Rectangle rectResList = (Rectangle){
        .x = screen.width.half - gd.bWidths.half,
        .y = screen.height.quart,
        .width = gd.bWidths.full,
        .height = gd.bWidths.full
    };
    resLastSelected = GuiListViewEx(rectResList, resLabels, N_RESOLUTIONS, &resListViewFocus, &resListViewScrollIndex, resLastSelected);

    if(resLastSelected != -1) resLastAccepted = resLastSelected;
    else resLastSelected = resLastAccepted;
    
    // GuiSetState( (setFullscreen? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    // if( GuiButton((Rectangle){r.x + gd.bWidths.triq, r.y + r.height + gd.bHeights.full*2, gd.bWidths.full, gd.bHeights.full}, "Fullscreen") ) setFullscreen = true;
    // GuiSetState( GUI_STATE_NORMAL);

    // GuiSetState( (!setFullscreen? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    // if( GuiButton((Rectangle){r.x - gd.bWidths.triq, r.y + r.height + gd.bHeights.full*2, gd.bWidths.full, gd.bHeights.full}, "Windowed") ) setFullscreen = false;
    // GuiSetState( GUI_STATE_NORMAL);


    GuiSetState( (setFullscreen? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    Rectangle btnFullscreen = (Rectangle){
        .x = rectResList.x,
        .y = rectResList.y + rectResList.height + gd.bHeights.quart,
        .width = gd.bWidths.full,
        .height = gd.bHeights.full
    };
    if( GuiButton(btnFullscreen, (const char*)(setFullscreen? "Fullscreen: Yes" : "Fullscreen: No") ) ) setFullscreen = !setFullscreen;
    GuiSetState( GUI_STATE_NORMAL);


    finishScreen = GuiButton((Rectangle){120, screen.height.full - 120, gd.bWidths.full, gd.bHeights.full}, "Return");

    applyChanges = GuiButton((Rectangle){screen.width.full - 120 - gd.bWidths.full, screen.height.full - 120, gd.bWidths.full, gd.bHeights.full}, "Apply");


}

// Options Screen Unload logic
void UnloadOptionsScreen(void)
{
    // TODO: Unload OPTIONS screen variables here!
    finishScreen = 0;
}

// Options Screen should finish?
int FinishOptionsScreen(void)
{
    return finishScreen;
}


void UpdateResolution(int requestedScreenSizeX, int requestedScreenSizeY){

    int newWindowPosX = 0;
    int newWindowPosY = 0;

    newWindowPosX = GetMonitorWidth(0)/2 - requestedScreenSizeX/2;
    newWindowPosY = GetMonitorHeight(0)/2 - requestedScreenSizeY/2;

    // Set window size.
    SetWindowSize(requestedScreenSizeX, requestedScreenSizeY);

    // Center window on monitor.
    SetWindowPosition(newWindowPosX, newWindowPosY);
}