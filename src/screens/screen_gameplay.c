/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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

////////////////////////////////////////////
// FEATURES
//

// General: Info-screen / Tutorial
// General: "raylib" information space / Credits

//?? // Interaction: Delayed tooltips, CheckBox to display help/controls. UI completion with '?' buttons for revealing explanations/controls.
//x // General: Relocate the overlay-menu draw code to its own function (for cleanliness).
//x // Visuals: Text at mouse, "'R' to rotate", "Place 'Glider'", Use Show/Hide Checkbox for text.
//x // Interaction: Reset tool orientation when a new tool is selected.
//x // Interaction: Automatically close the toolbox list when a tool is selected.
//x // Interaction: Press 'R' or "Shift + R" to rotate the shape.
//x // Visuals: Minimalist GUI Overhaul
//x // Interaction: Show...: FPS, Neighbor #s
//x // Interaction: menu - show... - pause - clear grid - Toolbox...
//x // Visuals: View-toggled scroll list for toolbox
//x // Visuals: New Tool Ghosts
//x // General: Save resolution/fullscreen settings to file
//?? // General: Determine if grid tiles have become stable. Save last several live-tile-counts, use cycle-counting (tortoise/hair???) or something to determine if it repeats.
//?? // Visuals: Spotlight around cursor (definition; brighter upper-right, darker lower-left corners? something like that)
//?? // Visuals: Spotlight around active tiles.
//?? // Visuals: Light around border of grid.
//x // Options: Accessable from gameplay screen.
//x // Load resolution/fullscreen settings from file
//x // Completed options menu. Static list position, full-screen, judge startup size from monitor res.
//?? // Interaction: Click-hold to use tool repeated, but ONLY if cursor has moved beyond previous stamp's area. Rst on tool-change.
//?? // Interaction: Toolbox slide-in / slide-out with either TAB-pressed OR by clicking on a slight hint-bar left near the slide-away area.
//x // "Powered by Raylib" startup
//x // Options: Fullscreen
//x // "Intro" screen
//x // Interaction: Slow simulation speed (via framesCounter), pause/play
//x // Visuals: Spotlight around cursor
//x // Tools: Need to accomodate tools larger than 3x3
//x // Tools: Add "Example Patterns" http://pi.math.cornell.edu/~lipa/mec/lesson6.html
//x // General: Enumeration for tools/brushes.
//x // General: Grid/Tile Render Culling
//x // Interaction: Drag/pan with mouse.
//x // General: Reduce number of iterative loops for going through grid!
//x // Debug Overlay: FPS improvement when Debug overlay is visible. (Only render tile-state-text when zoomed-in??)(Mouseover+radius??)
//x // Debug Overlay: Show FPS. (Maybe option to always show/hide independently??)
//x // General: Living tiles count (average maybe?) in the corner.
//x // General: Dark tint behind on-screen text.
//x // Interaction: Auto-pause after "Clear Grid" button clicked.
//x // Interaction: Cap zoom level at max bounds of grid.
//x // Interaction: Zoom on mouse.
//x // Interaction: Split-apart "tap" int Left/Right/Middle/other mouse clicks. Currently all are "GESTURE_x"
//x // Interaction: Click-once should do as the tool indicates just once. (i.e. split from "hold" action)
//x // General: Relocate checks for Screen<--->World point-rectangle collisions from Draw() to Update() (if feasible).

//////////////////////////////
// BUGS

// When in 'STEP' simspeed, when Space is pressed the sim immediately steps once forward, but clicking "PAUSE" forces a duration-wait.

// Following minimalism overhaul, re-selecting the current toolbox tool does not deselect it.

//x // Camera target doesn't update with resolution change / GUI resize
//x // Camera frustrum doesn't update with GUI resize
//x // GUI sizing still doesn't update after gameplay->options->updateResolution->gameplay sequence. Font okay though.
//x // World coords of lower-right corner of cam frustum not being recalculated properly; constantly set to grid's absolute last corner.
//x // GUI sizing doesn't update (despite calling the function) after returning from Options screen.


#include <stdio.h>
#include <string.h>

#include "raylib.h"
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include "screens.h"
#include "gameplay_typedefs.h"
// #include "ricons.h"
#include "raymath.h"

#include "tool_defs.h"

#define RICON_GEAR_BIG 142
#define RICON_HELP 193

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Gameplay screen global variables

Vector2 viewScroll = { 0, 0 };


static const int cameraPanSpeedBase = 512;

static int framesCounter;
static int finishScreen;

#define FLOOR_GLOW_MAX 0.50f
#define FLOOR_GLOW_MIN 0.05f
float floorGlow;
static bool isGlowIncreasing;


static bool isEntireGridVisible;
static bool isScreenCenterInsideGrid;
static bool showMenuOverlay;
static bool showHelpOverlay;
static bool showExitConfirmation;
static bool userWantsClearGrid;


static SimulationCounter sc;

static Vector2 mousePosScreen;
static Vector2 mousePosScreen_last;
static Vector2 mousePosWorld;
static Vector2 mousePosWorld_last;

float cameraPanDelta;
static Camera2D camera; // Target == world-coords.


Frustum camFrustum;
    

static bool isMouseOnGui;


static bool guiShowNeighborNums;
static bool guiShowFps;
static bool guiShowAlive;

int guiCategorySelected;
int guiToolboxSelection;
int listViewExScrollIndex;
int guiSimsPerCycleSelected;

static int toolbox;

static int toolType;
static float toolFade;


static Grid sgMainGrid;
static Grid sgUpdates;

static Vector2 tileLastClicked;
static Vector2 tileLastHovered;
static bool isMouseOnGrid;


static bool showToolbox;
Vector2 panelScroll = { 99, -20 };

