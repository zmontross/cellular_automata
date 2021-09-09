#ifndef GAMEPLAY_TYPEDEFS_H
#define GAMEPLAY_TYPEDEFS_H

//----------------------------------------------------------------------------------
// Gameplay Static Data, Types, and Enumerations
//----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#define DFLT_GRID_SIZE_X 1024
#define DFLT_GRID_SIZE_Y 768
#define DFLT_TILE_SIZE_PX 32 

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
    Color color;
    int neighbors;
} Tile;

typedef struct Corners {
    Vector2 ul;
    Vector2 ur;
    Vector2 ll;
    Vector2 lr;
} Corners;

typedef struct Grid {
    Color color;
    int tileSizePx;
    Vector2 numTiles;
    Vector2 posCenterPx;
    Corners corners;
    int active;
    Rectangle rect;
    Tile tiles[DFLT_GRID_SIZE_X][DFLT_GRID_SIZE_Y];
} Grid;


int GetNeighborsEx(int x, int y, Grid* grid);
bool IsGridEdgeTile(int x, int y, Grid* grid);


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
    else if(x == grid->numTiles.x-1){
        // Right Edge
        xMax = 0;
    }
    
    if(y == 0){
        // Top Edge
        yMin = 0;
    }
    else if(y == grid->numTiles.y-1){
        // Bottom Edge
        yMax = 0;
    }

    for (int m = xMin; m <= xMax; m++){
        for (int n = yMin; n <= yMax; n++){
            // Exclude tile-under-judgement
            if( !(m==0 && n==0) ){
                numNeighbors += (grid->tiles[x+m][y+n].alive == true ? 1 : 0);
            }

        }
    }

    return numNeighbors;
} // end GetNeighbors


// Check if tile at position x,y is on the edge of the grid.
bool IsGridEdgeTile(int x, int y, Grid* grid){
    return ((x == 0 ? true : (x == grid->numTiles.x-1 ? true : false)) || (y == 0 ? true : (y == grid->numTiles.y-1 ? true : false)));
} // end IsGridEdgeTile





#ifdef __cplusplus
}
#endif


#endif // GAMEPLAY_TYPEDEFS_H