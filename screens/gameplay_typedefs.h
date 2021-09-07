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







enum ToolOffsets{   TOS_SHIP=11,
                    TOS_STILL=21,
                    TOS_OSCILLATOR=31,
                    TOS_OTHER=41
};

enum ToolBox{   TB_NONE=0,                                                                       // Nothing
                TB_GLIDER=TOS_SHIP, TB_LWSHIP, TB_MWSHIP, TB_HWSHIP,                             // Self-propagating
                TB_BLOCK=TOS_STILL, TB_BEEHIVE, TB_LOAF, TB_BOAT, TB_TUB,                        // "Still-Life". Stable, no movement
                TB_BLINKER=TOS_OSCILLATOR, TB_TOAD, TB_BEACON, TB_PULSAR, TB_PENTADECATHLON,     // Oscillators
                TB_SINGLE=TOS_OTHER,                                                             // Single Tile
                TB_GOSPER,                                                                       // Guns that make gliders (TB_GLIDER)
                TB_RIPCONWAY                                                                     // XKCD, RIP John Conway (26DEC1937-11APR2020)
                };

enum ToolType{TT_RIGHT = 0, TT_DOWN, TT_LEFT, TT_UP};


typedef struct ToolProps{
    int width;
    int height;
    char* code;
} ToolProps;


ToolProps tpGlider = {
    .width = 3,
    .height = 3,
    .code = "011\n101\n001\n"
};

ToolProps tpLwSpaceship = {
    .width = 5,
    .height = 4,
    .code = "01111\n10001\n00001\n10010\n"
};

ToolProps tpMwSpaceship = {
    .width = 6,
    .height = 5,
    .code = "011111\n100001\n000001\n100010\n001000\n"
};

ToolProps tpHwSpaceship = {
    .width = 7,
    .height = 5,
    .code = "0111111\n1000001\n0000001\n1000010\n0011000\n"
};

ToolProps tpBlock = {
    .width = 2,
    .height = 2,
    .code = "11\n11\n"
};

ToolProps tpBeehive = {
    .width = 4,
    .height = 3,
    .code = "0110\n1001\n0110\n"
};

ToolProps tpLoaf = {
    .width = 4,
    .height = 4,
    .code = "0110\n1001\n0101\n0010\n"
};

ToolProps tpBoat = {
    .width = 3,
    .height = 3,
    .code = "110\n101\n010\n"
};

ToolProps tpTub = {
    .width = 3,
    .height = 3,
    .code = "010\n101\n010\n"
};

ToolProps tpBlinker = {
    .width = 3,
    .height = 3,
    .code = "010\n010\n010\n"
};

ToolProps tpToad = {
    .width = 4,
    .height = 4,
    .code = "0010\n1001\n1001\n0100\n"
};

ToolProps tpBeacon = {
    .width = 4,
    .height = 4,
    .code = "1100\n1100\n0011\n0011\n"
};

ToolProps tpPulsar = {
    .width = 13,
    .height = 13,
    .code = "0011100011100\n0000000000000\n1000010100001\n1000010100001\n1000010100001\n0011100011100\n0000000000000\n0011100011100\n1000010100001\n1000010100001\n1000010100001\n0000000000000\n0011100011100\n"
};

ToolProps tpPentaDecathlon = {
    .width = 3,
    .height = 16,
    .code = "010\n010\n111\n000\n000\n111\n010\n010\n010\n010\n111\n000\n000\n111\n010\n010\n"
};

ToolProps tpSingle = {
    .width = 1,
    .height = 1,
    .code = "1\n"
};

ToolProps tpGosper = {
    .width = 36,
    .height = 9, // life is painnnn
    .code = "000000000000000000000000100000000000\n000000000000000000000010100000000000\n000000000000110000001100000000000011\n000000000001000100001100000000000011\n110000000010000010001100000000000000\n110000000010001011000010100000000000\n000000000010000010000000100000000000\n000000000001000100000000000000000000\n000000000000110000000000000000000000\n"
};

ToolProps tpRipConway = {
    .width = 7,
    .height = 9,
    .code = "0011100\n0010100\n0010100\n0001000\n1011100\n0101010\n0001001\n0010100\n0010100\n"
};



