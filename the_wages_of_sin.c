// The wages of sin are blood... and bad code...

ToolProps tpGosper = {
    .width = 36,
    .height = 9, // life is painnnn
    .code = "000000000000000000000000100000000000\n000000000000000000000010100000000000\n000000000000110000001100000000000011\n000000000001000100001100000000000011\n110000000010000010001100000000000000\n110000000010001011000010100000000000\n000000000010000010000000100000000000\n000000000001000100000000000000000000\n000000000000110000000000000000000000\n"
};


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