Rectangle rectProg;
Rectangle btnClearGridRect;
Rectangle btnPauseRect;
Rectangle btnOpenToolboxRect;
Rectangle btnMenuRect;
Rectangle btnHelpRect;
Rectangle rectSimSpeed;
Rectangle view;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    isMouseOnGui = true;
    
    guiShowNeighborNums = false;
    guiShowFps = false;
    guiShowAlive = false;

    guiCategorySelected = TCAT_SHIP;
    guiSimsPerCycleSelected = 0;

    toolbox = TB_GLIDER;

    showToolbox = false;

    toolFade = 0.5f;
    
    isEntireGridVisible = false;
    isScreenCenterInsideGrid = false;
    isMouseOnGui = false;
    showMenuOverlay = false;
    showHelpOverlay = false;
    showExitConfirmation = false;
    
    floorGlow = FLOOR_GLOW_MIN;
    isGlowIncreasing = true;

    mousePosScreen = GetMousePosition();
    mousePosScreen_last = mousePosScreen;
    mousePosWorld = GetScreenToWorld2D(mousePosScreen, camera);
    mousePosWorld_last = mousePosWorld;

    sc = (SimulationCounter){
        .framesPerCycle = 60, // Target FPS.
        .simsPerCycle = SPC_STEP,
        .frames = 0,
        .sims = 0,
        .paused = true
    };
    sc.sims = 1;
    sc.framesPerSim = sc.framesPerCycle / sc.simsPerCycle;


    sgMainGrid = (Grid){ 0 };
    sgMainGrid.tileSizePx = DFLT_TILE_SIZE_PX;
    sgMainGrid.numTiles = (Vector2){ DFLT_GRID_SIZE_X, DFLT_GRID_SIZE_Y };
    sgMainGrid.rect = (Rectangle){
        0,
        0,
        (sgMainGrid.numTiles.x*sgMainGrid.tileSizePx),
        (sgMainGrid.numTiles.y*sgMainGrid.tileSizePx)
    };
    sgMainGrid.posCenterPx = (Vector2){
        sgMainGrid.rect.x + (sgMainGrid.tileSizePx * sgMainGrid.numTiles.x/2),
        sgMainGrid.rect.y + (sgMainGrid.tileSizePx * sgMainGrid.numTiles.y/2)
    };
    sgMainGrid.active = 0;
    sgMainGrid.color = DARKGRAY;

    for(int i=0; i<sgMainGrid.numTiles.x; i++){
        for(int j=0; j<sgMainGrid.numTiles.y; j++){
            sgMainGrid.tiles[i][j].alive = false;
            sgMainGrid.tiles[i][j].neighbors = 0;
        }
    }

    sgUpdates = sgMainGrid;

    camera.target = sgMainGrid.posCenterPx;
    camera.offset = (Vector2){ screen.width.half, screen.height.half };
    camera.zoom = ZOOM_DFLT;

    camFrustum.ul = (Vector2){ -64, -64 };
    camFrustum.lr = (Vector2){ screen.width.full+64, screen.height.full+64 };

    tileLastHovered = (Vector2){sgMainGrid.numTiles.x/2, sgMainGrid.numTiles.y/2, };
    tileLastClicked = tileLastHovered;
    
} // end InitGameplayScreen()


// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{   


    rectProg = (Rectangle){
        .x = gd.margin*2,
        .y = screen.height.full - gd.margin*4,
        .width = screen.width.full - gd.margin*4,
        .height = gd.margin*1.5
    };

    btnClearGridRect = (Rectangle){
        .x = gd.margin*4,
        .y = rectProg.y - gd.bHeights.triq - gd.margin*2,
        .width = gd.bWidths.full,
        .height = gd.bHeights.triq,
    };

    btnPauseRect = (Rectangle){
        .x = screen.width.half - gd.bWidths.half - gd.margin,
        .y = rectProg.y - gd.bHeights.triq - gd.margin*2,
        .width = gd.bWidths.full,
        .height = gd.bHeights.triq,
    };

    btnMenuRect = (Rectangle){
        .x = screen.width.full - gd.bWidths.quart - gd.margin*2,
        .y = gd.margin*2,
        .width = gd.bWidths.quart,
        .height = gd.bWidths.quart
    };

    btnHelpRect = (Rectangle){
        .x = screen.width.full - gd.bWidths.quart*2 - gd.margin*4,
        .y = gd.margin*2,
        .width = gd.bWidths.quart,
        .height = gd.bWidths.quart
    };

    btnOpenToolboxRect = (Rectangle){
        .x = screen.width.full - gd.bWidths.full - gd.margin*4,
        .y = rectProg.y - gd.bHeights.triq - gd.margin*2,
        .width = gd.bWidths.full,
        .height = gd.bHeights.triq,
    };

    rectSimSpeed = (Rectangle){
        .x = screen.width.half + gd.bHeights.quart,
        .y = rectProg.y - gd.bHeights.triq - gd.margin*2,
        .width = gd.bWidths.half,
        .height = gd.bHeights.triq,
    };
    

    // THE WAGES OF SIN ARE BLOOD... AND BAD CODE...



    framesCounter = framesCounter>=60? 0 : framesCounter+1;

    if(!sc.paused){

        // Sims-per-Cycle will be modified by user. Recalculate.
        sc.framesPerSim = sc.framesPerCycle / sc.simsPerCycle;

        sc.frames++;
        if(sc.frames >= sc.framesPerSim) sc.frames = 0;

        sc.sims++;
        if(sc.sims >= sc.simsPerCycle) sc.sims = 0;
    }

    
    toolFade = 0.5f;//(float)(framesCounter/60);

    // Find Mouse
    mousePosScreen_last = mousePosScreen; //Vector2Distance(mousePosScreen_last, mousePosScreen) >= 0.1f ? mousePosScreen : mousePosScreen_last ;
    mousePosWorld_last = GetScreenToWorld2D(mousePosScreen_last, camera);

    mousePosScreen = GetMousePosition();
    mousePosWorld = GetScreenToWorld2D(mousePosScreen, camera);

    bool mouseBarTest = CheckCollisionPointRec(mousePosScreen, gd.rects.bar);
    bool mouseToolboxTest = CheckCollisionPointRec(mousePosScreen, view);
    bool mouseMenuHelpTest = CheckCollisionPointRec(mousePosScreen, (Rectangle){btnHelpRect.x, btnHelpRect.y, btnHelpRect.width + btnMenuRect.width + gd.margin*2, btnHelpRect.height});

    isMouseOnGui = showMenuOverlay | mouseBarTest | (showToolbox? mouseToolboxTest : false) | mouseMenuHelpTest;

    isMouseOnGrid = (isMouseOnGui? false : CheckCollisionPointRec(GetScreenToWorld2D(mousePosScreen, camera), sgMainGrid.rect));


    // Pause / Set Sim Speed
    if(IsKeyPressed(KEY_SPACE)){
        sc.paused = !sc.paused;
        // sc.simsPerCycle = SPC_STEP;
        // guiSimsPerCycleSelected = 0;
    }
    else if(IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_ONE)){
        sc.simsPerCycle = SPC_SLOW;
        guiSimsPerCycleSelected = 1;
        sc.paused = false;
    }
    else if(IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_TWO)){
        sc.simsPerCycle = SPC_MED;
        guiSimsPerCycleSelected = 2;
        sc.paused = false;
    }
    else if(IsKeyPressed(KEY_KP_3) || IsKeyPressed(KEY_THREE)){
        sc.simsPerCycle = SPC_FAST;
        guiSimsPerCycleSelected = 3;
        sc.paused = false;
    }
    else if(IsKeyPressed(KEY_F)){
        if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
            switch(sc.simsPerCycle){
                case SPC_STEP:
                    sc.simsPerCycle = SPC_FAST;
                    break;
                case SPC_SLOW:
                    sc.simsPerCycle = SPC_STEP;
                    break;
                case SPC_MED:
                    sc.simsPerCycle = SPC_SLOW;
                    break;
                case SPC_FAST:
                    sc.simsPerCycle = SPC_MED;
                    break;
                default:
                    break;
            }
            guiSimsPerCycleSelected--;
            if(guiSimsPerCycleSelected < 0) guiSimsPerCycleSelected = 3;
        }
        else{
            switch(sc.simsPerCycle){
                case SPC_STEP:
                    sc.simsPerCycle = SPC_SLOW;
                    break;
                case SPC_SLOW:
                    sc.simsPerCycle = SPC_MED;
                    break;
                case SPC_MED:
                    sc.simsPerCycle = SPC_FAST;
                    break;
                case SPC_FAST:
                    sc.simsPerCycle = SPC_STEP;
                    break;
                default:
                    break;
            }
            guiSimsPerCycleSelected++;
            if(guiSimsPerCycleSelected > 3) guiSimsPerCycleSelected = 0;
        }
    }
    

    // Tool Rotation
    if (IsKeyPressed(KEY_R) || IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
        toolType = (toolType + 1) % 4;
    }


    // Overlay Menu.
    if(IsKeyPressed(KEY_ESCAPE)){
        // Action for ESCAPE
        showMenuOverlay = !showMenuOverlay;
        showExitConfirmation = false;
    }


    // Camera.
    
    // Accounts for resolution changes.
    camera.offset = (Vector2){ screen.width.half, screen.height.half };
    camFrustum.ul = (Vector2){ -64, -64 };
    camFrustum.lr = (Vector2){ screen.width.full+64, screen.height.full+64 };


    if(IsKeyPressed(KEY_T)) {
        camera.zoom = ZOOM_DFLT;
        camera.target = sgMainGrid.posCenterPx;
    }


    int scrollMovement = GetMouseWheelMove();
    camera.zoom += (camera.zoom<=ZOOM_SENSITV_THRESHOLD? (scrollMovement*ZOOM_SENSITV_2) : (scrollMovement*ZOOM_SENSITV_1));
    
    // Camera zoom is clamped to min/max values.
    if (camera.zoom > ZOOM_MAX) camera.zoom = ZOOM_MAX;
    else if (camera.zoom < ZOOM_MIN) camera.zoom = ZOOM_MIN;

    // If camera zoom is caught in dead-zone between sensitivities it is snapped to the threshold line.
    if( (camera.zoom > ZOOM_SENSITV_THRESHOLD) && (camera.zoom <= (ZOOM_SENSITV_THRESHOLD+ZOOM_SENSITV_1-ZOOM_SENSITV_2)) ){
        camera.zoom = ZOOM_SENSITV_THRESHOLD+ZOOM_SENSITV_1;
    }
    
    
    if( IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ){

        Vector2 dragVector = Vector2Subtract(mousePosScreen_last, mousePosScreen);

        if(camera.zoom < 0.3f){
            dragVector = Vector2Scale(dragVector, 6.0f);
        }
        else if(camera.zoom < 0.4f){
            dragVector = Vector2Scale(dragVector, 3.0f);
        }
        else if(camera.zoom < 0.8f){
            dragVector = Vector2Scale(dragVector, 1.25f);
        }

        camera.target = Vector2Add(camera.target, dragVector);
    }
    else{
        cameraPanDelta = cameraPanSpeedBase * GetFrameTime();
        if(IsKeyDown(KEY_W)) camera.target.y -= cameraPanDelta;
        if(IsKeyDown(KEY_S)) camera.target.y += cameraPanDelta;
        if(IsKeyDown(KEY_A)) camera.target.x -= cameraPanDelta;
        if(IsKeyDown(KEY_D)) camera.target.x += cameraPanDelta;
    }
    

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        
        tileLastClicked = tileLastHovered;
        
        // If mouse not on grid, return.
        if( isMouseOnGrid && (toolbox != TB_NONE) ){

            if(toolbox == TB_SINGLE){
                sgUpdates.tiles[(int)tileLastClicked.x][(int)tileLastClicked.y].alive = !sgMainGrid.tiles[(int)tileLastClicked.x][(int)tileLastClicked.y].alive;
            }
            else if(IsGridEdgeTile((int)tileLastClicked.x, (int)tileLastClicked.y, &sgMainGrid) == false){

                ToolProps* tpSwitch;
                switch(toolbox){
                    case TB_GLIDER:
                        tpSwitch = &tpGlider;
                        break;
                    case TB_LWSHIP:
                        tpSwitch = &tpLwSpaceship;
                        break;
                    case TB_MWSHIP:
                        tpSwitch = &tpMwSpaceship;
                        break;
                    case TB_HWSHIP:
                        tpSwitch = &tpHwSpaceship;
                        break;

                    case TB_BLOCK:
                        tpSwitch = &tpBlock;
                        break;
                    case TB_BEEHIVE:
                        tpSwitch = &tpBeehive;
                        break;
                    case TB_LOAF:
                        tpSwitch = &tpLoaf;
                        break;
                    case TB_BOAT:
                        tpSwitch = &tpBoat;
                        break;
                    case TB_TUB:
                        tpSwitch = &tpTub;
                        break;

                    case TB_BLINKER:
                        tpSwitch = &tpBlinker;
                        break;
                    case TB_TOAD:
                        tpSwitch = &tpToad;
                        break;
                    case TB_BEACON:
                        tpSwitch = &tpBeacon;
                        break;
                    case TB_PULSAR:
                        tpSwitch = &tpPulsar;
                        break;
                    case TB_PENTADECATHLON:
                        tpSwitch = &tpPentaDecathlon;
                        break;

                    case TB_GOSPER:
                        tpSwitch = &tpGosper;
                        break;
                    case TB_RIPCONWAY:
                        tpSwitch = &tpRipConway;
                        break;

                    default:
                        // Default to Glider
                        tpSwitch = &tpGlider;
                        break;
                } // switch

                UseTool(tileLastClicked, tpSwitch, toolType, &sgUpdates);
            }
        } // Click
    }

    

    /********************************************************************************************************************************/

    // Process Automata Rules
    if( !sc.paused ){
        if(sc.frames == 0){

            if(sc.simsPerCycle == SPC_STEP) sc.paused = true;

            for(int i=0; i<sgMainGrid.numTiles.x; i++){
                for(int j=0; j<sgMainGrid.numTiles.y; j++){

                    // Judge life/death based on neighbors/current-state
                    if(sgMainGrid.tiles[i][j].alive == true){
                        // ALIVE
                        // If less than two neighbors, die. Underpopulation.
                        if(sgMainGrid.tiles[i][j].neighbors < 2) sgUpdates.tiles[i][j].alive = false;
                        // If more than three neighbors, die. Overpopulation.
                        if(sgMainGrid.tiles[i][j].neighbors > 3) sgUpdates.tiles[i][j].alive = false;
                    }
                    else{
                        // DEAD
                        // If exactly three neighbors, live. Reproduction.
                        if(sgMainGrid.tiles[i][j].neighbors == 3) sgUpdates.tiles[i][j].alive = true;
                    }
                }
            }
        }
    }

    // Sync Grids
    sgMainGrid.active = 0;
    if(userWantsClearGrid){
        userWantsClearGrid = false;
        memset( sgMainGrid.tiles, 0, sizeof(Tile) * sgMainGrid.numTiles.x * sgMainGrid.numTiles.y);
        memset( sgUpdates.tiles, 0, sizeof(Tile) * sgUpdates.numTiles.x * sgUpdates.numTiles.y);
        sgMainGrid.active = 0;
    }
    else{
        for(int i=0; i<sgMainGrid.numTiles.x; i++){
            for(int j=0; j<sgMainGrid.numTiles.y; j++){

                // Propagate Updates to MainGrid
                sgMainGrid.tiles[i][j].alive = sgUpdates.tiles[i][j].alive;
                sgMainGrid.tiles[i][j].neighbors = GetNeighborsEx(i, j, &sgUpdates);
                sgMainGrid.active = (sgMainGrid.tiles[i][j].alive == true ? sgMainGrid.active+1 : sgMainGrid.active);

                // Check for the last tile that the mouse hovered-over.
                if(isMouseOnGrid){
                    // Only update if the current tile does not match the recorded tile location.
                    if( (tileLastHovered.x != i) || (tileLastHovered.y != j) ){
                        Rectangle tile = (Rectangle){
                                            (sgMainGrid.rect.x + (i * sgMainGrid.tileSizePx)),
                                            (sgMainGrid.rect.y + (j * sgMainGrid.tileSizePx)),
                                            sgMainGrid.tileSizePx,
                                            sgMainGrid.tileSizePx
                                        };
                        if(CheckCollisionPointRec(mousePosWorld, tile)){
                            tileLastHovered = (Vector2){i, j};
                        }
                    }
                }
                else{
                    // Default the location to the center of the grid if the mouse has moved-away.
                    // TODO be smarter about this; the goal was for tools "ghosts" to remain in the grid and visible.
                    tileLastHovered = (Vector2){sgMainGrid.numTiles.x/2, sgMainGrid.numTiles.y/2};
                }
                
            } // for j
        } // for i
    }
    

    
} // end UpdateGameplayScreen()


// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // Draw background
    DrawRectangle(0, 0, screen.width.full, screen.height.full, BLACK);

    BeginMode2D(camera);

        // Get the world-coordinate translation of the camera frustum.


        Frustum camFrustumWorld = (Frustum){ GetScreenToWorld2D(camFrustum.ul,camera), GetScreenToWorld2D(camFrustum.lr,camera) };

        Frustum camFrustumTiles;

        camFrustumTiles.ul.x = (int)(camFrustumWorld.ul.x / sgMainGrid.tileSizePx);
        camFrustumTiles.ul.y = (int)(camFrustumWorld.ul.y / sgMainGrid.tileSizePx);
        camFrustumTiles.lr.x = (int)(camFrustumWorld.lr.x / sgMainGrid.tileSizePx);
        camFrustumTiles.lr.y = (int)(camFrustumWorld.lr.y / sgMainGrid.tileSizePx);

        // Clamp values to grid boundaries.
        camFrustumTiles.ul.x = (camFrustumTiles.ul.x < 0)? 0 : camFrustumTiles.ul.x;
        camFrustumTiles.ul.y = (camFrustumTiles.ul.y < 0)? 0 : camFrustumTiles.ul.y;
        camFrustumTiles.lr.x = (camFrustumTiles.lr.x > sgMainGrid.numTiles.x)? sgMainGrid.numTiles.x : camFrustumTiles.lr.x;
        camFrustumTiles.lr.y = (camFrustumTiles.lr.y > sgMainGrid.numTiles.y)? sgMainGrid.numTiles.y : camFrustumTiles.lr.y;

        for(int i=camFrustumTiles.ul.x; i<camFrustumTiles.lr.x; i++){
            for(int j=camFrustumTiles.ul.y; j<camFrustumTiles.lr.y; j++){

                Vector2 gridTileCoords = (Vector2){
                    sgMainGrid.rect.x + (i * sgMainGrid.tileSizePx),
                    sgMainGrid.rect.y + (j * sgMainGrid.tileSizePx)
                };

                Rectangle gridTileRect = (Rectangle){
                    .x = gridTileCoords.x,
                    .y = gridTileCoords.y,
                    .width = sgMainGrid.tileSizePx,
                    .height = sgMainGrid.tileSizePx
                };
                
                
                // Draw highlight on tiles around cursor
                if(isMouseOnGrid){
                    if(CheckCollisionCircleRec(mousePosWorld, sgMainGrid.tileSizePx*CURSOR_GLOW_RADIUS, gridTileRect)){

                        int fadeFactor = 0;
                        int colorFactor = 0;
                        Color baseColor = BEIGE;
                        Color lineColor = BLANK;

                        for(int k=1; k<=CURSOR_GLOW_RADIUS; k++){
                            if(CheckCollisionCircleRec(mousePosWorld, k*sgMainGrid.tileSizePx, gridTileRect)){
                                // fadeFactor = k + GetRandomValue(-0.25f, 0.25f);
                                fadeFactor = k + 0.25 * GetRandomValue(-1, 1);
                                colorFactor = k * 100 / CURSOR_GLOW_RADIUS;
                                break;
                            }
                        }

                        lineColor.r = (baseColor.r - colorFactor) < 0.0f ? 0.0f : (baseColor.r - colorFactor);
                        lineColor.g = (baseColor.g - colorFactor) < 0.0f ? 0.0f : (baseColor.g - colorFactor);
                        lineColor.b = (baseColor.b - colorFactor) < 0.0f ? 0.0f : (baseColor.b - colorFactor);

                        DrawRectangleLinesEx( gridTileRect, 1, Fade(lineColor, 1.00f - (0.1f * fadeFactor)) );
                    }
                }

                // Draw active tiles in view
                if(sgMainGrid.tiles[i][j].alive == true){
                    DrawRectangle(gridTileCoords.x, gridTileCoords.y, sgMainGrid.tileSizePx, sgMainGrid.tileSizePx, WHITE);
                }

                // if debug, show centered true/false text
                if(guiShowNeighborNums && (sgMainGrid.tiles[i][j].neighbors != 0) && (camera.zoom > 0.45f)){
                    Color debugTextColor = (sgMainGrid.tiles[i][j].alive == true ? LIME : RED);
                    DrawText(TextFormat("%d", sgMainGrid.tiles[i][j].neighbors), gridTileCoords.x+8, gridTileCoords.y+8, 20, debugTextColor);
                } // if debug
            } // for j
        } // for i


        // Grid Border
        DrawRectangleLinesEx((Rectangle){-4, -4, sgMainGrid.numTiles.x*sgMainGrid.tileSizePx+4, sgMainGrid.numTiles.y*sgMainGrid.tileSizePx+4}, (camera.zoom < 0.5f ? 12 : 8), GOLD);


        // Render tool ghost
        if(isMouseOnGrid){
            if(toolbox == TB_SINGLE){
                Rectangle r = (Rectangle){
                    tileLastHovered.x * sgMainGrid.tileSizePx,
                    tileLastHovered.y * sgMainGrid.tileSizePx,
                    sgMainGrid.tileSizePx,
                    sgMainGrid.tileSizePx
                };
                DrawRectangleRec(r, Fade(SKYBLUE, toolFade) );
            }
            else if(IsGridEdgeTile((int)tileLastHovered.x, (int)tileLastHovered.y, &sgMainGrid) == false){
                ToolProps* tpSwitch;
                switch(toolbox){
                    case TB_GLIDER:
                        tpSwitch = &tpGlider;
                        break;
                    case TB_LWSHIP:
                        tpSwitch = &tpLwSpaceship;
                        break;
                    case TB_MWSHIP:
                        tpSwitch = &tpMwSpaceship;
                        break;
                    case TB_HWSHIP:
                        tpSwitch = &tpHwSpaceship;
                        break;

                    case TB_BLOCK:
                        tpSwitch = &tpBlock;
                        break;
                    case TB_BEEHIVE:
                        tpSwitch = &tpBeehive;
                        break;
                    case TB_LOAF:
                        tpSwitch = &tpLoaf;
                        break;
                    case TB_BOAT:
                        tpSwitch = &tpBoat;
                        break;
                    case TB_TUB:
                        tpSwitch = &tpTub;
                        break;

                    case TB_BLINKER:
                        tpSwitch = &tpBlinker;
                        break;
                    case TB_TOAD:
                        tpSwitch = &tpToad;
                        break;
                    case TB_BEACON:
                        tpSwitch = &tpBeacon;
                        break;
                    case TB_PULSAR:
                        tpSwitch = &tpPulsar;
                        break;
                    case TB_PENTADECATHLON:
                        tpSwitch = &tpPentaDecathlon;
                        break;

                    case TB_GOSPER:
                        tpSwitch = &tpGosper;
                        break;
                    case TB_RIPCONWAY:
                        tpSwitch = &tpRipConway;
                        break;

                    default:
                        // Default to Glider
                        tpSwitch = &tpGlider;
                        break;
                } // switch

                DrawTool(tileLastHovered, tpSwitch, toolType, &sgUpdates, Fade(SKYBLUE, toolFade));
            }
        } // ismouseOnGrid

    EndMode2D();



    ////////////////////////////////////////////////////////////////
    // Render Graphical User Interface

    // Background
    DrawRectangleRec(gd.rects.bar, Fade(DARKGRAY, 0.4f));

    // "Simulation Paused" Indication Border
    if( sc.paused ) DrawRectangleRoundedLines( (Rectangle){0+8, 0+8, screen.width.full-16, screen.height.full-16}, 0.0f, 10, 8, Fade(RED, 0.5f) );

    // Simulation Progress Feedback Bar
    GuiSetState( GUI_STATE_FOCUSED );
    GuiProgressBar(rectProg, NULL, NULL, (sc.frames==0? (float)sc.framesPerSim : (float)sc.frames ), 0.1, (float)sc.framesPerSim);
    GuiSetState( GUI_STATE_NORMAL );

    // Clearn-Grid Button
    if( GuiButton(btnClearGridRect, "Clear Grid")) userWantsClearGrid = true;


    // Checkboxes.
    Rectangle rectCheckbox = (Rectangle){
        .x = screen.width.quart - gd.bWidths.full/2,
        .y = rectProg.y - gd.bHeights.triq - gd.margin*2,
        .width = gd.bHeights.quart,
        .height = gd.bHeights.quart,
    };
    guiShowNeighborNums = GuiCheckBox(rectCheckbox, "Show Neighbor Numbers", guiShowNeighborNums);
    rectCheckbox.y += rectCheckbox.height + gd.margin;
    guiShowFps = GuiCheckBox(rectCheckbox, "Show Frames Per Second", guiShowFps);
    rectCheckbox.y += rectCheckbox.height + gd.margin;
    guiShowAlive = GuiCheckBox(rectCheckbox, "Show Live Tiles Count", guiShowAlive);


    // Pause Button
    GuiSetState( (sc.paused? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton( btnPauseRect, "Pause" ) ){
        sc.paused = !sc.paused;
    }
    GuiSetState( GUI_STATE_NORMAL );

    // Menu Overlay Button
    if(!showMenuOverlay)
        if( GuiButton(btnMenuRect, GuiIconText(RICON_GEAR_BIG, NULL)) ){
            showMenuOverlay = true;
            showHelpOverlay = false;
        }

    // Help Overlay Button
    if (GuiButton(btnHelpRect, GuiIconText(RICON_HELP, NULL)) ) showHelpOverlay = !showHelpOverlay;

    // Show help (controls, etc)
    if(showHelpOverlay) DrawHelpOverlay();

    // Simulation Speed Buttons
    rectSimSpeed.x += rectSimSpeed.width + gd.margin;
    GuiSetState( (sc.simsPerCycle == SPC_STEP ? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton(rectSimSpeed, "Step") ){
        guiSimsPerCycleSelected = 0;
        sc.simsPerCycle = SPC_STEP;
    }
    GuiSetState( GUI_STATE_NORMAL );

    rectSimSpeed.x += rectSimSpeed.width + gd.margin;
    GuiSetState( (sc.simsPerCycle == SPC_SLOW ? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton(rectSimSpeed, "1") ){
        guiSimsPerCycleSelected = 1;
        sc.simsPerCycle = SPC_SLOW;
    }
    GuiSetState( GUI_STATE_NORMAL );

    rectSimSpeed.x += rectSimSpeed.width + gd.margin;
    GuiSetState( (sc.simsPerCycle == SPC_MED ? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton(rectSimSpeed, "2") ){
        guiSimsPerCycleSelected = 2;
        sc.simsPerCycle = SPC_MED;
    }
    GuiSetState( GUI_STATE_NORMAL );

    rectSimSpeed.x += rectSimSpeed.width + gd.margin;
    GuiSetState( (sc.simsPerCycle == SPC_FAST ? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton(rectSimSpeed, "3") ){
        guiSimsPerCycleSelected = 3;
        sc.simsPerCycle = SPC_FAST;
    }
    GuiSetState( GUI_STATE_NORMAL );


    // Show FPS
    if(guiShowFps) DrawFPS(20, 20);


    // Show count of alive tiles.
    if(guiShowAlive) DrawText(TextFormat("%d Alive", sgMainGrid.active), 20, 40, 20, GREEN);


    // Toolbox Button
    GuiSetState( (showToolbox? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
    if( GuiButton(btnOpenToolboxRect, (const char*)(showToolbox? "Close..." : "Open Toolbox...")) ) showToolbox = !showToolbox;
    GuiSetState( GUI_STATE_NORMAL );


    // Toolbox Content (if visible)
    if(showToolbox && isMouseOnGui){
        
        int totalContentHeight = gd.bHeights.half * (N_CATEGORIES + N_CAT_STILLS + N_CAT_OSCILLATORS + N_CAT_SHIPS + N_CAT_OTHER) + gd.margin;
        Rectangle panelContentRec = {0, 0, btnOpenToolboxRect.width, totalContentHeight };

        Rectangle panelRec =  (Rectangle){
            .x = 0,
            .y = 0,
            .width = panelContentRec.width + gd.margin*12,
            .height = ( (panelContentRec.height + gd.margin*2) > screen.height.triq ? screen.height.triq : (panelContentRec.height + gd.margin*2) )
        };

        panelRec.x = btnOpenToolboxRect.x + btnOpenToolboxRect.width - panelRec.width; // Right-side-aligned with Toolbox button.
        panelRec.y = btnOpenToolboxRect.y - panelRec.height;//btnOpenToolboxRect.y - panelRec.height - gd.bHeights.quart/2;

        GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 20);
        view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);

        BeginScissorMode(view.x, view.y, view.width, view.height);
            Rectangle r = btnOpenToolboxRect;

            r.x = panelRec.x + gd.margin*2 + panelScroll.x;
            r.y = panelRec.y + panelScroll.y;
            r.width -= gd.margin*2;
            r.height = gd.bHeights.half;

            GuiLabel(r, guiLabels_categories[0]);
            for(int i=0; i<N_CAT_STILLS; i++){
                r.y += r.height;
                GuiSetState( (toolbox == TOS_STILL + i? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
                if( GuiButton(r, guiLabels_stills[i]) ){
                    toolbox = TOS_STILL + i;
                    toolType = 0;
                }
                GuiSetState( GUI_STATE_NORMAL );
            }

            r.y += r.height;
            GuiLabel(r, guiLabels_categories[1]);
            for(int i=0; i<N_CAT_OSCILLATORS; i++){
                r.y += r.height;
                GuiSetState( (toolbox == TOS_OSCILLATOR + i? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
                if( GuiButton(r, guiLabels_oscillators[i]) ){
                    toolbox = TOS_OSCILLATOR + i;
                    toolType = 0;
                }
                GuiSetState( GUI_STATE_NORMAL );
            }

            r.y += r.height;
            GuiLabel(r, guiLabels_categories[2]);
            for(int i=0; i<N_CAT_SHIPS; i++){
                r.y += r.height;
                GuiSetState( (toolbox == TOS_SHIP + i? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
                if( GuiButton(r, guiLabels_ships[i]) ){
                    toolbox = TOS_SHIP + i;
                    toolType = 0;
                }
                GuiSetState( GUI_STATE_NORMAL );
            }

            r.y += r.height;
            GuiLabel(r, guiLabels_categories[3]);
            for(int i=0; i<N_CAT_OTHER; i++){
                r.y += r.height;
                GuiSetState( (toolbox == TOS_OTHER + i? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );
                if( GuiButton(r, guiLabels_other[i]) ){
                    toolbox = TOS_OTHER + i;
                    toolType = 0;
                }
                GuiSetState( GUI_STATE_NORMAL );
            }
        EndScissorMode();


        // Draw preview of selected tool

        ToolProps* tpSwitch = NULL;
        if(toolbox == TB_SINGLE){
            tpSwitch = &tpSingle;
        }
        else if(IsGridEdgeTile((int)tileLastHovered.x, (int)tileLastHovered.y, &sgMainGrid) == false){
            
            switch(toolbox){
                case TB_GLIDER:
                    tpSwitch = &tpGlider;
                    break;
                case TB_LWSHIP:
                    tpSwitch = &tpLwSpaceship;
                    break;
                case TB_MWSHIP:
                    tpSwitch = &tpMwSpaceship;
                    break;
                case TB_HWSHIP:
                    tpSwitch = &tpHwSpaceship;
                    break;

                case TB_BLOCK:
                    tpSwitch = &tpBlock;
                    break;
                case TB_BEEHIVE:
                    tpSwitch = &tpBeehive;
                    break;
                case TB_LOAF:
                    tpSwitch = &tpLoaf;
                    break;
                case TB_BOAT:
                    tpSwitch = &tpBoat;
                    break;
                case TB_TUB:
                    tpSwitch = &tpTub;
                    break;

                case TB_BLINKER:
                    tpSwitch = &tpBlinker;
                    break;
                case TB_TOAD:
                    tpSwitch = &tpToad;
                    break;
                case TB_BEACON:
                    tpSwitch = &tpBeacon;
                    break;
                case TB_PULSAR:
                    tpSwitch = &tpPulsar;
                    break;
                case TB_PENTADECATHLON:
                    tpSwitch = &tpPentaDecathlon;
                    break;

                case TB_GOSPER:
                    tpSwitch = &tpGosper;
                    break;
                
                case TB_RIPCONWAY:
                    tpSwitch = &tpRipConway;
                    break;

                default:
                    // Default to Glider
                    tpSwitch = &tpGlider;
                    break;
            } // switch
            
        }

        int tileSizePx = 8; // pixels

        Rectangle rectToolPreview = (Rectangle){
            .x = panelRec.x,
            .y = panelRec.y - (2 + tpSwitch->height)*tileSizePx,
            .width = panelRec.width,
            .height = (2 + tpSwitch->height)*tileSizePx
        };

        // Button is used for visual theme consistency; better than a solid-color rectangle.
        GuiButton(rectToolPreview, "");
        GuiSetState(GUI_STATE_NORMAL);

        int toolPreviewPosX = rectToolPreview.x + (rectToolPreview.width/2) - (tpSwitch->width * tileSizePx / 2);
        DrawToolPreview( toolPreviewPosX, rectToolPreview.y + tileSizePx, tileSizePx, tpSwitch, 0, SKYBLUE );
    }
    else{
        // Hide toolbox pane if user moves mouse off of it.
        showToolbox = false;
    }

    // Show menu for returning to main screen, or changing the resolution.
    if(showMenuOverlay) DrawMenuOverlay();

} // end DrawGameplayScreen
                                                  

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void){
    // TODO: Unload GAMEPLAY screen variables here!
    finishScreen = 0;
}


// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}


void DrawMenuOverlay(){
    sc.paused = true;
    
    showExitConfirmation == true? GuiDisable() : GuiEnable();
    Rectangle rectMenuOverlay = (Rectangle){ screen.width.quart*2 - gd.bWidths.full/2 -20, screen.height.quart*1.5 + 20, gd.bWidths.full + 40, (gd.bHeights.full*3)+40+20+(5*(3-1)) }; // Three buttons, stacked vertically, 20px from borders of overlay all sides, 5px between each, +20px for label.
    DrawRectangle(0, 0, screen.width.full, screen.height.full, Fade(BLACK, 0.4f));
    DrawRectangleRec(rectMenuOverlay, Fade(BLACK, 0.4f));
    DrawText("Menu", rectMenuOverlay.x + (rectMenuOverlay.width / 2) - (MeasureText("Menu", 20) / 2), rectMenuOverlay.y + 20, 20, GRAY);
    
    // showExitConfirmation == true? 0 : GuiSetTooltip("Close this window.");
    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40, gd.bWidths.full, gd.bHeights.full }, "Return")) showMenuOverlay = false;
    // GuiClearTooltip();

    // showExitConfirmation == true? 0 : GuiSetTooltip("Update resolution.");
    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40 + 5 + gd.bHeights.full*1, gd.bWidths.full, gd.bHeights.full }, "Options")) finishScreen = 2;
    // GuiClearTooltip();

    // showExitConfirmation == true? 0 : GuiSetTooltip("Exit, and return to title screen.");
    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40 + (5*2) + gd.bHeights.full*2, gd.bWidths.full, gd.bHeights.full }, "Exit to Menu")) showExitConfirmation = true;
    // GuiClearTooltip();
    showExitConfirmation == true? GuiEnable() : GuiDisable();

    if(showExitConfirmation){
        const char* confirmText = "Are you sure?";
        DrawRectangle(0, 0, screen.width.full, screen.height.full, Fade(BLACK, 0.8f));
        DrawText(confirmText, screen.width.quart*2 - MeasureText(confirmText, 40)/2 , screen.height.quart*2 - gd.bHeights.full*2, 40, GRAY);
        if (GuiButton((Rectangle){ screen.width.quart*2 - 10 - gd.bWidths.half, screen.height.half - gd.bHeights.half, gd.bWidths.half, gd.bHeights.full }, "Yes")) finishScreen = 1;
        if (GuiButton((Rectangle){ screen.width.quart*2 + 10, screen.height.quart*2 - gd.bHeights.half, gd.bWidths.half, gd.bHeights.full }, "No")) showExitConfirmation = false;
    }
} // end DrawMenuOverlay()


void DrawHelpOverlay(){

    char* helpText[4] = {
        "Sets all \"alive\" tiles to dead.",
        "Neighbor Numbers indicate adjacent \"alive\" tiles.",
        "Pause the simulation.",
        "Step = One Simulation, then Pause.    1/2/3 = Slow/Med./Fast"
    };

    char* controlText[13] = {
        "Shortcuts:",
        "R, MMB ... Rotate tool shape.",
        "LMB ...... Use Tool at Cursor.",
        "W, S ..... Pan Up/Down.",
        "A, D ..... Pan Left/Right.",
        "RMB ...... Drag to Pan.",
        "MWheel ... Zoom In/Out.",
        "T ........ Reset Camera",
        "SPACE .... Pause (or step once if 'Step').",
        "F ........ Cycle Speed.",
        "1 ........ Slow Speed.",
        "2 ........ Medium Speed.",
        "3 ........ Fast Speed."
    };

    int posYhelp = gd.rects.bar.y - gd.bHeights.quart;
    int helpSize = 16;
    Color helpColor = GOLD;    

    DrawText(helpText[0], btnClearGridRect.x, posYhelp, helpSize, helpColor);
    DrawText(helpText[1], screen.width.quart - gd.bWidths.full/2, posYhelp, helpSize, helpColor);
    DrawText(helpText[2], btnPauseRect.x, posYhelp, helpSize, helpColor);
    DrawText(helpText[3], btnPauseRect.x + btnPauseRect.width + rectSimSpeed.width, posYhelp, helpSize, helpColor);

    int posYcontrol = 80;
    int controlSize = 20;
    Color controlColor = GOLD;

    for(int i=0; i<13; i++){
        DrawText(controlText[i], 20, posYcontrol + i*(controlSize+gd.margin), controlSize, controlColor);
    }

    // DrawText("Change Placement Tool.", btnOpenToolboxRect.x, btnOpenToolboxRect.y + tool);
} // end DrawHelpOverlay()