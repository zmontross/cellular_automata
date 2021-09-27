

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


int CycleToolType(int current, bool clockwise);
ToolProps GetToolProp(int toolbox);
void UseTool(TileCoord tile, int toolbox, int toolType, Grid* grid);
void DrawTool(TileCoord tile, int toolbox, int toolType, Grid* grid, Color color);
void DrawToolPreview(int posX, int posY, int tileSizePx, int toolbox, int toolType, Color color);


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


ToolProps GetToolProp(int toolbox){

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

    return *tpSwitch;

} // end GetToolProp


//void UseTool(int x, int y, ToolProps* props, int toolType, Grid* grid){
void UseTool(TileCoord tile, int toolbox, int toolType, Grid* grid){

    ToolProps props = GetToolProp(toolbox);

    char* string = props.code;
    
    int i = 0;
    int col = 0;
    int row = 0;
    int colStart = 0;
    int rowStart = 0;
    
    switch(toolType){
        case TT_RIGHT:
            colStart = tile.x - (props.width)/2;
            rowStart = tile.y - (props.height)/2;
            col = colStart;
            row = rowStart;

            while(string[i]){
                if(string[i] == '1'){
                    grid->updates[col][row]= true;
                    col++;
                }
                else if(string[i] == '0'){
                    grid->updates[col][row] = false;
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
            colStart = tile.x + (props.height)/2;
            rowStart = tile.y - (props.width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->updates[col][row] = true;
                    row++;
                }
                else if(string[i] == '0'){
                    grid->updates[col][row] = false;
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
            colStart = tile.x + (props.width)/2;
            rowStart = tile.y + (props.height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->updates[col][row] = true;
                    col--;
                }
                else if(string[i] == '0'){
                    grid->updates[col][row] = false;
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
            colStart = tile.x - (props.height)/2;
            rowStart = tile.y + (props.width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                if(string[i] == '1'){
                    grid->updates[col][row] = true;
                    row--;
                }
                else if(string[i] == '0'){
                    grid->updates[col][row] = false;
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

void DrawTool(TileCoord tile, int toolbox, int toolType, Grid* grid, Color color){

    ToolProps props = GetToolProp(toolbox);

    char* string = props.code;
    
    int i = 0;
    int col = 0;
    int row = 0;
    int colStart = 0;
    int rowStart = 0;

    Rectangle r = (Rectangle){0, 0, grid->gfx.tile_size_px, grid->gfx.tile_size_px};
    
    switch(toolType){
        case TT_RIGHT:
            colStart = tile.x - (props.width)/2;
            rowStart = tile.y - (props.height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;

                if(string[i] == '1'){
                    DrawRectangleRec(r, color);
                    col++;
                }
                else if(string[i] == '0'){
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
            colStart = tile.x + (props.height)/2;   //3/2=1r1
            rowStart = tile.y - (props.width)/2;  //16/2=8r0
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;

                if(string[i] == '1'){
                    DrawRectangleRec(r, color);
                    row++;
                }
                else if(string[i] == '0'){
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
            colStart = tile.x + (props.width)/2;
            rowStart = tile.y + (props.height)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;
                if(string[i] == '1'){
                    DrawRectangleRec(r, color);
                    col--;
                }
                else if(string[i] == '0'){
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
            colStart = tile.x - (props.height)/2;
            rowStart = tile.y + (props.width)/2;
            col = colStart;
            row = rowStart;
            while(string[i]){
                r.x = col * r.width;
                r.y = row * r.height;
                if(string[i] == '1'){
                    DrawRectangleRec(r, color);
                    row--;
                }
                else if(string[i] == '0'){
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

void DrawToolPreview(int posX, int posY, int tileSizePx, int toolbox, int toolType, Color color){

    ToolProps props = GetToolProp(toolbox);

    char* string = props.code;
    
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

