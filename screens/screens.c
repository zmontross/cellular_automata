
#include "raylib.h"
#include "screens.h"
//#include "raygui.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
//#include "ricons.h"
#undef RAYGUI_IMPLEMENTATION

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------

static bool onStartup = true;

//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

void LoadSettings(const char* fname, UserOptions* opts){

    #define BUF_LINE 256

    FILE *setFile = fopen(fname, "rt");

    // Note: UserOptions will be initialized here.

    if(setFile != NULL){
        char buffer[BUF_LINE] = {0};
        fgets(buffer, BUF_LINE, setFile);

        if (buffer[0] == '~'){

            while(!feof(setFile)){

                if(StartsWith(buffer, (const char*)"~resolution")){
                    int width = 0;
                    int height = 0;
                    sscanf(buffer, "~resolution:%dx%d", &width, &height);
                    opts->screenWidth = width;
                    opts->screenHeight = height;
                }
                else if(StartsWith(buffer, (const char*)"~fullscreen")){
                    char fullScreenYesNo;
                    sscanf(buffer, "~fullscreen:%c", &fullScreenYesNo);
                    
                    switch(fullScreenYesNo){
                        case 'y':
                            opts->fullscreen = true;
                            break;
                        case 'n':
                        default:
                            opts->fullscreen = false;
                            break;
                    }

                }

                fgets(buffer, 256, setFile);
            }
        }
    }
    fclose(setFile);

} // end LoadSettings()


void SaveSettings(const char* fname, UserOptions* opts){

    #define BUF_FILE 512

    char buffer[BUF_FILE] = {0};

    sprintf(buffer, "~resolution:%dx%d\n~fullscreen:%c\n", opts->screenWidth, opts-> screenHeight, (opts->fullscreen? 'y' : 'n'));

    SaveFileText(fname, buffer);

} // end SaveSettings()


bool StartsWith(const char* line, const char* word){
    int wlen = strlen(word);    
    for(int i=0; i<wlen; i++){
        if( (word[i]=='\0') || (line[i]=='\n') ) return false;
        else if(line[i] != word[i]) return false;
    }
    return true;
} // end isKeyEqual()


void TransitionScreens(int prevScreen, int nextScreen){
    // Unload the last screen
    switch(prevScreen) {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Load/Init the next screen
    switch(nextScreen) {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }
}

void UpdateGuiSizing(void){
    if(onStartup){
        // GuiLoadStyle() comes from RayGui which isn't defined for main.c....
        // Attempting to do that was a trashfire....
        onStartup = false;
        if(FileExists(stylefile))
            GuiLoadStyle(stylefile);
    }

    UpdateStruct_Screen(&screen);
    UpdateStruct_GuiData(&gd);
}

void UpdateStruct_Dimension(Dimension* dimStruct, int fullVal){
    dimStruct->full = fullVal;
    dimStruct->half = fullVal/2;
    dimStruct->quart = fullVal/4;
    dimStruct->triq = dimStruct->half + dimStruct->quart ;
}

void UpdateStruct_Screen(Screen* screenStruct){

    UpdateStruct_Dimension(&screenStruct->width, GetScreenWidth());
    UpdateStruct_Dimension(&screenStruct->height, GetScreenHeight());

    screenStruct->center = (Vector2){ screen.width.half, screen.height.half };
}

void UpdateStruct_GuiData(GuiData* gdstruct){
    UpdateStruct_Dimension(&(gdstruct->bWidths), (screen.width.quart / 2));
    UpdateStruct_Dimension(&(gdstruct->bHeights), (screen.height.quart / 4));

    gdstruct->margin = 4;

    gdstruct->rects.bar = (Rectangle){ 
                                0,
                                screen.height.full - gdstruct->bHeights.triq - gdstruct->margin*8,
                                screen.width.full,
                                gdstruct->bHeights.triq + gdstruct->margin*8
                            };

    

    gdstruct->bLargeSize = gdstruct->bHeights.full*2 - gdstruct->margin*2;

    // Something just screams "NO WHAT ARE YOU DOING?!"...
    gdstruct->rects.toolbox.tool = (Rectangle){
                                            gdstruct->rects.bar.x + gdstruct->bLargeSize + gdstruct->bHeights.quart,
                                            gdstruct->rects.bar.y + gdstruct->bHeights.quart - gdstruct->margin,
                                            screen.width.half,
                                            gdstruct->bHeights.full
                                        };
    gdstruct->rects.toolbox.category = (Rectangle){
                                            gdstruct->rects.toolbox.tool.x,
                                            gdstruct->rects.toolbox.tool.y + gdstruct->rects.toolbox.tool.height,
                                            gdstruct->rects.toolbox.tool.width,
                                            gdstruct->bHeights.triq
                                            };
    gdstruct->rects.simspeed.speed = (Rectangle){
                                            screen.width.half + screen.width.quart/2 - gdstruct->margin*2,
                                            gdstruct->rects.toolbox.tool.y,
                                            screen.width.quart,
                                            gdstruct->rects.toolbox.tool.height
                                        };
    gdstruct->rects.simspeed.progress = (Rectangle){
                                            gdstruct->rects.simspeed.speed.x,
                                            gdstruct->rects.simspeed.speed.y + gdstruct->rects.simspeed.speed.height,
                                            gdstruct->rects.simspeed.speed.width,
                                            gdstruct->bHeights.quart
                                        };
}