int guiCategorySelected;
int guiToolboxSelection;
int listViewExScrollIndex;
int guiSimsPerCycleSelected;

#define N_CATEGORIES 4
enum ToolCategories {   TCAT_STILL=0,
                        TCAT_OSC,
                        TCAT_SHIP,
                        TCAT_OTHER
                    };
const char* guiLabels_categories[N_CATEGORIES] = {
    "Still Life",
    "Oscillators",
    "Ships",
    "Other"
};


#define N_CAT_SHIPS 4
const char* guiLabels_ships[N_CAT_SHIPS] = {
    "Glider",
    "Light Ship",
    "Medium Ship",
    "Heavy Ship",
};

#define N_CAT_STILLS 5
const char* guiLabels_stills[N_CAT_STILLS] = {
    "Block",
    "Beehive",
    "Loaf",
    "Boat",
    "Tub",
};

#define N_CAT_OSCILLATORS 5
const char* guiLabels_oscillators[N_CAT_OSCILLATORS] = {
    "Blinker",
    "Toad",
    "Beacon",
    "Pulsar",
    "Pentadecathlon"
};

#define N_CAT_OTHER 3
const char* guiLabels_other[N_CAT_OTHER] = {
    "Single Tile",
    "Gosper Glider Gun",
    "XKCD Conway"
};



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





//                                                                                                
// 88        88         db         888b      88  88888888ba,    88           88888888888  ad88888ba   
// 88        88        d88b        8888b     88  88      `"8b   88           88          d8"     "8b  
// 88        88       d8'`8b       88 `8b    88  88        `8b  88           88          Y8,          
// 88aaaaaaaa88      d8'  `8b      88  `8b   88  88         88  88           88aaaaa     `Y8aaaaa,    
// 88""""""""88     d8YaaaaY8b     88   `8b  88  88         88  88           88"""""       `"""""8b,  
// 88        88    d8""""""""8b    88    `8b 88  88         8P  88           88                  `8b  
// 88        88   d8'        `8b   88     `8888  88      .a8P   88           88          Y8a     a8P  
// 88        88  d8'          `8b  88      `888  88888888Y"'    88888888888  88888888888  "Y88888P"   
//                                                                                                 
// http://patorjk.com/software/taag/#p=display&f=Univers&t=HANDLES                                                                                                   
//

int GetNeighborsEx(int x, int y, Grid* grid);
bool IsGridEdgeTile(int x, int y, Grid* grid);
void UpdateGlider(int x, int y, int direction, Grid* grid);
void DrawGlider(int x, int y, int size, int direction, Color color);
int CycleGlider(int gliderCurrent, bool clockwise);
int CycleToolType(int current, bool clockwise);
void UseTool(Vector2 tile, ToolProps* props, int toolType, Grid* grid);
void DrawTool(Vector2 tile, ToolProps* props, int toolType, Grid* grid, Color color);
void DrawToolPreview(int posX, int posY, int tileSizePx, ToolProps* props, int toolType, Color color);
int SpookyGui_InterlockedButtonsHorizontal(Rectangle bounds, const char** labels, int count, int active);


//                                                                                                                    
// 88888888888  88        88  888b      88    ,ad8888ba,  888888888888  88    ,ad8888ba,    888b      88   ad88888ba   
// 88           88        88  8888b     88   d8"'    `"8b      88       88   d8"'    `"8b   8888b     88  d8"     "8b  
// 88           88        88  88 `8b    88  d8'                88       88  d8'        `8b  88 `8b    88  Y8,          
// 88aaaaa      88        88  88  `8b   88  88                 88       88  88          88  88  `8b   88  `Y8aaaaa,    
// 88"""""      88        88  88   `8b  88  88                 88       88  88          88  88   `8b  88    `"""""8b,  
// 88           88        88  88    `8b 88  Y8,                88       88  Y8,        ,8P  88    `8b 88          `8b  
// 88           Y8a.    .a8P  88     `8888   Y8a.    .a8P      88       88   Y8a.    .a8P   88     `8888  Y8a     a8P  
// 88            `"Y8888Y"'   88      `888    `"Y8888Y"'       88       88    `"Y8888Y"'    88      `888   "Y88888P"   
//                                                                                                                  
// http://patorjk.com/software/taag/#p=display&f=Univers&t=FUNCTIONS                                                                                                                    
//

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


