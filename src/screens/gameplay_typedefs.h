#ifndef GAMEPLAY_TYPEDEFS_H
#define GAMEPLAY_TYPEDEFS_H

//----------------------------------------------------------------------------------
// Gameplay Static Data, Types, and Enumerations
//----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#define DEFAULT_TILE_COUNT 1000
#define DEFAULT_TILE_SIZE_PX 32 

#define TOOL_FADE_MIN 0.10f
#define TOOL_FADE_MAX 0.90f

#define CURSOR_GLOW_RADIUS 16

#define FRAME_COUNT_MAX 60

#define ZOOM_MIN 0.10f
#define ZOOM_MAX 1.00f
#define ZOOM_DFLT 0.70f
#define ZOOM_SENSITV_1 0.05f
#define ZOOM_SENSITV_2 0.01f
#define ZOOM_SENSITV_THRESHOLD 0.30f
#define ZOOM_CAMTRG_SENSITV 2.0f

#define DRAG_THRESHOLD 2

#define TESTING false

#include "raylib.h"
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
// #include "ricons.h"



typedef struct SimulationCounter {
    int framesPerCycle; // Ideally Target FPS, e.g. 60. Only set at initialization.
    int simsPerCycle;   // Number of simulations per cycle. This determines how frequently we iterate the simulation.
    int framesPerSim;   // Number of frames between simulations. This ensures simulations occur at an regularfrequency.
    int frames;         // Increment each loop, roll-over when equal framesPerSim
    int sims;           // Increment each time frames rolls-over. Roll-over when equal to simsPerCycle;
    bool paused;        // Do not update if paused.
} SimulationCounter;

enum SimulationsPerCycle { SPC_STEP=1, SPC_SLOW=2, SPC_MED=10, SPC_FAST=60 };

typedef struct Tile {
    bool alive;
    int neighbors;
} Tile;

typedef struct TileCoord{
    int x;
    int y;
} TileCoord;

typedef struct Grid {
    struct size{
        int x;
        int y;
    } size;
    int num_alive;
    Tile tiles[DEFAULT_TILE_COUNT][DEFAULT_TILE_COUNT];
    bool updates[DEFAULT_TILE_COUNT][DEFAULT_TILE_COUNT];

    struct gfx {
        int tile_size_px;
        Rectangle rect;
        Vector2 center_px;
    } gfx;
} Grid;

int GetNeighborsEx(int x, int y, Grid* grid);
bool IsGridEdgeTile(int x, int y, Grid* grid);
TileCoord GetTileLastHovered(Vector2 mousePosWorld, Grid* grid);

int GetNeighborsEx(int x, int y, Grid* grid){

    int numNeighbors = 0;

    int xMin = -1;
    int xMax = 1;
    int yMin = -1;
    int yMax = 1;

    if(x == 0){
        // Left Edge
        xMin = 0;
    }
    else if(x == grid->size.x-1){
        // Right Edge
        xMax = 0;
    }
    
    if(y == 0){
        // Top Edge
        yMin = 0;
    }
    else if(y == grid->size.y-1){
        // Bottom Edge
        yMax = 0;
    }

    for (int m = xMin; m <= xMax; m++){
        for (int n = yMin; n <= yMax; n++){
            // Exclude tile-under-judgement
            if( !(m==0 && n==0) ){
                numNeighbors += (grid->updates[x+m][y+n] == true ? 1 : 0);
            }

        }
    }

    return numNeighbors;
} // end GetNeighbors


// Check if tile at position x,y is on the edge of the grid.
bool IsGridEdgeTile(int x, int y, Grid* grid){
    return ((x == 0 ? true : (x == grid->size.x-1 ? true : false)) || (y == 0 ? true : (y == grid->size.y-1 ? true : false)));
} // end IsGridEdgeTile


TileCoord GetTileLastHovered(Vector2 mousePosWorld, Grid* grid){
    TileCoord tc = (TileCoord){0, 0};
    Vector2 tc_px = (Vector2){0.0f, 0.0f};

    // Check if mouse position is within grid boundaries (in pixels)
    // If yes, determine which tile is located at that positiion
    // It is assumed that the grid is drawn at world-pixel-coord [0,0]
    //  and all tiles are drawn at positive increments of x,y

    int grid_max_size_x_px = grid->size.x * grid->gfx.tile_size_px;
    int grid_max_size_y_px = grid->size.y * grid->gfx.tile_size_px;

    // Find 'x'
    if(mousePosWorld.x < 0.0f){
        tc_px.x = 0.0f;
    }
    else if(mousePosWorld.x > grid_max_size_x_px){
        tc_px.x = grid_max_size_x_px - 1;
    }
    else{
        tc_px.x = mousePosWorld.x;
    }

    // Find 'y'
    if(mousePosWorld.y < 0.0f){
        tc_px.y = 0.0f;
    }
    else if(mousePosWorld.y > grid_max_size_y_px){
        tc_px.y = grid_max_size_y_px - 1;
    }
    else{
        tc_px.y = mousePosWorld.y;
    }

    tc.x = (int)(tc_px.x / grid->gfx.tile_size_px);

    tc.y = (int)(tc_px.y / grid->gfx.tile_size_px);

    return tc;

} // end GetTileLastHovered



#ifdef __cplusplus
}
#endif


#endif // GAMEPLAY_TYPEDEFS_H