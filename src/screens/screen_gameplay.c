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


// TODO Randomize grid state

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

static bool userWantsGridRandomState;

static SimulationCounter sc;

static Vector2 mousePosScreen;
static Vector2 mousePosScreen_last;
static Vector2 mousePosWorld;
static Vector2 mousePosWorld_last;

float cameraPanDelta;
static Camera2D camera; // Target == world-coords.
static bool refreshCamera;

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

static Grid grid;

static TileCoord tileLastHovered;
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

Rectangle btnGridRandomRect;

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
    guiShowFps = true;
    guiShowAlive = true;

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


    grid.size.x = DEFAULT_TILE_COUNT;
    grid.size.y = DEFAULT_TILE_COUNT;
    grid.num_alive = 0;
    grid.gfx.tile_size_px = DEFAULT_TILE_SIZE_PX;
    grid.gfx.rect.width = grid.gfx.tile_size_px * grid.size.x;
    grid.gfx.rect.height = grid.gfx.tile_size_px * grid.size.y;
    grid.gfx.center_px.x = grid.gfx.rect.width / 2;
    grid.gfx.center_px.y = grid.gfx.rect.height / 2;


    refreshCamera = true;

    camera.target = grid.gfx.center_px;
    camera.offset = (Vector2){ screen.width.half, screen.height.half };
    camera.zoom = ZOOM_DFLT;

    camFrustum.ul = (Vector2){ -64, -64 };
    camFrustum.lr = (Vector2){ screen.width.full+64, screen.height.full+64 };

    tileLastHovered = (TileCoord){ 0, 0 };

    
} // end InitGameplayScreen()


// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{   


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

    isMouseOnGrid = (isMouseOnGui? false : CheckCollisionPointRec(mousePosWorld, grid.gfx.rect));

    tileLastHovered = GetTileLastHovered(mousePosWorld, &grid);

    // Accounts for resolution changes.
    if(refreshCamera){
        refreshCamera = false;
        camera.offset = (Vector2){ screen.width.half, screen.height.half };
        camFrustum.ul = (Vector2){ -64, -64 };
        camFrustum.lr = (Vector2){ screen.width.full+64, screen.height.full+64 };

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

        btnGridRandomRect = (Rectangle){
            .x = screen.width.full - gd.bWidths.quart - gd.margin*2,
            .y = gd.margin*4 + gd.bWidths.quart,
            .width = gd.bWidths.quart,
            .height = gd.bWidths.quart
        };
    }

    

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
        if((toolbox != TB_NONE) && !showMenuOverlay){
            toolbox = TB_NONE;
        }
        else{
            showMenuOverlay = !showMenuOverlay;
            showExitConfirmation = false;
        }
        
    }


    // Camera.
    
    if(IsKeyPressed(KEY_T)) {
        camera.zoom = ZOOM_DFLT;
        camera.target = grid.gfx.center_px;
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
   

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        
        // If mouse not on grid, return.
        if( isMouseOnGrid && (toolbox != TB_NONE) ){


            if(toolbox == TB_SINGLE){
                grid.updates[tileLastHovered.x][tileLastHovered.y] = !grid.tiles[tileLastHovered.x][tileLastHovered.y].alive;
            }
            else if(IsGridEdgeTile(tileLastHovered.x, tileLastHovered.y, &grid) == false){

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

                UseTool(tileLastHovered, tpSwitch, toolType, &grid);
            }
        } // Click
    }

    

    /********************************************************************************************************************************/

    // Process Automata Rules
    if( !sc.paused ){
        if(sc.frames == 0){

            if(sc.simsPerCycle == SPC_STEP) sc.paused = true;

            for(int i=0; i<grid.size.x; i++){
                for(int j=0; j<grid.size.y; j++){

                    // Judge life/death based on neighbors/current-state
                    if(grid.tiles[i][j].alive == true){
                        // ALIVE
                        // If less than two neighbors, die. Underpopulation.
                        if(grid.tiles[i][j].neighbors < 2) grid.updates[i][j] = false;
                        // If more than three neighbors, die. Overpopulation.
                        if(grid.tiles[i][j].neighbors > 3) grid.updates[i][j] = false;
                    }
                    else{
                        // DEAD
                        // If exactly three neighbors, live. Reproduction.
                        if(grid.tiles[i][j].neighbors == 3) grid.updates[i][j] = true;
                    }
                }
            }
        }
    }

    // Sync Grids
    grid.num_alive = 0;
    if(userWantsClearGrid){
        userWantsClearGrid = false;
        memset( grid.tiles, 0, sizeof(Tile) * grid.size.x * grid.size.y);
        memset( grid.updates, 0, sizeof(bool) * grid.size.x * grid.size.y);
    }
    else if(userWantsGridRandomState){
        userWantsGridRandomState = false;
        // TODO initialize the entire grid to a random state.
        memset( grid.tiles, 0, sizeof(Tile) * grid.size.x * grid.size.y);
        memset( grid.updates, 0, sizeof(bool) * grid.size.x * grid.size.y);

        UseTool((TileCoord){grid.size.x/2, grid.size.y/2}, &tpBlinker, toolType, &grid);
    }
    else{
        for(int i=0; i<grid.size.x; i++){
            for(int j=0; j<grid.size.y; j++){

                grid.tiles[i][j].alive = grid.updates[i][j];
                grid.tiles[i][j].neighbors = GetNeighborsEx(i, j, &grid);
                if(grid.updates[i][j]){
                    grid.num_alive++;
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


        Frustum camFrustumWorld = (Frustum){
                                    GetScreenToWorld2D(camFrustum.ul,camera),
                                    GetScreenToWorld2D(camFrustum.lr,camera)
                                };

        Frustum camFrustumTiles;

        camFrustumTiles.ul.x = (int)(camFrustumWorld.ul.x / grid.gfx.tile_size_px);
        camFrustumTiles.ul.y = (int)(camFrustumWorld.ul.y / grid.gfx.tile_size_px);
        camFrustumTiles.lr.x = (int)(camFrustumWorld.lr.x / grid.gfx.tile_size_px);
        camFrustumTiles.lr.y = (int)(camFrustumWorld.lr.y / grid.gfx.tile_size_px);

        // Clamp values to grid boundaries.
        camFrustumTiles.ul.x = (camFrustumTiles.ul.x < 0)? 0 : camFrustumTiles.ul.x;
        camFrustumTiles.ul.y = (camFrustumTiles.ul.y < 0)? 0 : camFrustumTiles.ul.y;
        camFrustumTiles.lr.x = (camFrustumTiles.lr.x > grid.size.x)? grid.size.x : camFrustumTiles.lr.x;
        camFrustumTiles.lr.y = (camFrustumTiles.lr.y > grid.size.y)? grid.size.y : camFrustumTiles.lr.y;



        // Draw Tiles.

        Vector2 gridTileCoords = (Vector2){
            grid.gfx.rect.x + (camFrustumTiles.ul.x * grid.gfx.tile_size_px),
            grid.gfx.rect.y + (camFrustumTiles.ul.y * grid.gfx.tile_size_px)
        };
        
        int row_pixel_increment;
        int column_pixel_increment;

        for(int i=camFrustumTiles.ul.x; i<camFrustumTiles.lr.x; i++){
                
            row_pixel_increment = i * grid.gfx.tile_size_px;

            for(int j=camFrustumTiles.ul.y; j<camFrustumTiles.lr.y; j++){

                column_pixel_increment = j * grid.gfx.tile_size_px;

                gridTileCoords.x = grid.gfx.rect.x + row_pixel_increment;
                gridTileCoords.y = grid.gfx.rect.y + column_pixel_increment;

                if(grid.tiles[i][j].alive == true){

                    Vector2 ul = (Vector2){ gridTileCoords.x, gridTileCoords.y };
                    Vector2 ur = (Vector2){ gridTileCoords.x + grid.gfx.tile_size_px, gridTileCoords.y };
                    Vector2 ll = (Vector2){ gridTileCoords.x, gridTileCoords.y + grid.gfx.tile_size_px };
                    Vector2 lr = (Vector2){ gridTileCoords.x + grid.gfx.tile_size_px, gridTileCoords.y + grid.gfx.tile_size_px };
                    
                    Rectangle r = (Rectangle){
                        gridTileCoords.x + 4,
                        gridTileCoords.y + 4,
                        grid.gfx.tile_size_px - 8,
                        grid.gfx.tile_size_px - 8
                    };

                    DrawTriangle(ll, ur, ul, WHITE);
                    DrawTriangle(ll, lr, ur, GRAY);
                    DrawRectangleRec(r, LIGHTGRAY);
                }
                else{
                    // TODO Draw *something* for an empty cell
                }

                if(guiShowNeighborNums && (grid.tiles[i][j].neighbors != 0) && (camera.zoom > 0.45f)){
                    Color color = (grid.tiles[i][j].alive == true ? LIME : RED);
                    DrawText(TextFormat("%d", grid.tiles[i][j].neighbors), gridTileCoords.x+8, gridTileCoords.y+8, 20, color);
                }
                
            } // for j
        } // for i

        

        

        // Render mouse halo, tool ghost
        if(isMouseOnGrid){

            // Mouse Halo
            for(int i=camFrustumTiles.ul.x; i<camFrustumTiles.lr.x; i++){
                for(int j=camFrustumTiles.ul.y; j<camFrustumTiles.lr.y; j++){

                    Vector2 gridTileCoords = (Vector2){
                        grid.gfx.rect.x + (i * grid.gfx.tile_size_px),
                        grid.gfx.rect.y + (j * grid.gfx.tile_size_px)
                    };

                    Rectangle gridTileRect = (Rectangle){
                        .x = gridTileCoords.x,
                        .y = gridTileCoords.y,
                        .width = grid.gfx.tile_size_px,
                        .height = grid.gfx.tile_size_px
                    };

                    if(CheckCollisionCircleRec(mousePosWorld, grid.gfx.tile_size_px*CURSOR_GLOW_RADIUS, gridTileRect)){

                        int fadeFactor = 0;
                        int colorFactor = 0;
                        Color baseColor = BEIGE;
                        Color lineColor = BLANK;

                        for(int k=1; k<=CURSOR_GLOW_RADIUS; k++){
                            if(CheckCollisionCircleRec(mousePosWorld, k*grid.gfx.tile_size_px, gridTileRect)){
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

                } // for j
            } // for i
            

            if(toolbox == TB_NONE){

            }
            else if(toolbox == TB_SINGLE){
                Rectangle r = (Rectangle){
                    tileLastHovered.x * grid.gfx.tile_size_px,
                    tileLastHovered.y * grid.gfx.tile_size_px,
                    grid.gfx.tile_size_px,
                    grid.gfx.tile_size_px
                };
                DrawRectangleRec(r, Fade(SKYBLUE, toolFade) );
            }
            else if(IsGridEdgeTile(tileLastHovered.x, tileLastHovered.y, &grid) == false){
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

                DrawTool(tileLastHovered, tpSwitch, toolType, &grid, Fade(SKYBLUE, toolFade));
            }
        } // ismouseOnGrid
        

        // Grid Border
        DrawRectangleLinesEx((Rectangle){-4, -4, grid.size.x*grid.gfx.tile_size_px+4, grid.size.y*grid.gfx.tile_size_px+4}, (camera.zoom < 0.5f ? 12 : 8), GOLD);


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

    // Clear-Grid Button
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

    // Show FPS
    if(guiShowFps) DrawFPS(20, 20);

    // Show count of alive tiles.
    if(guiShowAlive) DrawText(TextFormat("%d Alive", grid.num_alive), 20, 40, 20, GREEN);


    DrawText(TextFormat("TileLastHov: %d, %d", tileLastHovered.x, tileLastHovered.y), 20, 80, 20, ORANGE);


    // Show help (controls, etc)
    if(showHelpOverlay) DrawHelpOverlay();

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

    // Simulation Speed Buttons
    rectSimSpeed.x = screen.width.half + gd.bHeights.quart;
    
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


    // Randomize Grid State Button
    if (GuiButton(btnGridRandomRect, "!") ) userWantsGridRandomState = true;

    
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


        ToolProps* tpSwitch = NULL;           
        switch(toolbox){

            case TB_SINGLE:
                tpSwitch = &tpSingle;
                break;

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
    
    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40, gd.bWidths.full, gd.bHeights.full }, "Return")) showMenuOverlay = false;

    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40 + 5 + gd.bHeights.full*1, gd.bWidths.full, gd.bHeights.full }, "Options")){
        refreshCamera = true;
        finishScreen = 2;
    }

    if (GuiButton((Rectangle){ rectMenuOverlay.x + 20, rectMenuOverlay.y + 40 + (5*2) + gd.bHeights.full*2, gd.bWidths.full, gd.bHeights.full }, "Exit to Menu")) showExitConfirmation = true;
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