// Cycles (or gives the next state) of the given tooltype.
int CycleToolType(int current, bool clockwise){
    
    int next;

    if(clockwise){
        switch(current){
            case(TT_RIGHT):
            case(TT_DOWN):
            case(TT_LEFT):
                next = current+1;
                break;
            case(TT_UP):
                next = TT_RIGHT;
                break;
            default:
                next = TT_RIGHT;
                break;
        }
    }
    else{
        switch(current){
            case(TT_UP):
            case(TT_LEFT):
            case(TT_DOWN):
                next = current-1;
                break;
            case(TT_RIGHT):
                next = TT_UP;
                break;
            default:
                next = TT_UP;
                break;
        }
    }

    return next;

} // end CycleToolType()



//void UseTool(int x, int y, ToolProps* props, int toolType, Grid* grid){
void UseTool(Vector2 tile, ToolProps* props, int toolType, Grid* grid){

    char* string = props->code;
    
    int i = 0;
    int col = 0;
    int row = 0;
    int colStart = 0;
    int rowStart = 0;
    
    switch(toolType){
        case TT_RIGHT:
            colStart = tile.x - (props->width)/2;
            rowStart = tile.y - (props->height)/2;
            col = colStart;
            row = rowStart;

            while(string[i]){
                if(string[i] == '1'){
                    grid->tiles[col][row].alive = true;
                    col++;
                }
                else if(string[i] == '0'){
                    grid->tiles[col][row].alive = false;
                    col++;
                }
                else{
                    row++;
                    col = colStart;
                }
                i++;
            }
            break;

        case TT_DOWN:
            colStart = tile.x + (props->height)/2;
            rowStart = tile.y - (props->width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->tiles[col][row].alive = true;
                    row++;
                }
                else if(string[i] == '0'){
                    grid->tiles[col][row].alive = false;
                    row++;
                }
                else{
                    col--;
                    row = rowStart;
                }
                i++;
            }
            break;

        case TT_LEFT:
            colStart = tile.x + (props->width)/2;
            rowStart = tile.y + (props->height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->tiles[col][row].alive = true;
                    col--;
                }
                else if(string[i] == '0'){
                    grid->tiles[col][row].alive = false;
                    col--;
                }
                else{
                    row--;
                    col = colStart;
                }
                i++;
            }
            break;

        case TT_UP:
            colStart = tile.x - (props->height)/2;
            rowStart = tile.y + (props->width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->tiles[col][row].alive = true;
                    row--;
                }
                else if(string[i] == '0'){
                    grid->tiles[col][row].alive = false;
                    row--;
                }
                else{
                    col++;
                    row = rowStart;
                }
                i++;
            }
            break;

        default:
            break;
    }
} // end UseTool()
//3*16

void DrawTool(Vector2 tile, ToolProps* props, int toolType, Grid* grid, Color color){
    char* string = props->code;
    
    int i = 0;
    int col = 0;
    int row = 0;
    int colStart = 0;
    int rowStart = 0;

    Rectangle r = (Rectangle){0, 0, grid->tileSizePx, grid->tileSizePx};
    
    switch(toolType){
        case TT_RIGHT:
            colStart = tile.x - (props->width)/2;
            rowStart = tile.y - (props->height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;

                if(string[i] == '1'){
                    //grid->tiles[col][row].alive = true;
                    DrawRectangleRec(r, color);
                    col++;
                }
                else if(string[i] == '0'){
                    //grid->tiles[col][row].alive = false;
                    col++;
                }
                else{
                    row++;
                    col = colStart;
                }
                i++;
            }
            break;

        case TT_DOWN:
            colStart = tile.x + (props->height)/2;   //3/2=1r1
            rowStart = tile.y - (props->width)/2;  //16/2=8r0
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;

                if(string[i] == '1'){
                    //grid->tiles[col][row].alive = true;
                    DrawRectangleRec(r, color);
                    row++;
                }
                else if(string[i] == '0'){
                    //grid->tiles[col][row].alive = false;
                    row++;
                }
                else{
                    col--;
                    row = rowStart;
                }
                i++;
            }
            break;

        case TT_LEFT:
            colStart = tile.x + (props->width)/2;
            rowStart = tile.y + (props->height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;
                if(string[i] == '1'){
                    //grid->tiles[col][row].alive = true;
                    DrawRectangleRec(r, color);
                    col--;
                }
                else if(string[i] == '0'){
                    //grid->tiles[col][row].alive = false;
                    col--;
                }
                else{
                    row--;
                    col = colStart;
                }
                i++;
            }
            break;

        case TT_UP:
            colStart = tile.x - (props->height)/2;
            rowStart = tile.y + (props->width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;
                if(string[i] == '1'){
                    //grid->tiles[col][row].alive = true;
                    DrawRectangleRec(r, color);
                    row--;
                }
                else if(string[i] == '0'){
                    //grid->tiles[col][row].alive = false;
                    row--;
                }
                else{
                    col++;
                    row = rowStart;
                }
                i++;
            }
            break;

        default:
            break;
    }
} // end DrawTool()



