/**********************************************************************************************
*
*   raylib - Standard Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
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

#ifndef SCREENS_H
#define SCREENS_H
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

typedef struct Dimension{
    int quart;
    int half;
    int triq;
    int full;
}Dimension;

typedef struct Screen{
    Dimension width;
    Dimension height;
    Vector2 center;
} Screen;

typedef struct UserOptions{
    int screenWidth;
    int screenHeight;
    bool fullscreen;
    bool showToolboxRight;
} UserOptions;

typedef struct Frustum{
    Vector2 ul;
    Vector2 lr;
} Frustum;

typedef struct GuiData{
    Dimension bWidths;
    Dimension bHeights;
    struct rects{               // <-- "I just want this fucking thing done"
        Rectangle bar;
        struct toolbox{
            Rectangle tool;
            Rectangle category;
        }toolbox;
        struct simspeed{
            Rectangle speed;
            Rectangle progress;
        }simspeed;
    }rects;
    int bLargeSize;
    int margin;
}GuiData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
GameScreen currentScreen;
GameScreen prevScreen;

const char* applicationName;

const char* style_file;
const char* settings_file;
#define SETTINGS_DEFAULT "~resolution:1920x1080\n~fullscreen:n\n"

UserOptions useropts;

bool showToolboxRight;


Screen screen;

// Dimension bWidths;
// Dimension bHeights;


GuiData gd;




#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Screens.c Functions Declaration
//---------------------------------------------------------------------------------- 
void LoadSettings(const char* fname, UserOptions* opts);
void SaveSettings(const char* fname, UserOptions* opts);
bool StartsWith(const char* line, const char* word);

void TransitionScreens(int prevScreen, int nextScreen);
void UpdateGuiSizing(void);
void UpdateStruct_Dimension(Dimension* dimStruct, int fullVal);
void UpdateStruct_Screen(Screen* screenStruct);
void UpdateStruct_GuiData(GuiData* gdstruct);

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);
void UpdateResolution(int requestedScreenSizeX, int requestedScreenSizeY);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
void DrawMenuOverlay(void);
void DrawHelpOverlay(void);

//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);




#ifdef __cplusplus
}
#endif


#endif // SCREENS_H