void DrawToolPreview(int posX, int posY, int tileSizePx, ToolProps* props, int toolType, Color color){
    char* string = props->code;
    
    int i = 0;

    Rectangle r = (Rectangle){posX, posY, tileSizePx, tileSizePx};

    while(string[i]){

        if(string[i] == '1'){
            DrawRectangleRec(r, color);
            r.x += r.width;
        }
        else if(string[i] == '0'){
            r.x += r.width;
        }
        else{
            r.y += r.height;
            r.x = posX;
        }
        i++;
    }
    
} // end DrawToolPreview()





int SpookyGui_InterlockedButtonsHorizontal(Rectangle bounds, const char** labels, int count, int active){

    #ifndef SPOOKYGUI_MARGIN
        #define SPOOKYGUI_MARGIN 4
    #endif

    int itemSelected = active;

    // Establish a base button rectangle from which all will be derived.
    // As many whole buttons will be fit into the space with surrounding margins.

    int bwidth = ((int)bounds.width / count) - (SPOOKYGUI_MARGIN * (count - 1));

    int bspacing = (bounds.width - (count * bwidth)) / count;
    // int bspacing = SPOOKYGUI_MARGIN;

    Rectangle r = (Rectangle){
        .x = bounds.x + SPOOKYGUI_MARGIN,
        .y = bounds.y + SPOOKYGUI_MARGIN,
        .width = bwidth,
        .height = bounds.height - (SPOOKYGUI_MARGIN * 2)
    };


    for(int i=0; i<count; i++){

        if(i>0){
            r.x = r.x + r.width + bspacing;
        }
       
        GuiSetState( (active == i? GUI_STATE_PRESSED : GUI_STATE_NORMAL) );

        if( GuiButton(r, labels[i]) ) itemSelected = (itemSelected==i? 0 : i);

        GuiSetState(GUI_STATE_NORMAL);

    }

    return itemSelected;
}

bool GuiPauseButton(Rectangle r, bool b){

    bool retval = false;
    // Pause Button ~ manually draw rectangles because using RAYICONS yields a microscopic pause symbol.
    Rectangle rp1 = (Rectangle){r.x + r.width/2 - r.width/8 - r.width/4, r.y + r.height/8, r.width/4, r.height - r.height/4};
    Rectangle rp2 = (Rectangle){r.x + r.width/2 + r.width/8, rp1.y, rp1.width, rp1.height};

    retval = GuiButton(r, NULL);
    DrawRectangleRec(rp1, GetColor(GuiGetStyle(LABEL, (GuiGetState() == GUI_STATE_DISABLED)? TEXT_COLOR_DISABLED : TEXT_COLOR_NORMAL)));
    DrawRectangleRec(rp2, GetColor(GuiGetStyle(LABEL, (GuiGetState() == GUI_STATE_DISABLED)? TEXT_COLOR_DISABLED : TEXT_COLOR_NORMAL)));

    return retval;
}

#ifdef __cplusplus
}
#endif


#endif // GAMEPLAY_TYPEDEFS_H